#include <glad/glad.h>
#include <SDL.h>
#include <SDL_mixer.h>

#include <iostream>
#include <ostream>

#include "PhysicsBody.hpp"
#include "Player.hpp"
#include "Shader.hpp"
#include "Buffer.hpp"

#include "Texture.hpp"
#include "Timestep.hpp"
#include "math/Mat4.hpp"
#include "math/Vec2.hpp"
#include "math/AABB.hpp"
#include "math/Transform.hpp"

#include "Input.hpp"
#include "Physics.hpp"

#include "ParticleSystem.hpp"

struct Box {
    Buffer vbo, ebo;
    AABB bounds;
    uint32_t vao;

    void bind() {
        vbo.bind();
        ebo.bind();
        glBindVertexArray(vao);
    }
};

Box makeBox(AABB bounds) {
    float vertices[] = {
        bounds.min.x, bounds.min.y, 0.0f, 0.0f,
        bounds.min.x, bounds.max.y, 0.0f, 1.0f,
        bounds.max.x, bounds.max.y, 1.0f, 1.0f,
        bounds.max.x, bounds.min.y, 1.0f, 0.0f,
    };
    int elements[] = {
        0, 1, 2,
        0, 2, 3
    };

    Buffer vbo(GL_ARRAY_BUFFER, vertices, sizeof(vertices));
    vbo.bind();
    Buffer ebo(GL_ELEMENT_ARRAY_BUFFER, elements, sizeof(elements));
    ebo.bind();

    uint32_t vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, (void*)8);

    return Box { vbo, ebo, bounds, vao };
}

struct AppData {
    SDL_Window *window;
    SDL_GLContext glcontext;
};

bool init(AppData &data) {
    if (Mix_Init(MIX_INIT_MP3) == 0) {
        std::cout << "Mix error: " << Mix_GetError() << "\n";
        return false;
    }
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        std::cout << "SDL Error: " << SDL_GetError() << "\n";
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Mix error: " << Mix_GetError() << "\n";
        return false;
    }

    int window_flags = SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN;
    SDL_Window *window = SDL_CreateWindow("Battle Squares", 0, 0, 0, 0, window_flags);
    if (window == nullptr) {
        std::cout << "SDL Error: " << SDL_GetError() << "\n";
        return false;
    }
    SDL_SetWindowSize(window, 1280, 720);
    SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_ShowWindow(window);

    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    if (gladLoadGLLoader(SDL_GL_GetProcAddress) == GL_FALSE) {
        std::cout << "GL Error: " << glGetError() << "\n";
        return false;
    }

    data.window = window;
    data.glcontext = glcontext;
    return true;
}

bool quit(AppData &data) {
    Mix_Quit();
    SDL_GL_DeleteContext(data.glcontext);
    SDL_DestroyWindow(data.window);
    SDL_Quit();
    return true;
}

int main(int argc, char *argv[]) {
    AppData appdata;
    if (!init(appdata)) {
        std::cout << "Failed to init\n";
        return 1;
    }

    Shader shader("assets/shaders/default.vert", "assets/shaders/default.frag");
    Shader particle_shader("assets/shaders/particles.vert", "assets/shaders/particles.frag");

    Mat4 proj = Mat4::createOrtho(-8.0f, 8.0f, -4.5f, 4.5f, 0.0f, 1.0f);

    shader.setMat4("u_Proj", proj);
    shader.setMat4("u_View", Mat4::identity());
    particle_shader.setMat4("u_Proj", proj);
    particle_shader.setMat4("u_View", Mat4::identity());

    int elements[] = {
        0, 1, 2,
        0, 2, 3
    };
    Buffer player_ebo(GL_ELEMENT_ARRAY_BUFFER, elements, sizeof(elements));

    AABB player_model = AABB::centerSize(Vec2::zero(), Vec2::one());
    float vertices[] = {
        player_model.min.x, player_model.min.y, 0.0f, 0.75f,
        player_model.min.x, player_model.max.y, 0.0f,  1.0f,
        player_model.max.x, player_model.max.y, 0.25f, 1.0f,
        player_model.max.x, player_model.min.y, 0.25f, 0.75f,
    };

    Buffer player_vbo(GL_ARRAY_BUFFER, vertices, sizeof(vertices));
    player_vbo.bind();

    player_ebo.bind();
    uint32_t vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, (void*)8);

    Transform trans = Transform::identity();
    Vec2 velocity = Vec2::zero();

    Box boxes[] = {
        makeBox(AABB(-8.0f, -12.0f, 8.0f, -4.0f)),
        makeBox(AABB(4.0f, -4.0f, 11.0f, 3.0f)),
        makeBox(AABB(1.0f, -2.0f, 1.5f, -1.5f)),
        makeBox(AABB(1.0f, -4.0f, 3.0f, -2.0f)),
    };

    Physics physics;
    for (Box box : boxes) {
        physics.addBody(new PhysicsBody(box.bounds.center(), box.bounds.size(), true));
    }

    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

    Texture skins_texture("assets/textures/skins.png");
    Texture wall_texture("assets/textures/wall.png");

    bool running = true;
    SDL_Event event;

    Input input;
    ParticleSystem particle_system;
    Player player = Player(particle_system);
    physics.addBody(player.getBody());

    Timestep ts;
    while (running) {
        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    input.keyCallback(event.key.keysym.sym, event.key.keysym.mod, SDL_KEYDOWN);
                    break;
                case SDL_KEYUP:
                    input.keyCallback(event.key.keysym.sym, event.key.keysym.mod, SDL_KEYUP);
                    break;
            }
        }

        physics.step(ts);
        particle_system.update(ts);

        if (input.isKeyDown(SDLK_q)) {
            player.setPos(Vec2::zero());
            velocity = Vec2::zero();
        }

        if (input.isKeyPressed(SDLK_ESCAPE)) {
            running = false;
        }

        player.update(ts, input, physics);

        glClear(GL_COLOR_BUFFER_BIT);

        shader.bind();
        skins_texture.bind();

        player_ebo.bind();
        player_vbo.bind();
        glBindVertexArray(vao);

        shader.setMat4("u_Model", player.getMatrix());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        wall_texture.bind();
        for (Box box : boxes) {
            box.bind();
            shader.setMat4("u_Model", Mat4::identity());
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        }

        particle_shader.bind();
        particle_system.render();

        SDL_GL_SwapWindow(appdata.window);

        input.update();

        ts.delta = (SDL_GetTicks() - ts.current) * 0.001f;
        ts.current = SDL_GetTicks();
    }

    if (!quit(appdata)) {
        std::cout << "Failed to quit\n";
        return 1;
    }

    return 0;
}
