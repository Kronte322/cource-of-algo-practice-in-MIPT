// 73843831
#include <cmath>
#include <iomanip>
#include <iostream>

const double kScalingMultiplier = 10000.0;
const int64_t kPrecision = 10;

void CalculatePrefixMultiplications(double* prefixes, const double* array,
                                    const size_t kSize) {
  prefixes[0] = powl(array[0], 1.0 / (kScalingMultiplier));
  for (size_t j = 1; j < kSize; ++j) {
    prefixes[j] = powl(array[j], 1.0 / (kScalingMultiplier)) * prefixes[j - 1];
  }
}

double CalculateAverageValue(const int& start, const int& end, double* array,
                             double* prefixes) {
  if (start == end) {
    return array[start];
  }
  if (start == 0) {
    return powl(prefixes[end], kScalingMultiplier / (end - start + 1));
  }
  return powl(prefixes[end] / prefixes[start - 1],
              kScalingMultiplier / (end - start + 1));
}

int main() {
  std::cout << std::fixed << std::setprecision(kPrecision);
  size_t size = 0;
  std::cin >> size;
  double* array = new double[size]{};
  int start = 0;
  int end = 0;
  for (size_t i = 0; i < size; ++i) {
    std::cin >> array[i];
  }
  int number_of_requests = 0;
  std::cin >> number_of_requests;
  double* prefixes = new double[size]{};
  CalculatePrefixMultiplications(prefixes, array, size);
  double res = 0;
  for (int i = 0; i < number_of_requests; ++i) {
    std::cin >> start >> end;
    res = CalculateAverageValue(start, end, array, prefixes);
    std::cout << res << '\n';
  }
  delete[] array;
  delete[] prefixes;
}