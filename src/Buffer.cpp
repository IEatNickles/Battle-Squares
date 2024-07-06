#include <glad/glad.h>

#include "Buffer.hpp"

Buffer::Buffer(uint32_t target, void* data, uint64_t size) : m_BufferTarget(target) {
    glGenBuffers(1, &m_BufferID);
    glBufferData(m_BufferTarget, size, data, GL_STATIC_DRAW);
}

void Buffer::bind() const {
    glBindBuffer(m_BufferTarget, m_BufferID);
}
