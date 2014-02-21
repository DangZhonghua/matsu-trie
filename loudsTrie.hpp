#ifndef LOUDS_HPP
#define LOUDS_HPP

#include <queue>
#include <fstream>
#include <cstring>
#include "bitVector.hpp"

/*
 * LoudsTrie is succinct trie data structure.
 * It extends raw LOUDS to labeled tree with terminal ID.
 */
const uint32_t NOTFOUND = -1;

struct LoudsTrie {
    string labels;
    BitVector louds;
    BitVector terminal;

    uint32_t size() {
        return labels.size() + louds.getSize() + terminal.getSize();
    }

    // find node labeled with c
    uint32_t linearSearch(uint32_t index, char c) {
        uint32_t id = louds.rank1(index+1);
        index = louds.select0(id);    // goto first child
        uint32_t label_index = louds.rank1(index+1) - 2;

        while (louds.get(index) != 0) {
            char label = labels[label_index];

            if (label == c) {
                return index;
            }
            index = index + 1;    // goto next sibling
            label_index  = label_index + 1;
        }
        return NOTFOUND;
    }

    // exact match query
    uint32_t lookup(string query) {
        uint32_t index = 0;

        for (auto c : query ) {
            index = linearSearch(index, c);
            if (index == NOTFOUND)
                return 0;
        }

        uint32_t id = louds.rank1(index+1);
        if (terminal.get(id-1))
            return terminal.rank1(id);

        return 0;
    }

    // traverse vocabulary
    void traverse(uint32_t index = 2, string str = "") {
        uint32_t id = louds.rank1(index+1);
        char label = labels.at(id-2);

        // print terminal
        if (terminal.get(id-1) != 0)
            cout << str + label << endl;

        // traverse first child
        uint32_t firstChild = louds.select0(id);
        if (firstChild < louds.size && louds.get(firstChild) != 0) {
            traverse(firstChild, str + label);
        }

        // traverse next sibling
        if (index+1 < louds.size && louds.get(index+1) != 0) {
            traverse(index+1, str);
        }
    }

    // build bit vectors from word list
    void build(vector<const char*> &words) {
        queue<const char*> que;
        for (auto word : words) {
            que.push(word);
        }

        // super root
        louds.push_back(1);
        louds.push_back(0);
        terminal.push_back(0);

        char prev = '\0';
        while (!que.empty()) {
            const char *p = que.front(); que.pop();

            if (p != NULL) {
                // internal node case
                if (*p != prev) {
                    labels.push_back(*p);
                    louds.push_back(1);
                    prev = *p;
                    que.push(NULL);     // add external node

                    // set terminal flag
                    if (*(p+1) != '\0') {
                        terminal.push_back(0);
                    } else {
                        terminal.push_back(1);
                    }
                }
                if (*(p+1) != '\0') {
                    que.push(p + 1);    // add internal node
                }
            } else {
                // external node case
                louds.push_back(0);
                prev = '\0';
            }
        }

        louds.build();
        terminal.build();
    }

    // build from text file
    void read(const string &filename) {
        ifstream ifs(filename);

        // get size of file
        ifs.seekg (0, ifs.end);
        int length = ifs.tellg();
        ifs.seekg (0, ifs.beg);

        // read whole file
        char *buffer = new char[length];
        ifs.read (buffer,length);
        ifs.close();

        // split line by line
        vector<const char*> words;
        char *current = buffer;
        words.push_back(current);
        while (char *p = strchr(current, '\n')) {
            *p = '\0';
            current = p + 1;
            words.push_back(current);
        }

        // build trie
        build(words);

        delete[] buffer;
    }

    void save(const string &filename) {
        ofstream ofs(filename, ios::binary);
        louds.save(ofs);
        terminal.save(ofs);
        ofs.write(labels.data(), labels.size());
        ofs.close();
    }

    void load(const string &filename) {
        labels.clear();
        louds.clear();
        terminal.clear();

        ifstream ifs(filename, ios::binary);

        louds.load(ifs);
        terminal.load(ifs);

        labels.resize(louds.size/2 - 1);
        ifs.read((char*)labels.data(), louds.size/2 - 1);

        ifs.close();
    }

    bool operator==(const LoudsTrie &trie) const {
        if (labels.size() != trie.labels.size()) {
            return false;
        }

        for (size_t i = 0; i < labels.size(); ++i) {
            if (labels.at(i) != trie.labels.at(i)) {
                return false;
            }
        }

        if (louds != trie.louds)
            return false;

        if (terminal != trie.terminal)
            return false;

        return true;
    }
};

// formatting for printf debug
inline ostream& operator<<(ostream& out, LoudsTrie& trie) {
    out << "labels: " << trie.labels << endl;
    out << "louds: \n" << trie.louds << endl;
    out << "terminal: \n" << trie.terminal << endl;

    return out;
}

#endif
