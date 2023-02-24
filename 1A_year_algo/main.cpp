// 73817313
#include <iostream>

bool BinarySearch(const int* begin, const int* end, int target) {
  const int* mid = (begin + (end - begin) / 2);
  if (target == *mid) {
    return true;
  }
  if (end - begin == 1) {
    return false;
  }
  if (target < *mid) {
    return BinarySearch(begin, mid, target);
  }
  return BinarySearch(mid, end, target);
}

int main() {
  size_t size_of_array = 0;
  std::cin >> size_of_array;
  int* a = new int[size_of_array];
  for (size_t i = 0; i < size_of_array; ++i) {
    std::cin >> a[i];
  }
  int num_of_requests = 0;
  std::cin >> num_of_requests;
  int begin = 0;
  int end = 0;
  int target = 0;
  for (int i = 0; i < num_of_requests; ++i) {
    std::cin >> begin >> end >> target;
    BinarySearch(a + begin, a + end, target) ? std::cout << "YES\n"
                                             : std::cout << "NO\n";
  }
  delete[] a;
}