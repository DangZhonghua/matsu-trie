matsu-trie
===========

A space-efficient trie data structure based on LOUDS.

LOUDS (Level Order Unary Degree Sequence) is succinct unlabeled static tree. Here, "succinct" means that its size is smallest in the sense of information theory. LOUDS is unlabeled tree, but can be extended to labeled tree easily. It is static tree so you can not add or remove nodes from the tree.

> G. Jacobson. Space-efficient static trees and graphs. Foundations of Computer Science, 1989.

Trie is labeled tree which is used to store and search dictionary. Trie is abstract data structure thus there are many aproaches to implement the functionality. Its nodes are labled with character so that we can search words efficiently and flexibly. It is used for applications in natural language processing such as spelling correction or auto-completion.

LOUDS can be used to implement Trie by 1) associating with character labels and 2) storing terminal flags to identify the end of words in the tree. In my experiment, matsu-trie is 7 times smaller than pointer-based implementation of trie, and 50% slower than the baseline.

How does it work?
-----------
<iframe src="http://www.slideshare.net/nokuno/slideshelf" width="760px" height="570px" frameborder="0" marginwidth="0" marginheight="0" scrolling="no" style="border:none;" allowfullscreen webkitallowfullscreen mozallowfullscreen></iframe>

Performance
-----------

* Data source: /usr/share/dict/words in Ubuntu Linux
* Data size: 99,171 words
* Query: shuffled words * 100

| Library       | Data structure | Size (byte) | Time (second) |
| ------------- | -------------- | ----------- | ------------- |
| text          | N/A            | 917K        | N/A           |
| hinoki-trie   | Pointer-based  | 2.9M        | 10            |
| matsu-trie    | LOUDS          | 390K        | 15            |
| marisa-trie   | MARISA         | 255K        | 6             |

Install
-----------

    ./waf configure
    ./waf build --check
    sudo ./waf install

Usage Example
-----------

    % wc words
    99171  99171 938848 words

    % matsu-build words words.matsu 
    labels.size(): 227979
    louds.getSize(): 113996
    terminal.getSize(): 57004
    Total size: 398979

    % matsu-lookup words.matsu
    beautiful
    56028
    thisisnotword
    0

    % matsu-traverse words.matsu | head
    A
    A's
    AA's
    AB's
    ABM's
    AC's
    ACTH's
    AI's
    AIDS's
    AM's

What is Matsu?
-----------

Matsu (松) means pine tree in Japanese language.

![Matsu](http://upload.wikimedia.org/wikipedia/commons/2/24/Matsu01.jpg)
