// 73850335
#include <iostream>
#include <vector>

const uint64_t kShiftNum = 8;
const uint64_t kNumForBitwiseOperation = 255;
const uint64_t kNumOfBoxes = 256;

uint64_t ReturnDigit(uint64_t num, const uint64_t kDigit) {
  return (num >> kShiftNum * kDigit) & kNumForBitwiseOperation;
}

void LsdSort(std::vector<uint64_t>& vect, const uint64_t kCount,
             const uint64_t kMaxDigit) {
  std::vector<std::vector<uint64_t>> boxes = {};
  boxes.resize(kNumOfBoxes);
  for (uint64_t i = 0; i < kNumOfBoxes; ++i) {
    boxes[i].reserve(kCount);
  }
  for (uint64_t i = 0; i < kMaxDigit; ++i) {
    for (uint64_t j = 0; j < kCount; ++j) {
      uint64_t digit_size = 0;
      digit_size = ReturnDigit(vect[j], i);
      boxes[digit_size].push_back(vect[j]);
    }
    vect.clear();
    for (uint64_t i = 0; i <= kNumForBitwiseOperation; ++i) {
      uint64_t j = 0;
      while (!boxes[i].empty()) {
        vect.push_back(boxes[i][j]);
        boxes[i].pop_back();
        ++j;
      }
    }
  }
}

uint64_t ReturnMaxDigit(uint64_t num) {
  uint64_t res = 0;
  while (num != 0) {
    num /= kNumOfBoxes;
    ++res;
  }
  return res;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  size_t size = 0;
  std::cin >> size;
  std::vector<uint64_t> vect;
  vect.reserve(size);
  uint64_t max = 0;
  for (size_t i = 0; i < size; ++i) {
    uint64_t intermediate = 0;
    std::cin >> intermediate;
    if (intermediate > max) {
      max = intermediate;
    }
    vect.push_back(intermediate);
  }
  uint64_t max_digit = ReturnMaxDigit(max);
  LsdSort(vect, size, max_digit);
  for (size_t i = 0; i < size; ++i) {
    std::cout << vect[i] << '\n';
  }
  return 0;
}