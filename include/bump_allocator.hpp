#pragma once

#include <cstdlib>
#include <new>
#include <cstdint>
#include <cassert>
#include <cstring>
#include <bit>
#include <utility>

#include "padding.h"

#define TO_BYTE_PTR(p) reinterpret_cast<unsigned char*>(p)

namespace airful_engine
{
    template<size_t A>
    class BumpAllocator
    {
        static_assert(std::has_single_bit(A), "Alignment must be a power of 2");

    public:
        BumpAllocator() = default;

        BumpAllocator(const BumpAllocator&) = delete;
        BumpAllocator& operator=(const BumpAllocator&) = delete;

        BumpAllocator(BumpAllocator&& other) noexcept
            : m_bounds(other.m_bounds), m_position(other.m_position), m_buffer(other.m_buffer)
        {
            other.m_bounds      = nullptr;
            other.m_position    = nullptr;
            other.m_buffer      = nullptr;
        }

        BumpAllocator& operator=(BumpAllocator&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            release();

            m_bounds    = other.m_bounds;
            m_position  = other.m_position;
            m_buffer    = other.m_buffer;

            other.m_bounds      = nullptr;
            other.m_position    = nullptr;
            other.m_buffer      = nullptr;

            return *this;
        }

        void allocate(size_t sizeInBytes)
        {
            release();

            constexpr size_t headerSize = sizeof(void*);
            void* unalignedStart        = std::malloc(sizeInBytes + A + headerSize);
    
            if (!unalignedStart) return;

            const uintptr_t rawStart        = reinterpret_cast<uintptr_t>(unalignedStart) + headerSize;
            const uintptr_t alignedStart    = (rawStart + A - 1) & ~(A - 1);

            auto** pHeader  = reinterpret_cast<void**>(alignedStart) - 1;
            *pHeader        = unalignedStart;

            m_buffer   = reinterpret_cast<void*>(alignedStart);
            m_position = m_buffer;
            m_bounds   = TO_BYTE_PTR(m_buffer) + sizeInBytes;

            std::memset(m_buffer, 0, sizeInBytes);
        }

        template<typename T, typename... Args>
        [[nodiscard]] T* requestMemory(Args&&... args)
        {
            auto* const alignedPosition = TO_BYTE_PTR(m_position);
            
            const size_t sizeInBytes    = sizeof(T);
            const uintptr_t endAddr     = reinterpret_cast<uintptr_t>(alignedPosition + sizeInBytes);

            const uint64_t tailPadding = padding::getPaddingBytes(A, endAddr);

            auto* const nextPosition = alignedPosition + sizeInBytes + tailPadding;

            if (nextPosition > m_bounds)
                return nullptr;

            T* result = new (alignedPosition) T(std::forward<Args>(args)...);

            m_position = nextPosition;

            return result;
        }

        void freeMemory(size_t sizeInBytes)
        {
            auto* unalignedEnd = TO_BYTE_PTR(m_position) - sizeInBytes;
            
            if (unalignedEnd <= m_buffer)
            {
                m_position = m_buffer;
                return;
            }

            const uintptr_t endAddr     = reinterpret_cast<uintptr_t>(unalignedEnd);
            const uint64_t tailPadding  = padding::getPaddingBytes(A, endAddr);

            m_position = unalignedEnd - tailPadding;
        }

        void release()
        {
            if (!m_buffer)
                return;

            void* unalignedStart = *(reinterpret_cast<void**>(m_buffer) - 1);
            std::free(unalignedStart);

            m_buffer   = nullptr;
            m_bounds   = nullptr;
            m_position = nullptr;
        }

        [[nodiscard]] ptrdiff_t getSizeInBytes() const noexcept
        {
            return TO_BYTE_PTR(m_bounds) - TO_BYTE_PTR(m_buffer);
        }

        [[nodiscard]] ptrdiff_t getLiveSizeInBytes() const noexcept
        {
            return TO_BYTE_PTR(m_position) - TO_BYTE_PTR(m_buffer);
        }

        [[nodiscard]] ptrdiff_t getInactiveSizeInBytes() const noexcept
        {
            return TO_BYTE_PTR(m_bounds) - TO_BYTE_PTR(m_position);
        }

        [[nodiscard]] void* peek() const noexcept
        {
            return m_buffer;
        }

        ~BumpAllocator()
        {
            release();
        }

    private:
        void* m_bounds   { nullptr };
        void* m_position { nullptr };
        void* m_buffer   { nullptr };
    };
}
