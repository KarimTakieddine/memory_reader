#include <gtest/gtest.h>

#include "memory_cursor.hpp"

TEST(TestMemoryCursor, stepIncludesPaddingBytes)
{
    MemoryCursor<16> memoryCursor;
    memoryCursor.step<uint64_t>();
    ASSERT_EQ(memoryCursor.getOffset(), 16);
    memoryCursor.step<uint8_t>();
    ASSERT_EQ(memoryCursor.getOffset(), 32);
    memoryCursor.step<uint16_t>();
    ASSERT_EQ(memoryCursor.getOffset(), 48);
    memoryCursor.step<uint32_t>();
    ASSERT_EQ(memoryCursor.getOffset(), 64);
    memoryCursor.step<uint64_t>();
    ASSERT_EQ(memoryCursor.getOffset(), 80);
    memoryCursor.reset();
    ASSERT_EQ(memoryCursor.getOffset(), 0);
}

TEST(TestMemoryCursor, stepArrayIncludesPaddingBytes)
{
    MemoryCursor<16> memoryCursor;
    memoryCursor.step_array<uint64_t>(1);
    ASSERT_EQ(memoryCursor.getOffset(), 32);
    memoryCursor.step_array<uint8_t>(3);
    ASSERT_EQ(memoryCursor.getOffset(), 64);
    memoryCursor.step_array<uint32_t>(5);
    ASSERT_EQ(memoryCursor.getOffset(), 112);
    memoryCursor.step_array<uint16_t>(1);
    ASSERT_EQ(memoryCursor.getOffset(), 144);
    memoryCursor.reset();
    ASSERT_EQ(memoryCursor.getOffset(), 0);
}