// 76094379
#include <iostream>

const int* SearchCyclicalShift(const int* begin, const int* end) {
  while (end - begin != 1) {
    const int* middle = begin + (end - begin) / 2;
    if (*begin <= *middle) {
      begin = middle;
    } else {
      end = middle;
    }
  }
  return end;
}

int main() {
  size_t size_of_array = 0;
  std::cin >> size_of_array;
  int* array = new int[size_of_array];
  for (size_t i = 0; i < size_of_array; ++i) {
    std::cin >> array[i];
  }
  if (array[0] < array[size_of_array - 1]) {
    std::cout << 0;
  } else {
    const int* result = 0;
    result = SearchCyclicalShift(array, array + size_of_array - 1);
    std::cout << (result - array) % size_of_array;
  }
  delete[] array;
}