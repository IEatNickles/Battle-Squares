#include <cstdint>

class Buffer {
    public:
        Buffer(uint32_t target, void* data, uint64_t size);

        void bind() const;

    private:
        void* m_Data;
        uint64_t m_Size;

        uint32_t m_BufferID;
        uint32_t m_BufferTarget;
};
