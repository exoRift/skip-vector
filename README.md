# skip-vector
A C++ data structure aimed at increasing the performance of a vector when many deletions are expected

Currently, stdlib vectors are `O(1)` access and `O(n)` deletion.

What if, instead, access and deletion were `O(log(d))` where `d` is the number of deletions, amortized to `O(1)` on account of vector resizing?
