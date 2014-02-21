#include <gtest/gtest.h>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include "bitVector.hpp"

const uint32_t TEST_SIZE = 1000;

class BitVectorTest : public ::testing::Test {
 protected:
    void SetUp() {
        srand(0);
        for (uint32_t i = 0; i < TEST_SIZE; ++i) {
            bitVector.push_back(rand() % 2);
        }
        bitVector.build();
    }
    BitVector bitVector;
};

TEST_F(BitVectorTest, Get) {
    srand(0);
    EXPECT_EQ(TEST_SIZE, bitVector.size);
    for (uint32_t i = 0; i < bitVector.size; ++i) {
        EXPECT_EQ(rand() % 2, bitVector.get(i));
    }
}

TEST_F(BitVectorTest, Rank) {
    uint32_t count1 = 0, count0 = 0;
    for (uint32_t i = 0; i < bitVector.size; ++i) {
        EXPECT_EQ(count1, bitVector.rank1(i)) << "When i = " << i;
        count1 += bitVector.get(i);

        EXPECT_EQ(count0, bitVector.rank0(i)) << "When i = " << i;
        count0 += !bitVector.get(i);
    }
}

TEST_F(BitVectorTest, Select) {
    uint32_t count1 = 0, count0 = 0;
    for (uint32_t i = 0; i < bitVector.size; ++i) {
        if (bitVector.get(i)) {
            count1 += 1;
            EXPECT_EQ(i+1, bitVector.select1(count1));
        } else {
            count0 += 1;
            EXPECT_EQ(i+1, bitVector.select0(count0));
        }
    }

    for (uint32_t i = 0; i < bitVector.rank1(bitVector.size); ++i) {
        EXPECT_EQ(i, bitVector.rank1(bitVector.select1(i)));
        EXPECT_EQ(i, bitVector.rank0(bitVector.select0(i)));
    }
}

TEST_F(BitVectorTest, SaveLoad) {
    ofstream ofs("temp.bv", ios::binary);
    bitVector.save(ofs);
    ofs.close();

    ifstream ifs("temp.bv", ios::binary);
    BitVector actual;
    actual.load(ifs);
    ifs.close();

    EXPECT_EQ(bitVector, actual);
}
