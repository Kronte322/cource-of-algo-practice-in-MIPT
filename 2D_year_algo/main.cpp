// 76617576
#include <cmath>
#include <iostream>
#include <vector>

const uint64_t kModulo = 1 << 30;

struct Member {
  uint64_t contain;
  uint64_t address;
};

class MinMaxHeap {
 public:
  void InsertItem(Member item);
  uint64_t ShowMin() const { return array_of_nums_in_minheap_[1].contain; }
  void ExtractMin();
  uint64_t ShowMax() const { return array_of_nums_in_maxheap_[1].contain; }
  void ExtractMax();
  void Clear();
  uint64_t Size() const { return size_; };

 private:
  size_t size_ = 0;
  Member foundation_;
  std::vector<Member> array_of_nums_in_minheap_ = {foundation_};
  std::vector<Member> array_of_nums_in_maxheap_ = {foundation_};

  void SiftDownMin(uint64_t index_in_array);
  void SiftUpMin(uint64_t index_in_array);
  void SiftUpMax(uint64_t index_in_array);
  void SiftDownMax(uint64_t index_in_array);
  void HugeSwapMin(uint64_t index_in_array, uint64_t j);
  void HugeSwapMax(uint64_t index_in_array, uint64_t j);
  void DeleteItemMin(uint64_t index_in_array);
  void DeleteItemMax(uint64_t index_in_array);
};

uint64_t Parent(uint64_t index_in_array) { return index_in_array >> 1; }

uint64_t LeftChild(uint64_t index_in_array) { return index_in_array << 1; }

uint64_t RightChild(uint64_t index_in_array) {
  return (index_in_array << 1) + 1;
}

void MinMaxHeap::HugeSwapMin(uint64_t index_in_array, uint64_t j) {
  array_of_nums_in_maxheap_[array_of_nums_in_minheap_[index_in_array].address]
      .address = j;
  array_of_nums_in_maxheap_[array_of_nums_in_minheap_[j].address].address =
      index_in_array;
  std::swap(array_of_nums_in_minheap_[index_in_array],
            array_of_nums_in_minheap_[j]);
}

void MinMaxHeap::HugeSwapMax(uint64_t index_in_array, uint64_t j) {
  array_of_nums_in_minheap_[array_of_nums_in_maxheap_[index_in_array].address]
      .address = j;
  array_of_nums_in_minheap_[array_of_nums_in_maxheap_[j].address].address =
      index_in_array;
  std::swap(array_of_nums_in_maxheap_[index_in_array],
            array_of_nums_in_maxheap_[j]);
}

void MinMaxHeap::Clear() {
  array_of_nums_in_minheap_.clear();
  array_of_nums_in_maxheap_.clear();
  Member foundation;
  foundation.address = 0;
  foundation.contain = 0;
  array_of_nums_in_maxheap_.push_back(foundation);
  array_of_nums_in_minheap_.push_back(foundation);
  size_ = 0;
}

void MinMaxHeap::ExtractMax() {
  HugeSwapMax(1, size_);
  DeleteItemMin(size_);
  array_of_nums_in_maxheap_.pop_back();
  --size_;
  SiftDownMax(1);
}

void MinMaxHeap::SiftUpMax(uint64_t index_in_array) {
  if (index_in_array == 1) {
    return;
  }
  if (array_of_nums_in_maxheap_[index_in_array].contain >
      array_of_nums_in_maxheap_[Parent(index_in_array)].contain) {
    HugeSwapMax(index_in_array, Parent(index_in_array));
  }
  uint64_t test = Parent(index_in_array);
  SiftUpMax(test);
}

void MinMaxHeap::SiftDownMax(uint64_t index_in_array) {
  uint64_t sift_id = index_in_array;
  if (LeftChild(index_in_array) <= size_ &&
      array_of_nums_in_maxheap_[index_in_array].contain <
          array_of_nums_in_maxheap_[LeftChild(index_in_array)].contain) {
    sift_id = LeftChild(index_in_array);
  }
  if (RightChild(index_in_array) <= size_ &&
      array_of_nums_in_maxheap_[sift_id].contain <
          array_of_nums_in_maxheap_[RightChild(index_in_array)].contain) {
    sift_id = RightChild(index_in_array);
  }
  if (sift_id == index_in_array) {
    return;
  }
  HugeSwapMax(index_in_array, sift_id);
  SiftDownMax(sift_id);
}

void MinMaxHeap::DeleteItemMin(uint64_t index_in_array) {
  uint64_t num_in_min_array = array_of_nums_in_maxheap_[index_in_array].address;
  HugeSwapMin(num_in_min_array, size_);
  array_of_nums_in_minheap_.pop_back();
  SiftUpMin(num_in_min_array);
}

void MinMaxHeap::DeleteItemMax(uint64_t index_in_array) {
  uint64_t num_in_max_array = array_of_nums_in_minheap_[index_in_array].address;
  HugeSwapMax(num_in_max_array, size_);
  array_of_nums_in_maxheap_.pop_back();
  SiftUpMax(num_in_max_array);
}

void MinMaxHeap::InsertItem(Member item) {
  ++size_;
  array_of_nums_in_minheap_.push_back(item);
  array_of_nums_in_maxheap_.push_back(item);
  array_of_nums_in_minheap_[size_].address = size_;
  array_of_nums_in_maxheap_[size_].address = size_;
  SiftUpMin(size_);
  SiftUpMax(size_);
}

void MinMaxHeap::SiftUpMin(uint64_t index_in_array) {
  if (index_in_array == 1) {
    return;
  }
  if (array_of_nums_in_minheap_[index_in_array].contain <
      array_of_nums_in_minheap_[Parent(index_in_array)].contain) {
    HugeSwapMin(index_in_array, Parent(index_in_array));
  }
  uint64_t test = Parent(index_in_array);
  SiftUpMin(test);
}

void MinMaxHeap::SiftDownMin(uint64_t index_in_array) {
  uint64_t sift_id = index_in_array;
  if (LeftChild(index_in_array) <= size_ &&
      array_of_nums_in_minheap_[index_in_array].contain >
          array_of_nums_in_minheap_[LeftChild(index_in_array)].contain) {
    sift_id = LeftChild(index_in_array);
  }
  if (RightChild(index_in_array) <= size_ &&
      array_of_nums_in_minheap_[sift_id].contain >
          array_of_nums_in_minheap_[RightChild(index_in_array)].contain) {
    sift_id = RightChild(index_in_array);
  }
  if (sift_id == index_in_array) {
    return;
  }
  HugeSwapMin(index_in_array, sift_id);
  SiftDownMin(sift_id);
}

void MinMaxHeap::ExtractMin() {
  HugeSwapMin(1, size_);
  DeleteItemMax(size_);
  --size_;
  array_of_nums_in_minheap_.pop_back();
  SiftDownMin(1);
}

int main() {
  MinMaxHeap heap;
  size_t size = 0;
  int num_of_minimal_elements_in_sequence = 0;
  int first_num_of_sequence = 0;
  int multiplier = 0;
  int num_for_sum = 0;
  std::cin >> size >> num_of_minimal_elements_in_sequence >>
      first_num_of_sequence >> multiplier >> num_for_sum;
  Member intermediate_num_of_sequence;
  intermediate_num_of_sequence.contain = first_num_of_sequence;
  for (int i = 0; i < num_of_minimal_elements_in_sequence; ++i) {
    intermediate_num_of_sequence.contain =
        (intermediate_num_of_sequence.contain * multiplier + num_for_sum) %
        kModulo;
    heap.InsertItem(intermediate_num_of_sequence);
  }
  for (size_t i = num_of_minimal_elements_in_sequence; i < size; ++i) {
    intermediate_num_of_sequence.contain =
        (intermediate_num_of_sequence.contain * multiplier + num_for_sum) %
        kModulo;
    if (heap.ShowMax() > intermediate_num_of_sequence.contain) {
      heap.ExtractMax();
      heap.InsertItem(intermediate_num_of_sequence);
    }
  }
  for (int i = 0; i < num_of_minimal_elements_in_sequence; ++i) {
    std::cout << heap.ShowMin() << '\n';
    heap.ExtractMin();
  }
}