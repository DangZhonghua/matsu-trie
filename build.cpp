#include "loudsTrie.hpp"

int main(int argc, char **argv) {
    if (argc < 3 || string(argv[1]) == "-h") {
        cerr << "Usage: matsu-build input output\n";
        return 0;
    }

    LoudsTrie trie;
    trie.read(argv[1]);
    trie.save(argv[2]);

    cout << "labels.size(): " << trie.labels.size() << endl;
    cout << "louds.getSize(): " << trie.louds.getSize() << endl;
    cout << "terminal.getSize(): " << trie.terminal.getSize() << endl;
    cout << "Total size: " << trie.size() << endl;
    return 0;
}
