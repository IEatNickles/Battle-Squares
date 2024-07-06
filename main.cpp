#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>

#include "Shader.hpp"
#include "Buffer.hpp"

#include "math/Mat4.hpp"
#include "math/Transform.hpp"

float lerp(float a, float b, float t) {
    return t * (b - a) + a;
}

int main() {
    if (glfwInit() == GLFW_FALSE) {
        printf("No glfw :'(\n");
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Battle Squares", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == GL_FALSE) {
        printf("No glad :'(\n");
        return 2;
    }

    Shader shader("assets/shaders/default.vert", "assets/shaders/default.frag");
    shader.bind();

    Mat4 ortho = Mat4::createOrtho(-8.0f, 8.0f, -4.5f, 4.5f, 0.0f, 1.0f);

    float vertices[] = {
        -0.5f, -0.5f,
        -0.5f,  0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
    };

    Buffer vbo(GL_ARRAY_BUFFER, vertices, sizeof(vertices));
    vbo.bind();

    uint32_t vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, 0);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    Transform trans = Transform::identity();
    Vec2 velocity = Vec2::zero();

    bool grounded = false;
    int jumps = 3;
    float jumpStart = 0.0f;

    glClearColor(0.2f, 0.1f, 0.9f, 1.0f);

    float delta_time;
    float last_time;
    while (!glfwWindowShouldClose(window)) {
        if (jumps > 0 && glfwGetTime() - jumpStart > 0.2f && glfwGetKey(window, GLFW_KEY_W)) {
            velocity.y = 10.0f;
            jumps -= 1;
            jumpStart = glfwGetTime();
        }

        if (glfwGetKey(window, GLFW_KEY_D)) {
            velocity.x = lerp(velocity.x, 7.0f, delta_time * 10.0f);
        } else if (glfwGetKey(window, GLFW_KEY_A)) {
            velocity.x = lerp(velocity.x, -7.0f, delta_time * 10.0f);
        } else {
            velocity.x = lerp(velocity.x, 0.0f, delta_time * 10.0f);
        }

        if (trans.translation.y > -4.0f) {
            velocity.y -= delta_time * 30.0f;
        } else if (velocity.y < 0.0f) {
            velocity.y = 0.0f;
            jumps = 3;
        }
        trans.translation = trans.translation.add(velocity.mul(delta_time));

        Mat4 model = trans.getMatrix();
        shader.setMat4("u_MVP", (ortho).mul(model));

        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

        glfwPollEvents();
        glfwSwapBuffers(window);

        delta_time = glfwGetTime() - last_time;
        last_time = glfwGetTime();
    }
}
