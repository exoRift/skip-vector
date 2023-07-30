# skip-vector
A C++ data structure aimed at increasing the performance of a vector when many deletions are expected

Currently, stdlib vectors are `O(1)` access and `O(n)` deletion.

What if, instead, access and deletion were `O(log(d))` where `d` is the number of deletions, amortized to `O(1)` on account of vector resizing?

## Specifications
### Store 2 dynamically resizable arrays
- One containing the actual data (which we will call the data vector)
- One containing pairs where the key is an index in the data vector and the value is a right shift offset, ordered by key (which we will call the offset vector)
> These arrays will be resized like vectors, doubled every capacity reach.

### Upon accessing the vector at index `i`,
- Begin your offset (`o`) at 0
- Iterate through the skip vector of `<x, k>` until `x > i + o`, adding each `k` to `o` along the way.
- Finally, access the element at `i + o`.

### Upon deleting an element at index `i (+ o)`,
- Insert a pair of `<i + o, 1>` into the offset vector (resizing said vector if necessary by the standard doubling of size).
> - If there exists a pair of `<i + o - k, k>`, alter that pair to be `<i + o - k, k + 1>`
> - If there exists a pair of `<i + o + n, 0>`, where `n` is positive or negative, and there is no other pair with an index between `i + o` and `i + o + n`, alter that pair to be `<i + o, 1>` instead
- *Upon resizing the offset vector, prune out any pairs with a `k` of `0`*

This implicitly deletes an index without any operations by marking it to be skipped.

### Upon inserting an element into the vector at index `i (+ o)`
> If there exists a pair of `<i + o - n, k>` where `k >= n` and there is no other pair with an index between `i + o` and `i + o - n`,
> - If `n` == `k`, alter that pair to be `<i + o - n, k - 1>`
> - If `n` > `k`, alter that pair to be `<i + o - n, n>` and insert `<i + o + 1, k - n>`
> - Finally, overwrite the element at `i + o` with the value to insert

### Upon resizing the data vector
- During the data copy, remove all indices marked as deleted and purge the skip vector.
