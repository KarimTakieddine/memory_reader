#pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <span>

template<typename MemoryType>
class MemoryView
{
    static_assert(
        std::is_same_v<MemoryType, std::byte> || std::is_same_v<MemoryType, const std::byte>,
        "MemoryView template type must be std::byte or const std::byte");

public:
    template<typename T>
    using ObjectType = std::conditional_t<std::is_const_v<MemoryType>, const T, T>;

    explicit MemoryView(std::span<MemoryType> bufferSpan) : m_bufferSpan(bufferSpan) { }

    template<typename T>
    [[nodiscard]] inline std::span<ObjectType<T>, 1> read_object(size_t offset) const noexcept
    {
        assert(offset + sizeof(T) <= m_bufferSpan.size());
        assert(reinterpret_cast<uintptr_t>(m_bufferSpan.data() + offset) % alignof(T) == 0);

        auto* rawBytes = m_bufferSpan.data() + offset;

        return std::span<ObjectType<T>, 1>(reinterpret_cast<ObjectType<T>*>(rawBytes), 1);
    }

    template<typename T>
    [[nodiscard]] inline std::span<ObjectType<T>> read_contiguous_array(size_t offset) const noexcept
    {
        assert(offset + sizeof(size_t) <= m_bufferSpan.size());

        auto* rawBytes = m_bufferSpan.data() + offset;

        size_t count{ 0 };
        std::memcpy(&count, rawBytes, sizeof(size_t));

        auto* arrayBytes = rawBytes + sizeof(size_t);
        assert(offset + sizeof(size_t) + (count * sizeof(T)) <= m_bufferSpan.size());
        assert(reinterpret_cast<uintptr_t>(arrayBytes) % alignof(T) == 0);

        return std::span<ObjectType<T>>(reinterpret_cast<ObjectType<T>*>(rawBytes + sizeof(size_t)), count);
    }

private:
    std::span<MemoryType> m_bufferSpan;
};

using MutableMemoryView = MemoryView<std::byte>;
using ConstMemoryView   = MemoryView<const std::byte>;
