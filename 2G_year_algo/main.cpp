// 76633306
#include <iostream>
#include <vector>

const int64_t kSizeOfSubArray = 5;

struct AdditionArrays {
  std::vector<int64_t> hold_left_nums;
  std::vector<int64_t> hold_right_nums;
  std::vector<int64_t> hold_equal_nums;
};

void SortBubble(std::vector<int64_t>& vect, int64_t start, int64_t end) {
  for (int64_t i = 0; i < end - start; ++i) {
    for (int64_t j = start; j < end - i; ++j) {
      if (vect[j] > vect[j + 1] && j + 1 <= end) {
        std::swap(vect[j], vect[j + 1]);
      }
    }
  }
}

void Partition(std::vector<int64_t>& vect, std::vector<int64_t>& hold_left_nums,
               std::vector<int64_t>& hold_right_nums,
               std::vector<int64_t>& hold_equal_nums, int64_t start) {
  int64_t place_in_vector = start;
  for (size_t j = 0; j < hold_left_nums.size(); ++j) {
    vect[place_in_vector] = hold_left_nums[j];
    ++place_in_vector;
  }
  for (size_t j = 0; j < hold_equal_nums.size(); ++j) {
    vect[place_in_vector] = hold_equal_nums[j];
    ++place_in_vector;
  }
  for (size_t j = 0; j < hold_right_nums.size(); ++j) {
    vect[place_in_vector] = hold_right_nums[j];
    ++place_in_vector;
  }
}

int64_t MedianMedian(std::vector<int64_t>& vect, int64_t start, int64_t end) {
  int counter_for_vector = 0;
  if (end - start <= 4) {
    SortBubble(vect, start, end);
    return vect[(end - start) / 2 + start];
  }

  int num_of_subarrays =
      (end - start) / kSizeOfSubArray +
      ((end - start) % kSizeOfSubArray) / ((end - start) % kSizeOfSubArray);

  for (int64_t j = 0; j < num_of_subarrays; ++j) {
    int64_t right_board = 0;
    int64_t interm_size = 0;
    if (start + j * kSizeOfSubArray + 4 <= end) {
      right_board = start + j * kSizeOfSubArray + 4;
      interm_size = kSizeOfSubArray;
    } else {
      right_board = end;
      interm_size = end - start - j * kSizeOfSubArray;
    }
    SortBubble(vect, start + j * kSizeOfSubArray, right_board);

    int hold = 0;
    if (interm_size < kSizeOfSubArray) {
      hold = vect[start + j * kSizeOfSubArray +
                  (end - (start + j * kSizeOfSubArray)) / 2];
      vect[counter_for_vector] = hold;
      ++counter_for_vector;
    } else {
      hold = vect[start + j * kSizeOfSubArray + 2];
      vect[counter_for_vector] = hold;
      ++counter_for_vector;
    }
  }
  return MedianMedian(vect, 0, counter_for_vector - 1);
}

int64_t IntermediateSort(std::vector<int64_t>& vect, int64_t start, int64_t end,
                         AdditionArrays& arrays) {
  int64_t counter_min = start;
  int64_t counter_middle = 0;
  std::vector<int64_t> vect_copy;
  vect_copy.reserve(end);
  for (int i = start; i <= end; ++i) {
    vect_copy.push_back(vect[i]);
  }
  int64_t hold = MedianMedian(vect_copy, 0, end - start);

  arrays.hold_left_nums.reserve(end - start + 2);
  arrays.hold_right_nums.reserve(end - start + 2);
  arrays.hold_equal_nums.reserve(end - start + 2);
  for (int64_t i = start; i < end + 1; ++i) {
    if (vect[i] == hold) {
      arrays.hold_equal_nums.push_back(hold);
      ++counter_middle;
    } else if (vect[i] < hold) {
      arrays.hold_left_nums.push_back(vect[i]);
      ++counter_min;
    } else if (vect[i] > hold) {
      arrays.hold_right_nums.push_back(vect[i]);
    }
  }
  Partition(vect, arrays.hold_left_nums, arrays.hold_right_nums,
            arrays.hold_equal_nums, start);

  arrays.hold_left_nums.clear();
  arrays.hold_equal_nums.clear();
  arrays.hold_right_nums.clear();
  return counter_min + (counter_middle - 1) / 2;
}

void RecursiveSortFunction(std::vector<int64_t>& vect, int64_t start,
                           int64_t end, AdditionArrays& arrays) {
  if (end - start == 0 || start >= end) {
    return;
  }

  int64_t point = IntermediateSort(vect, start, end, arrays);

  RecursiveSortFunction(vect, start, point - 1, arrays);
  RecursiveSortFunction(vect, point + 1, end, arrays);
}

void QuickSort(std::vector<int64_t>& vect, int64_t start, int64_t end) {
  AdditionArrays arrays;
  std::vector<int64_t> hold_left_nums;
  std::vector<int64_t> hold_right_nums;
  std::vector<int64_t> hold_equal_nums;
  arrays.hold_left_nums = hold_left_nums;
  arrays.hold_right_nums = hold_right_nums;
  arrays.hold_equal_nums = hold_equal_nums;

  RecursiveSortFunction(vect, start, end, arrays);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int64_t num = 0;
  std::cin >> num;
  std::vector<int64_t> vect = {};
  for (int64_t i = 0; i < num; ++i) {
    int64_t interm = 0;
    std::cin >> interm;
    vect.push_back(interm);
  }
  QuickSort(vect, 0, num - 1);
  for (int64_t i = 0; i < num; ++i) {
    std::cout << vect[i] << '\n';
  }
  return 0;
}
