#pragma once

#include "math/Vec2.hpp"
#include "math/Vec4.hpp"
#include "math/Mat4.hpp"

#include <string>
#include <cstdint>

#define ASSERT(expr, fmt, args...) if (!(expr)) {\
    printf("Assertion Failed: " fmt, args);\
    exit(EXIT_FAILURE);\
}

class Shader {
    public:
        Shader(const std::string& vertex_shader, const std::string& fragment_shader);

        void setInt(const std::string& name, int value);
        void setFloat(const std::string& name, float value);
        void setVec2(const std::string& name, Vec2 value);
        void setVec4(const std::string& name, Vec4 value);
        void setMat4(const std::string& name, Mat4 value);

        int getUniformLocation(const std::string& name) const;

        void bind() const;

        uint32_t getID() const {return m_ProgramID;}

    private:
        int getUniformLocation(const std::string& name);

    private:
        uint32_t m_ProgramID = 0;
};
