#include <type_traits>

#include <gtest/gtest.h>

#include "memory_view.hpp"

namespace
{
    struct TestStruct
    {
        uint8_t value_0{ 0 };
        uint8_t value_1{ 0 };
        uint8_t value_2{ 0 };
        uint8_t value_3{ 0 };
    };
}

TEST(TestMemoryView, singleObjectAddressing)
{
    std::array<std::byte, sizeof(TestStruct)> buffer;

    auto* inputObj      = reinterpret_cast<TestStruct*>(buffer.data());
    inputObj->value_0   = 8;
    inputObj->value_1   = 16;
    inputObj->value_2   = 32;
    inputObj->value_3   = 64;

    std::span<const std::byte, sizeof(TestStruct)> bufferSpan(buffer.data(), 4);

    ConstMemoryView memoryView(bufferSpan);
    auto testSpan = memoryView.read_object<TestStruct>(0);

    ASSERT_EQ(testSpan.size(), 1);
    ASSERT_EQ(testSpan.size_bytes(), sizeof(TestStruct));
    
    const TestStruct* outputObj = testSpan.data();

    ASSERT_EQ(outputObj->value_0, inputObj->value_0);
    ASSERT_EQ(outputObj->value_1, inputObj->value_1);
    ASSERT_EQ(outputObj->value_2, inputObj->value_2);
    ASSERT_EQ(outputObj->value_3, inputObj->value_3);
}

TEST(TestMemoryView, arrayAddressing)
{
    const size_t arraySize{ 4 };

    const size_t sizeInBytes = sizeof(size_t) + arraySize * sizeof(TestStruct);

    std::array<std::byte, sizeInBytes> buffer;

    std::memcpy(buffer.data(), &arraySize, sizeof(size_t));

    auto* inputObj_0    = reinterpret_cast<TestStruct*>(buffer.data() + sizeof(size_t));
    inputObj_0->value_0 = 7;
    inputObj_0->value_1 = 14;
    inputObj_0->value_2 = 28;
    inputObj_0->value_3 = 56;

    auto* inputObj_1    = reinterpret_cast<TestStruct*>(buffer.data() + sizeof(size_t) + sizeof(TestStruct));
    inputObj_1->value_0 = 8;
    inputObj_1->value_1 = 16;
    inputObj_1->value_2 = 32;
    inputObj_1->value_3 = 64;

    auto* inputObj_2    = reinterpret_cast<TestStruct*>(buffer.data() + sizeof(size_t) + 2 * sizeof(TestStruct));
    inputObj_2->value_0 = 1;
    inputObj_2->value_1 = 2;
    inputObj_2->value_2 = 3;
    inputObj_2->value_3 = 4;

    auto* inputObj_3    = reinterpret_cast<TestStruct*>(buffer.data() + sizeof(size_t) + 3 * sizeof(TestStruct));
    inputObj_3->value_0 = 9;
    inputObj_3->value_1 = 10;
    inputObj_3->value_2 = 11;
    inputObj_3->value_3 = 12;

    std::span<const std::byte, sizeInBytes> bufferSpan(buffer.data(), sizeInBytes);

    ConstMemoryView memoryView(bufferSpan);
    auto testSpan = memoryView.read_contiguous_array<TestStruct>(0);

    ASSERT_EQ(testSpan.size(), 4);
    ASSERT_EQ(testSpan.size_bytes(), testSpan.size() * sizeof(TestStruct));

    const TestStruct* outputObj_0 = testSpan.data();

    ASSERT_EQ(outputObj_0->value_0, inputObj_0->value_0);
    ASSERT_EQ(outputObj_0->value_1, inputObj_0->value_1);
    ASSERT_EQ(outputObj_0->value_2, inputObj_0->value_2);
    ASSERT_EQ(outputObj_0->value_3, inputObj_0->value_3);

    const TestStruct* outputObj_1 = testSpan.data() + 1;

    ASSERT_EQ(outputObj_1->value_0, inputObj_1->value_0);
    ASSERT_EQ(outputObj_1->value_1, inputObj_1->value_1);
    ASSERT_EQ(outputObj_1->value_2, inputObj_1->value_2);
    ASSERT_EQ(outputObj_1->value_3, inputObj_1->value_3);

    const TestStruct* outputObj_2 = testSpan.data() + 2;

    ASSERT_EQ(outputObj_2->value_0, inputObj_2->value_0);
    ASSERT_EQ(outputObj_2->value_1, inputObj_2->value_1);
    ASSERT_EQ(outputObj_2->value_2, inputObj_2->value_2);
    ASSERT_EQ(outputObj_2->value_3, inputObj_2->value_3);

    const TestStruct* outputObj_3 = testSpan.data() + 3;

    ASSERT_EQ(outputObj_3->value_0, inputObj_3->value_0);
    ASSERT_EQ(outputObj_3->value_1, inputObj_3->value_1);
    ASSERT_EQ(outputObj_3->value_2, inputObj_3->value_2);
    ASSERT_EQ(outputObj_3->value_3, inputObj_3->value_3);
}
