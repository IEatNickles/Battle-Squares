#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>

#include "Shader.hpp"
#include "Buffer.hpp"

#include "Texture.hpp"
#include "math/Mat4.hpp"
#include "math/Vec2.hpp"
#include "math/AABB.hpp"
#include "math/Transform.hpp"

float lerp(float a, float b, float t) {
    return t * (b - a) + a;
}

float invLerp(float a, float b, float v) {
    return (v - a) / (b - a);
}

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

struct RaycastHit {
    Vec2 point;
    Vec2 normal;
    float distance;
};

bool raycastLine(Vec2 a, Vec2 b, Vec2 origin, Vec2 end, RaycastHit& hit) {
    float x1 = a.x;
    float y1 = a.y;
    float x2 = b.x;
    float y2 = b.y;
    float x3 = origin.x;
    float y3 = origin.y;
    float x4 = end.x;
    float y4 = end.y;

    float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / d;
    float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / d;

    if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f) {
        Vec2 hit_point = Vec2(x1 + t * (x2 - x1), y1 + t * (y2 - y1));
        hit = RaycastHit { hit_point, Vec2(-(b.y - a.y), b.x - a.x).normalized(), hit_point.sub(end).length() };
        return true;
    }
    return false;
}

bool lineHitsAABB(Vec2 a, Vec2 dir, const AABB& bounds, RaycastHit& hit) {
    Vec2 inv_dir = dir.reciporical();

    Vec2 t0 = bounds.min.sub(a).mul(inv_dir);
    Vec2 t1 = bounds.max.sub(a).mul(inv_dir);

    if (std::isnan(t0.x) || std::isnan(t0.y)) {
        return false;
    }
    if (std::isnan(t1.x) || std::isnan(t1.y)) {
        return false;
    }

    if (t0.x > t1.x) {
        std::swap(t0.x, t1.x);
    }
    if (t0.y > t1.y) {
        std::swap(t0.y, t1.y);
    }

    if (t0.x > t1.y || t0.y > t1.x) {
        return false;
    }

    float near = std::max(t0.x, t0.y);
    float far = std::min(t1.x, t1.y);

    if (far < 0.0f) {
        return false;
    }

    hit.distance = near;
    hit.point = a.add(dir.mul(near));

    if (t0.x > t0.y) {
        if (inv_dir.x < 0.0f) {
            hit.normal = Vec2(1.0f, 0.0f);
        } else {
            hit.normal = Vec2(-1.0f, 0.0f);
        }
    } else if (t0.x < t0.y) {
        if (inv_dir.y < 0.0f) {
            hit.normal = Vec2(0.0f, 1.0f);
        } else {
            hit.normal = Vec2(0.0f, -1.0f);
        }
    }

    return true;
}

int main() {
    if (glfwInit() == GLFW_FALSE) {
        printf("No glfw?\n");
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Battle Squares", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == GL_FALSE) {
        printf("No glad?\n");
        return 2;
    }

    Shader shader("assets/shaders/default.vert", "assets/shaders/default.frag");

    Mat4 ortho = Mat4::createOrtho(-8.0f, 8.0f, -4.5f, 4.5f, 0.0f, 1.0f);

    int elements[] = {
        0, 1, 2,
        0, 2, 3
    };
    Buffer ebo(GL_ELEMENT_ARRAY_BUFFER, elements, sizeof(elements));

    AABB f = AABB::centerSize(Vec2::zero(), Vec2::one());
    float vertices[] = {
        f.min.x, f.min.y, 0.0f, 0.75f,
        f.min.x, f.max.y, 0.0f,  1.0f,
        f.max.x, f.max.y, 0.25f, 1.0f,
        f.max.x, f.min.y, 0.25f, 0.75f,
    };

    Buffer vbo(GL_ARRAY_BUFFER, vertices, sizeof(vertices));
    vbo.bind();

    ebo.bind();
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

    bool grounded = false;
    int jumps = 3;
    float jumpStart = 0.0f;

    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

    Vec2 squish = Vec2::one();

    Texture skins("assets/textures/skins.png");
    Texture wall("assets/textures/wall.png");

    float delta_time;
    float last_time;

    shader.bind();
    while (!glfwWindowShouldClose(window)) {
        if (jumps > 0 && glfwGetTime() - jumpStart > 0.2f && glfwGetKey(window, GLFW_KEY_W)) {
            velocity.y = sqrt(2.0f * 2.0f * 30.0f);
            jumps -= 1;
            jumpStart = glfwGetTime();
        }

        if (glfwGetKey(window, GLFW_KEY_D)) {
            velocity.x = lerp(velocity.x, 7.0f, 10.0f * delta_time);
        } else if (glfwGetKey(window, GLFW_KEY_A)) {
            velocity.x = lerp(velocity.x, -7.0f, 10.0f * delta_time);
        } else {
            velocity.x = lerp(velocity.x, 0.0f, 10.0f * delta_time);
        }

        float t = -velocity.y / 20.0f;
        Vec2 target_squish = Vec2(lerp(1.0f, 0.75f, t), lerp(1.0f, 1.5f, t));
        squish.x = lerp(squish.x, target_squish.x, delta_time * 20.0f);
        squish.y = lerp(squish.y, target_squish.y, delta_time * 20.0f);

        velocity.y -= 30.0f * delta_time;

        RaycastHit hit;
        AABB player_bounds = AABB::centerSize(trans.translation, Vec2::one());
        for (Box box : boxes) {
            Vec2 pb_size = player_bounds.size();
            AABB bounds = box.bounds.inflate(pb_size.x, pb_size.y);
            if (player_bounds.intersects(bounds)) {
                if (lineHitsAABB(trans.translation, velocity.mul(delta_time), bounds, hit)) {
                    if (hit.distance >= 0.0f && hit.distance < 1.0f) {
                        velocity = velocity.add(hit.normal.mul(velocity.abs()).mul(1.0f - hit.distance));
                        if (hit.normal.y > 0.0f) {
                            jumps = 3;
                        }
                    }
                }
            }
        }

        if (glfwGetKey(window, GLFW_KEY_Q)) {
            trans.translation = Vec2::zero();
            velocity = Vec2::zero();
            jumps = 3;
        }

        trans.translation = trans.translation.add(velocity.mul(delta_time));

        glClear(GL_COLOR_BUFFER_BIT);

        skins.bind();

        ebo.bind();
        vbo.bind();
        glBindVertexArray(vao);

        Vec2 shear = Vec2(lerp(0.0f, 0.25f, (velocity.y == 0.0f ? 1.0 : (velocity.y * 0.1f)) * (velocity.x / 7.0f)), 0.0f);
        shader.setMat4("u_MVP", ortho.mul(trans.getMatrix().mul(Mat4::fromShear(shear)).mul(Mat4::fromScale(squish))));
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        wall.bind();
        for (Box box : boxes) {
            box.bind();
            shader.setMat4("u_MVP", ortho);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        }

        glfwPollEvents();
        glfwSwapBuffers(window);

        delta_time = glfwGetTime() - last_time;
        last_time = glfwGetTime();
    }
}
