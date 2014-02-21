#include <gtest/gtest.h>
#include <cstdlib>
#include "loudsTrie.hpp"

class LoudsTrieTest : public ::testing::Test {
 protected:
    LoudsTrie trie;

    void SetUp() {
        vector<const char*> words = {"aaa","aab","ab","ba", "bb"};
        trie.build(words);
    }
};

TEST_F(LoudsTrieTest, Build) {
    EXPECT_EQ("abababab", trie.labels);
}

TEST_F(LoudsTrieTest, Lookup) {
    EXPECT_EQ(0, trie.lookup("a"));
    EXPECT_EQ(0, trie.lookup("abb"));
    EXPECT_EQ(0, trie.lookup("aaaa"));
    EXPECT_EQ(1, trie.lookup("ab"));
    EXPECT_EQ(2, trie.lookup("ba"));
    EXPECT_EQ(3, trie.lookup("bb"));
    EXPECT_EQ(4, trie.lookup("aaa"));
    EXPECT_EQ(5, trie.lookup("aab"));
}

TEST_F(LoudsTrieTest, SaveLoad) {
    trie.save("temp.trie");
    LoudsTrie actual;
    actual.load("temp.trie");
    EXPECT_EQ(trie, actual);
}
