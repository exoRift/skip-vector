#include <stdlib.h>
#include <vector>
#include "../lib/SkipVector.h"

int main (int argc, char* argv[]) {
  if (argc < 3) {
    printf("Please provide data count and operation count\n");

    return 1;
  }

  const size_t data_size = atoi(argv[1]);
  const size_t operation_count = atoi(argv[2]);

  printf("Data size: %lu\nOperation count: %lu", data_size, operation_count);

  // Seed data vectors
  std::vector<int> vanilla;
  SkipVector<int> skip;
  vanilla.reserve(data_size);
  skip.reserve(data_size);

  for (size_t i = 0; i < data_size; ++i) {
    const int value = rand() % 100;
    vanilla.push_back(value);
    skip.push_back(value);
  }

  // for ()

  return 0;
}
