#include <gtest/gtest.h>

#include "padding.h"

TEST(TestPadding, paddingNumbersComputedCorrectly)
{
    ASSERT_EQ(padding::getPaddingBytes(1, 12), 0);
    ASSERT_EQ(padding::getPaddingBytes(16, 12), 4);
    ASSERT_EQ(padding::getPaddingBytes(4, 2), 2);
    ASSERT_EQ(padding::getPaddingBytes(8, 3), 5);
    ASSERT_EQ(padding::getPaddingBytes(32, 19), 13);
    ASSERT_EQ(padding::getPaddingBytes(64, 32), 32);
    ASSERT_EQ(padding::getPaddingBytes(32, 64), 0);
}