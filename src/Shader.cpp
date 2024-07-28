#include "Shader.hpp"

#include <fstream>
#include <glad/glad.h>

uint32_t compileShader(const std::string& src, int type) {
    uint32_t shader = glCreateShader(type);
    const char* c_source = src.c_str();
    glShaderSource(shader, 1, &c_source, nullptr);
    glCompileShader(shader);

    int32_t status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE) {
        char buf[1024];
        glGetShaderInfoLog(shader, 1024, nullptr, buf);

        std::string shader_name = "fragment";
        if (type == GL_VERTEX_SHADER) {
            shader_name = "vertex";
        }

        printf("Error in %s shader:\n%s\n", shader_name.c_str(), buf);
        exit(EXIT_FAILURE);
        return 0;
    }

    return shader;
}

Shader::Shader(const std::string& vertex_shader, const std::string& fragment_shader) {
    std::ifstream vertex_stream{};
    vertex_stream.open(vertex_shader);
    std::string line;
    std::string vertex_src;
    while (std::getline(vertex_stream, line)) {
        vertex_src.append(line).append("\n");
    }
    vertex_stream.close();

    std::ifstream fragment_stream{};
    fragment_stream.open(fragment_shader);
    std::string fragment_src;
    while (std::getline(fragment_stream, line)) {
        fragment_src.append(line).append("\n");
    }
    fragment_stream.close();

    uint32_t vs = compileShader(vertex_src, GL_VERTEX_SHADER);
    uint32_t fs = compileShader(fragment_src, GL_FRAGMENT_SHADER);

    m_ProgramID = glCreateProgram();
    glAttachShader(m_ProgramID, vs);
    glAttachShader(m_ProgramID, fs);

    glLinkProgram(m_ProgramID);
    glValidateProgram(m_ProgramID);

    int status = 0;
    glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        char buf[1024];
        glGetProgramInfoLog(m_ProgramID, 1024, nullptr, buf);
        printf("Failed to link program:\n%s\n", buf);
        exit(EXIT_FAILURE);
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}

void Shader::setInt(const std::string& name, int value) {
    glProgramUniform1i(m_ProgramID, getUniformLocation(name), value);
}

void Shader::setFloat(const std::string& name, float value) {
    glProgramUniform1f(m_ProgramID, getUniformLocation(name), value);
}

void Shader::setVec2(const std::string& name, Vec2 value) {
    glProgramUniform2f(m_ProgramID, getUniformLocation(name), value.x, value.y);
}

void Shader::setVec4(const std::string& name, Vec4 value) {
    glProgramUniform4f(m_ProgramID, getUniformLocation(name), value.x, value.y, value.z, value.w);
}

void Shader::setMat4(const std::string& name, Mat4 value) {
    glProgramUniformMatrix4fv(m_ProgramID, getUniformLocation(name), 1, GL_FALSE, value.ptr());
}

int Shader::getUniformLocation(const std::string& name) {
    if (m_UniformLocs.contains(name)) {
        return m_UniformLocs.at(name);
    }
    int location = glGetUniformLocation(m_ProgramID, name.c_str());
    ASSERT(location > -1, "The uniform '%s' does not exist", name.c_str());
    m_UniformLocs.emplace(name, location);
    return location;
}

void Shader::bind() const {
    glUseProgram(m_ProgramID);
}
