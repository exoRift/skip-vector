# skip-vector
A C++ data structure aimed at increasing the performance of a vector when many deletions are expected

Currently, stdlib vectors are `O(1)` access and `O(n)` deletion.

What if, instead, access and deletion were `O(log(d))` where `d` is the number of deletions, amortized to `O(1)` on account of vector resizing?

## Specifications
### Store 2 dynamically resizable vectors.
- One containing the actual data (which we will call the data vector)
- One containing pairs where the key is an index in the data vector and the value is a right shift offset, ordered by key (which we will call the offset vector)

### Upon deleting an element at index X, insert a pair of `<x, 1>` into the offset vector (resizing said vector if necessary by the standard doubling of size).
> If there exists a pair of `<x - 1, k>`, alter that pair to be `<x - 1, k + 1>`

This implicitly deletes an index without any operations by marking it to be skipped.

### Upon accessing the vector at index `i`, begin your offset (`o`) at 0.
Iterate through the skip vector until `x > i`, adding each `k` to `o` along the way.
Finally, access the element at `i + o`.

### Upon resizing the data vector, during the data copy, remove all indices marked as deleted and purge the skip vector.
