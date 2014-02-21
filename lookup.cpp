#include "loudsTrie.hpp"

int main(int argc, char **argv) {
    if (argc < 2 || string(argv[1]) == "-h") {
        cerr << "Usage: matsu-lookup trie\n";
        return 0;
    }

    LoudsTrie trie;
    trie.load(argv[1]);

    string query;
    while (cin >> query) {
        uint32_t result = trie.lookup(query);
        cout << result << endl;
    }
    return 0;
}
