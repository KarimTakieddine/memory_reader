#pragma once

#include <cstddef>

#include "padding.h"

template<uint64_t A>
class MemoryCursor
{
    static_assert(std::has_single_bit(A), "Alignment must be a power of 2"); // FYI works for 1

public:
    MemoryCursor() : m_offset(0) { }

    explicit MemoryCursor(uint64_t offset) : m_offset(offset) { }

    void reset() noexcept
    {
        m_offset = 0;
    }

    template<typename T>
    void step() noexcept
    {
        m_offset += padding::getPaddingBytes(A, m_offset);
        m_offset += sizeof(T);
        m_offset += padding::getPaddingBytes(A, m_offset);
    }

    template<typename T>
    void step_array(size_t count) noexcept
    {
        m_offset += sizeof(uint64_t);
        m_offset += padding::getPaddingBytes(A, m_offset);
        m_offset += count * sizeof(T);
        m_offset += padding::getPaddingBytes(A, m_offset);
    }

    [[nodiscard]] uint64_t getOffset() const noexcept
    {
        return m_offset;
    }

private:
    uint64_t m_offset;
};
