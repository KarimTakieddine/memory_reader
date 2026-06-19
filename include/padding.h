#pragma once

#include <bit>
#include <cassert>
#include <cstdint>

namespace padding
{
    inline uint64_t getPaddingBytes(uint64_t alignment, uint64_t offset)
    {
        assert(std::has_single_bit(alignment));
        return ( alignment - ( offset & ( alignment - 1 ) ) ) & ( alignment - 1 );
    }
}
