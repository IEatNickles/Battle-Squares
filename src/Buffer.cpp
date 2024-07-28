#include <glad/glad.h>

#include "Buffer.hpp"

Buffer::Buffer() {
}

Buffer::Buffer(uint32_t target, void* data, int64_t size) : m_BufferTarget(target) {
    glGenBuffers(1, &m_BufferID);
    glBindBuffer(m_BufferTarget, m_BufferID);
    glBufferData(m_BufferTarget, size, data, GL_DYNAMIC_DRAW);
}

void Buffer::setSubData(void* data, int64_t offset, int64_t size) {
    bind();
    glBufferSubData(m_BufferTarget, offset, size, data);
}

void Buffer::bind() const {
    glBindBuffer(m_BufferTarget, m_BufferID);
}

uint32_t Buffer::getID() const {
    return m_BufferID;
}
