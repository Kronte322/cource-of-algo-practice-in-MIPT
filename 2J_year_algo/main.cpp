// 75592011
#include <iostream>
#include <vector>

struct Member {
  int64_t contain;
  int64_t index_in_vector;
  size_t index;
};

class MinHeap {
 public:
  void InsertItem(Member val);

  Member ShowMin() const { return vect_[0]; };

  void DecreaseKey(int64_t num, int64_t val);

  Member ExtractMin();

 private:
  int64_t size_ = 0;
  std::vector<Member> vect_;

  void SiftDown(int64_t index_in_vector);

  void SiftUp(int64_t index_in_vector);

  void DeleteItem(int64_t index_in_vector);
};

int64_t Parent(int64_t index_in_vector) { return index_in_vector >> 1; }

int64_t LeftChild(int64_t index_in_vector) { return index_in_vector << 1; }

int64_t RightChild(int64_t index_in_vector) {
  return (index_in_vector << 1) + 1;
}

void MinHeap::InsertItem(Member val) {
  ++size_;
  vect_.push_back(val);
  SiftUp(size_ - 1);
}

void MinHeap::SiftUp(int64_t index_in_vector) {
  if (index_in_vector == 0) {
    return;
  }
  if (vect_[index_in_vector].contain < vect_[Parent(index_in_vector)].contain) {
    std::swap(vect_[index_in_vector], vect_[Parent(index_in_vector)]);
  }
  SiftUp(Parent(index_in_vector));
}

void MinHeap::SiftDown(int64_t index_in_vector) {
  int64_t sift_id = index_in_vector;
  if (LeftChild(index_in_vector) < size_ &&
      vect_[index_in_vector].contain >
          vect_[LeftChild(index_in_vector)].contain) {
    sift_id = LeftChild(index_in_vector);
  }
  if (RightChild(index_in_vector) < size_ &&
      vect_[sift_id].contain > vect_[RightChild(index_in_vector)].contain) {
    sift_id = RightChild(index_in_vector);
  }
  if (sift_id == index_in_vector) {
    return;
  }

  std::swap(vect_[index_in_vector], vect_[sift_id]);
  SiftDown(sift_id);
}

Member MinHeap::ExtractMin() {
  Member max_num = ShowMin();
  std::swap(vect_[0], vect_[--size_]);
  vect_.pop_back();
  SiftDown(0);
  return max_num;
}

void MergeSort(std::vector<std::vector<Member>>& vect, int64_t count,
               int64_t general_counter) {
  std::vector<Member> res = {};
  MinHeap* heap = new MinHeap();
  for (int64_t i = 0; i < count; ++i) {
    heap->InsertItem(vect[i][0]);
  }
  for (int64_t i = 0; i < general_counter; ++i) {
    Member interm = heap->ExtractMin();
    std::cout << interm.contain << '\n';
    if (interm.index + 1 < vect[interm.index_in_vector].size()) {
      heap->InsertItem(vect[interm.index_in_vector][interm.index + 1]);
    }
  }
  delete heap;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int64_t count = 0;
  std::cin >> count;
  std::vector<std::vector<Member>> huge_vect;
  huge_vect.resize(count);
  int64_t general_counter = 0;
  for (int64_t i = 0; i < count; ++i) {
    size_t size = 0;
    std::cin >> size;
    general_counter += size;
    huge_vect[i].reserve(size + 1);
    for (size_t j = 0; j < size; ++j) {
      Member interm;
      std::cin >> interm.contain;
      interm.index_in_vector = i;
      interm.index = j;
      huge_vect[i].push_back(interm);
    }
  }
  MergeSort(huge_vect, count, general_counter);
}