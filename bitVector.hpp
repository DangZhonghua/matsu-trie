#ifndef BITVECTOR_HPP
#define BITVECTOR_HPP

#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <cmath>

using namespace std;

/*
 * BitVector is a primitive for succinct data structure.
 * It implements rank and select in the simplest way.
 */
struct BitVector {
    static const uint32_t WORD_SIZE = 32;
    uint32_t size;
    vector<uint32_t> bits;
    vector<uint32_t> blocks;

    BitVector() : size(0) {}

    uint32_t getSize() {
        return (bits.size() + blocks.size()) * sizeof(uint32_t);
    }

    void clear() {
        size = 0;
        bits.clear();
        blocks.clear();
    }

    void save(ostream &os) {
        os.write((const char*)&size, sizeof(uint32_t));
        os.write((const char*)bits.data(), bits.size() * sizeof(uint32_t));
        os.write((const char*)blocks.data(), blocks.size() * sizeof(uint32_t));
    }

    void load(istream &is) {
        is.read((char*)&size, sizeof(uint32_t));

        uint32_t block_size = (uint32_t)ceil((double)size / WORD_SIZE);
        bits.resize(block_size);
        blocks.resize(block_size+1);
        is.read((char*)bits.data(), block_size * sizeof(uint32_t));
        is.read((char*)blocks.data(), (block_size+1) * sizeof(uint32_t)); }

    // add one bit to the end
    void push_back(bool bit) {
        assert(bits.size() == ceil((double)size / WORD_SIZE));

        if (bits.size() * WORD_SIZE == size) {
            bits.push_back(0);
        }
        bits.back() |= bit << size % WORD_SIZE;
        size ++;
    }

    // get the bit at position
    uint32_t get(uint32_t position) {
        assert(position < size);

        uint32_t index = position / WORD_SIZE;
        assert(index < bits.size());

        uint32_t remainder = position % WORD_SIZE;
        return (bits[index] >> remainder) & 1;
    }

    // build rank blocks
    void build() {
        assert(bits.size() == ceil((double)size / WORD_SIZE));

        blocks.resize(bits.size());

        uint32_t count = 0;
        for (uint32_t i = 0; i < size; ++i) {
            if (i % WORD_SIZE == 0)
                blocks[i/WORD_SIZE] = count;
            count += get(i);
        }
        blocks.push_back(count);
    }

    // count 1s by looking up blocks and popcount
    uint32_t rank1(uint32_t position) {
        uint32_t division = position / WORD_SIZE;
        uint32_t absolute = blocks[division];
        uint32_t remainder = position % WORD_SIZE;

        if (remainder == 0)
            return absolute;

        uint32_t shifted = bits[division] << (WORD_SIZE - remainder);
        uint32_t relative = __builtin_popcount(shifted);
        return absolute + relative;
    }

    // count 0s rather than 1s
    uint32_t rank0(uint32_t position) {
        return position - rank1(position);
    }

    // find count-th 1 by binary search on blocks and linear search on bits
    uint32_t select1(uint32_t count) {
        if (count == 0)
            return 0;

        uint32_t index = lower_bound(blocks.begin(), blocks.end(), count) - blocks.begin() - 1;

        uint32_t relative = 0;
        for (uint32_t i = 0; i < WORD_SIZE; ++i) {
            relative += (bits[index] >> i) & 1;
            if (count == blocks[index] + relative) {
                return index * WORD_SIZE + i + 1;
            }
        }
        assert(0);
    }

    // find count-th 0 by binary search on blocks and linear search on bits
    uint32_t select0(uint32_t count) {
        if (count == 0)
            return 0;

        // TODO use STL instead of my own implementation
        uint32_t left = 0;
        uint32_t right = blocks.size();

        while (left < right) {
            uint32_t middle = (left + right) / 2;
            if (middle * WORD_SIZE - blocks[middle] < count)
                left = middle + 1;
            else
                right = middle;
        }
        uint32_t index = left - 1;

        uint32_t relative = 0;
        for (uint32_t i = 0; i < WORD_SIZE; ++i) {
            relative += !((bits[index] >> i) & 1);
            if (count == index * WORD_SIZE - blocks[index] + relative) {
                return index * WORD_SIZE + i + 1;
            }
        }
        assert(0);
    }

    bool operator!=(const BitVector &bitVector) const {
        return !(*this == bitVector);
    }

    bool operator==(const BitVector &bitVector) const {
        if (size != bitVector.size)
            return false;
        if (bits.size() != bitVector.bits.size())
            return false;
        if (blocks.size() != bitVector.blocks.size())
            return false;

        for (size_t i = 0; i < bits.size(); ++i) {
            if (bits.at(i) != bitVector.bits.at(i)) {
                return false;
            }
        }
        for (size_t i = 0; i < blocks.size(); ++i) {
            if (blocks.at(i) != bitVector.blocks.at(i)) {
                return false;
            }
        }
        return true;
    }
};

// formatting for printf debug
inline ostream& operator<<(ostream& out, BitVector& bits) {
    out << "size: " << bits.size << endl;

    for (uint32_t i = 0; i < bits.size; ++i) {
        out << bits.get(i);
        if (i % 8 == 7)
            out << " ";
        if (i % 16 == 15)
            out << endl;
    }
    out << endl;

    out << "block: ";
    for (uint32_t i = 0; i < bits.blocks.size(); ++i) {
        out << bits.blocks[i] << " ";
    }
    out << endl;
    return out;
}

#endif
