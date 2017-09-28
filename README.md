# Roulette tree

Random weighted selection, dynamic: select an element with probability proportional to its weight.

It is an important step in many randomized algorithms. This datastructure provides efficient update and query (O(log n)).

It is inspired by a [pointer-based implementation](https://github.com/hyPiRion/roulette-tree).
Compared to it, it will have much lower memory usage (~3x), and may have better performance (not benchmarked).

It is still a work in progress, and will still be improved over time.

## Implementation

The datastructure is a balanced binary tree with the elements as leaves. Each node stores the total weight of its subtree.
The selection procedure rolls a number between 0 and the sum of the weights, and the corresponding element is obtained by dichotomy.

Like a typical binary heap, the tree is stored in an array and doesn't have to use any pointer: the datastructure uses contiguous memory space for at most 3n weights.
