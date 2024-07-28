#pragma once

#include <cstdint>

class Buffer {
    public:
        Buffer();
        Buffer(uint32_t target, void* data, int64_t size);

        void setSubData(void* data, int64_t offset, int64_t size);

        void bind() const;

        uint32_t getID() const;

    private:
        void* m_Data;
        uint64_t m_Size;

        uint32_t m_BufferID;
        uint32_t m_BufferTarget;
};
