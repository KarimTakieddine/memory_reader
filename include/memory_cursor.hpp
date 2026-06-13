#pragma once

#include <cstddef>

template<size_t A>
class MemoryCursor
{
public:
    MemoryCursor() : m_offset(0) { }

    explicit MemoryCursor(size_t offset) : m_offset(offset) { }

    void reset() noexcept
    {
        m_offset = 0;
    }

    template<typename T>
    void step() noexcept
    {
        static_assert(sizeof(T) % A == 0, "step() size in bytes must be a multiple of the MemoryCursor alignment");

        m_offset += sizeof(T);
    }

    template<typename T>
    void step_array(size_t count) noexcept
    {
        const size_t sizeInBytes = count * sizeof(T);

        static_assert(sizeof(T) % A == 0, "step_array() size in bytes must be a multiple of the MemoryCursor alignment");

        m_offset += sizeof(size_t) + sizeInBytes;
    }

    [[nodiscard]] size_t getOffset() const noexcept
    {
        return m_offset;
    }

private:
    size_t m_offset;
};
