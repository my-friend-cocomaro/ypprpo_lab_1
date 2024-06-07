#include <gtest/gtest.h>
#include "../include/BigArray.h"

// Test default constructor
TEST(BitArrayTest, DefaultConstructor) {
    BitArray b;
    EXPECT_TRUE(b.empty());
    EXPECT_EQ(b.size(), 0);
}

// Test constructor with parameters
TEST(BitArrayTest, ParameterizedConstructor) {
    BitArray b(64, 0xF0F0F0F0);
    EXPECT_EQ(b.size(), 64);
    // You can add more checks here to verify the bit pattern if necessary
}

// Test copy constructor
TEST(BitArrayTest, CopyConstructor) {
    BitArray b1(64, 0xF0F0F0F0);
    BitArray b2(b1);
    EXPECT_EQ(b2.size(), 64);
    EXPECT_EQ(b1, b2);
}

// Test assignment operator
TEST(BitArrayTest, AssignmentOperator) {
    BitArray b1(64, 0xF0F0F0F0);
    BitArray b2;
    b2 = b1;
    EXPECT_EQ(b2.size(), 64);
    EXPECT_EQ(b1, b2);
}

// Test resizing the array
TEST(BitArrayTest, Resize) {
    BitArray b(32);
    b.resize(64, true);
    EXPECT_EQ(b.size(), 64);
    EXPECT_TRUE(b.any());
}

// Test clear method
TEST(BitArrayTest, Clear) {
    BitArray b(32, 0xFFFFFFFF);
    b.clear();
    EXPECT_TRUE(b.empty());
    EXPECT_EQ(b.size(), 0);
}

// Test push_back method
TEST(BitArrayTest, PushBack) {
    BitArray b;
    b.push_back(true);
    b.push_back(false);
    EXPECT_EQ(b.size(), 2);
    EXPECT_TRUE(b[0]);
    EXPECT_FALSE(b[1]);
}

// Test bitwise AND operator
TEST(BitArrayTest, BitwiseAndAssignmentOperator) {
    BitArray b1(32, 0xFFFFFFFF);
    BitArray b2(32, 0x0F0F0F0F);
    b1 &= b2;
    EXPECT_EQ(b1, BitArray(32, 0x0F0F0F0F));
}

// Test bitwise OR operator
TEST(BitArrayTest, BitwiseOrAssignmentOperator) {
    BitArray b1(32, 0xFFFFFFFF);
    BitArray b2(32, 0x0F0F0F0F);
    b1 |= b2;
    EXPECT_EQ(b1, BitArray(32, 0xFFFFFFFF));
}

// Test bitwise XOR operator
TEST(BitArrayTest, BitwiseXorAssignmentOperator) {
    BitArray b1(32, 0xFFFFFFFF);
    BitArray b2(32, 0x0F0F0F0F);
    b1 ^= b2;
    EXPECT_EQ(b1, BitArray(32, 0xF0F0F0F0));
}

// Test left shift operator
TEST(BitArrayTest, LeftShiftAssignmentOperator) {
    BitArray b(32, 0x1);
    b <<= 4;
    EXPECT_EQ(b, BitArray(32, 0x10));
}

// Test right shift operator
TEST(BitArrayTest, RightShiftAssignmentOperator) {
    BitArray b(32, 0x10);
    b >>= 4;
    EXPECT_EQ(b, BitArray(32, 0x1));
}

// -----------------------------------------

// Test set method with index and value
TEST(BitArrayTest, SetWithIndexAndValue) {
    BitArray b(8);
    b.set(3, true);
    EXPECT_TRUE(b[3]);
    b.set(3, false);
    EXPECT_FALSE(b[3]);
}

// Test set method without parameters
TEST(BitArrayTest, SetAll) {
    BitArray b(8);
    b.set();
    for (int i = 0; i < 8; ++i) {
        EXPECT_TRUE(b[i]);
    }
}

// Test reset method with index
TEST(BitArrayTest, ResetWithIndex) {
    BitArray b(8);
    b.set(3);
    EXPECT_TRUE(b[3]);
    b.reset(3);
    EXPECT_FALSE(b[3]);
}

// Test reset method without parameters
TEST(BitArrayTest, ResetAll) {
    BitArray b(8, 0xFF); // All bits set to true
    b.reset();
    for (int i = 0; i < 8; ++i) {
        EXPECT_FALSE(b[i]);
    }
}

// Test operator~
TEST(BitArrayTest, BitwiseNotOperator) {
    BitArray b(8, 0xF0); // 11110000
    BitArray notB = ~b;  // 00001111
    EXPECT_EQ(notB.to_string(), "00001111");
}

// Test swap method
TEST(BitArrayTest, Swap) {
    BitArray b1(8, 0xF0); // 11110000
    BitArray b2(8, 0x0F); // 00001111
    b1.swap(b2);
    EXPECT_EQ(b1.to_string(), "00001111");
    EXPECT_EQ(b2.to_string(), "11110000");
}

// Test << operator
TEST(BitArrayTest, LeftShiftOperator) {
    BitArray b(8, 0x01); // 00000001
    BitArray shifted = b << 3;
    EXPECT_EQ(shifted.to_string(), "00001000");
}

// Test >> operator
TEST(BitArrayTest, RightShiftOperator) {
    BitArray b(8, 0x08); // 00001000
    BitArray shifted = b >> 3;
    EXPECT_EQ(shifted.to_string(), "00000001");
}

// Test size method
TEST(BitArrayTest, Size) {
    BitArray b(8);
    EXPECT_EQ(b.size(), 8);
}

// Test empty method
TEST(BitArrayTest, Empty) {
    BitArray b;
    EXPECT_TRUE(b.empty());
    BitArray b2(8);
    EXPECT_FALSE(b2.empty());
}

// Test count method
TEST(BitArrayTest, Count) {
    BitArray b(8, 0xF0); // 11110000
    EXPECT_EQ(b.count(), 4);
}

// Test any method
TEST(BitArrayTest, Any) {
    BitArray b(8, 0xF0); // 11110000
    EXPECT_TRUE(b.any());
    b.reset();
    EXPECT_FALSE(b.any());
}

// Test none method
TEST(BitArrayTest, None) {
    BitArray b(8);
    EXPECT_TRUE(b.none());
    b.set(3);
    EXPECT_FALSE(b.none());
}

// Test to_string method
TEST(BitArrayTest, ToString) {
    BitArray b(8, 0xF0); // 11110000
    EXPECT_EQ(b.to_string(), "11110000");
}

// ------------------------------------------
// ------------------------------------------

// Test equality operator
TEST(BitArrayTest, EqualityOperator) {
    BitArray b1(8, 0xF0); // 11110000
    BitArray b2(8, 0xF0); // 11110000
    EXPECT_TRUE(b1 == b2);
}

// Test inequality operator
TEST(BitArrayTest, InequalityOperator) {
    BitArray b1(8, 0xF0); // 11110000
    BitArray b2(8, 0x0F); // 00001111
    EXPECT_TRUE(b1 != b2);
}

// Test bitwise AND operator
TEST(BitArrayTest, BitwiseAndOperator) {
    BitArray b1(8, 0xF0); // 11110000
    BitArray b2(8, 0x0F); // 00001111
    BitArray result = b1 & b2;
    EXPECT_EQ(result.to_string(), "00000000");
}

// Test bitwise OR operator
TEST(BitArrayTest, BitwiseOrOperator) {
    BitArray b1(8, 0xF0); // 11110000
    BitArray b2(8, 0x0F); // 00001111
    BitArray result = b1 | b2;
    EXPECT_EQ(result.to_string(), "11111111");
}

// Test bitwise XOR operator
TEST(BitArrayTest, BitwiseXorOperator) {
    BitArray b1(8, 0xF0); // 11110000
    BitArray b2(8, 0x0F); // 00001111
    BitArray result = b1 ^ b2;
    EXPECT_EQ(result.to_string(), "11111111");
}
