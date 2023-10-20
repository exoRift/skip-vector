#define TIME_POINT std::chrono::steady_clock::time_point
#define NOW std::chrono::steady_clock::now()
#define CAST_NS std::chrono::duration_cast<std::chrono::nanoseconds>
#include <stdlib.h>
#include <vector>
#include <chrono>
#include <algorithm>
#include "../lib/SkipVector.h"

template <class T, class R>
void perform_operation (T* vec, R& cursor, unsigned char op, size_t value) {
  switch (op) {
    case 0: // move
      // cursor += value;
      cursor = cursor + value;

      break;
    case 1: // insert
      vec->insert(cursor, vec->size());

      break;
    case 2: // erase
      // vec->erase(cursor, cursor + value);
      cursor = vec->erase(cursor);

      break;
    case 3: // access
      vec->at(value);

      break;
  }
}

int main (int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(stderr, "Please provide data count and operation count\n");

    return 1;
  }

  const size_t data_size = atoi(argv[1]);
  const size_t operation_count = atoi(argv[2]);

  printf("Data size: %lu\nOperation count: %lu\n\n", data_size, operation_count);

  // Seed data vectors
  std::vector<size_t> vanilla;
  SkipVector<size_t> skip;
  vanilla.reserve(data_size);
  skip.reserve(data_size);

  TIME_POINT data_seed_begin = NOW;
  for (size_t i = 0; i < data_size; ++i) {
    const int value = rand() % 100;

    vanilla.push_back(value);
    skip.push_back(value);
  }
  TIME_POINT data_seed_end = NOW;
  printf("Seeding data took %luns\n", CAST_NS(data_seed_end - data_seed_begin).count());

  size_t seedingPhantomSize = 0;
  size_t seedingPhantomPosition = 0;
  std::vector<unsigned char> operations;
  std::vector<size_t> values;
  operations.reserve(operation_count);
  values.reserve(operation_count);

  // Seed operations
  TIME_POINT operation_seed_begin = NOW;
  for (size_t i = 0; i < operation_count; ++i) {
    const unsigned char op = seedingPhantomSize > 1 ? rand() % 4 : 1;

    operations.push_back(op);

    switch (op) {
      case 1: ++seedingPhantomSize; break; // insert
      case 2: --seedingPhantomSize; break; // erase
    }

    if (op) { // erase
      const size_t amount = rand() % 5;
      seedingPhantomSize -= amount;

      values.push_back(std::min(amount, seedingPhantomSize - seedingPhantomPosition));
    } else {
      // some relative movement amount
      const size_t amount = rand() % (seedingPhantomSize - 1) - seedingPhantomPosition;
      seedingPhantomPosition += amount;

      values.push_back(amount);
    }
  }
  TIME_POINT operation_seed_end = NOW;
  printf("Seeding operations took %luns\n", CAST_NS(operation_seed_end - operation_seed_begin).count());

  // Perform operations
  // Vanilla
  std::vector<unsigned char>::const_iterator operation = operations.cbegin();
  std::vector<size_t>::const_iterator value = values.cbegin();
  std::vector<size_t>::iterator vanillaCursor = vanilla.begin();
  TIME_POINT execution_vanilla_begin = NOW;
  for (; operation != operations.end() && value != values.end(); ++operation, ++value)
    perform_operation(&vanilla, vanillaCursor, *operation, *value);
  TIME_POINT execution_vanilla_end = NOW;

  printf("Executing operations for vanilla vector took %luns\n", CAST_NS(execution_vanilla_begin - execution_vanilla_end).count());

  // Skip
  operation = operations.cbegin();
  value = values.cbegin();
  SkipVector<size_t>::iterator skipCursor = skip.begin();
  TIME_POINT execution_skip_begin = NOW;
  for (; operation != operations.end() && value != values.end(); ++operation, ++value)
    perform_operation(&skip, skipCursor, *operation, *value);
  TIME_POINT execution_skip_end = NOW;

  printf("Executing operations for skip vector took %luns\n", CAST_NS(execution_skip_begin - execution_skip_end).count());

  // Sanity Check

  // Data Integrity Check

  return 0;
}
