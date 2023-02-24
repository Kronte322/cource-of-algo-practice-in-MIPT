// 76612670
#include <algorithm>
#include <iostream>
#include <optional>
#include <string>

class StackWithMinSustain {
 public:
  StackWithMinSustain(size_t sz);

  ~StackWithMinSustain();

  void Push(int n);

  int Pop();

  int Back();

  int Front();

  int Min();

  int Size() const;

  void Clear();

  bool Empty() const;

 private:
  int* array_ = nullptr;
  int* array_for_mins_ = nullptr;
  size_t capacity_ = 0;
  int size_ = -1;
  int top_for_mins_ = -1;

  bool DoesMemAlloced();

  void Allocation();
};

StackWithMinSustain::StackWithMinSustain(size_t sz) : capacity_{sz} {
  array_ = new int[capacity_];
  array_for_mins_ = new int[capacity_];
}

StackWithMinSustain::~StackWithMinSustain() {
  delete[] array_;
  delete[] array_for_mins_;
}

void StackWithMinSustain::Push(int n) {
  if (!DoesMemAlloced()) {
    Allocation();
  }
  ++size_;
  array_[size_] = n;
  if (Empty()) {
    ++top_for_mins_;
    array_for_mins_[top_for_mins_] = n;
    return;
  }
  ++top_for_mins_;
  array_for_mins_[top_for_mins_] =
      std::min(n, array_for_mins_[top_for_mins_ - 1]);
}

int StackWithMinSustain::Pop() {
  --size_;
  --top_for_mins_;
  return array_[size_ + 1];
}
int StackWithMinSustain::Back() { return array_[size_]; }

int StackWithMinSustain::Front() { return array_[0]; }

int StackWithMinSustain::Min() {
  if (Empty()) {
    return 0;
  }
  return array_for_mins_[top_for_mins_];
}

int StackWithMinSustain::Size() const { return size_ + 1; }

void StackWithMinSustain::Clear() {
  size_ = -1;
  top_for_mins_ = -1;
  delete[] array_;
  delete[] array_for_mins_;
  array_ = nullptr;
  array_for_mins_ = nullptr;
}

bool StackWithMinSustain::Empty() const { return (top_for_mins_ == -1); }

bool StackWithMinSustain::DoesMemAlloced() { return !(array_ == nullptr); }

void StackWithMinSustain::Allocation() {
  array_ = new int[capacity_];
  array_for_mins_ = new int[capacity_];
}

class QueueWithMinSustain {
 public:
  QueueWithMinSustain(StackWithMinSustain& first_stack,
                      StackWithMinSustain& second_stack);

  void Pop();

  void Front();

  void Min();

  void Size();

  void Clear();

  void PushInFirstStack(int& item);

 private:
  StackWithMinSustain& first_stack_;
  StackWithMinSustain& second_stack_;
};

QueueWithMinSustain::QueueWithMinSustain(StackWithMinSustain& first_stack,
                                         StackWithMinSustain& second_stack)
    : first_stack_{first_stack}, second_stack_{second_stack} {}

void QueueWithMinSustain::PushInFirstStack(int& item) {
  first_stack_.Push(item);
}

void QueueWithMinSustain::Pop() {
  if (second_stack_.Size() + first_stack_.Size() == 0) {
    std::cout << "error\n";
    return;
  }
  if (second_stack_.Size() == 0) {
    int hold_size = 0;
    hold_size = first_stack_.Size();
    for (int i = 0; i < hold_size; ++i) {
      second_stack_.Push(first_stack_.Pop());
    }
  }
  std::cout << second_stack_.Pop() << '\n';
}

void QueueWithMinSustain::Front() {
  if (second_stack_.Size() + first_stack_.Size() == 0) {
    std::cout << "error\n";
  } else if (second_stack_.Size() == 0) {
    std::cout << first_stack_.Front() << '\n';
  } else {
    std::cout << second_stack_.Back() << '\n';
  }
}

void QueueWithMinSustain::Min() {
  if (first_stack_.Size() + second_stack_.Size() == 0) {
    std::cout << "error\n";
    return;
  }
  if (first_stack_.Size() == 0) {
    std::cout << second_stack_.Min() << '\n';
    return;
  }
  if (second_stack_.Size() == 0) {
    std::cout << first_stack_.Min() << '\n';
    return;
  }
  if ((first_stack_.Min() < second_stack_.Min())) {
    std::cout << first_stack_.Min() << '\n';
    return;
  }
  std::cout << second_stack_.Min() << '\n';
}

void QueueWithMinSustain::Size() {
  std::cout << first_stack_.Size() + second_stack_.Size() << '\n';
}

void QueueWithMinSustain::Clear() {
  first_stack_.Clear();
  second_stack_.Clear();
  std::cout << "ok\n";
}

void ProgInput(const std::string& str, QueueWithMinSustain& queue) {
  if (str == "enqueue") {
    int input = 0;
    std::cin >> input;
    std::cout << "ok\n";
    queue.PushInFirstStack(input);
  } else if (str == "dequeue") {
    queue.Pop();
  } else if (str == "front") {
    queue.Front();
  } else if (str == "min") {
    queue.Min();
  } else if (str == "size") {
    queue.Size();
  } else if (str == "clear") {
    queue.Clear();
  }
}

int main() {
  size_t size = 0;
  std::cin >> size;
  StackWithMinSustain first_stack(size);
  StackWithMinSustain second_stack(size);
  QueueWithMinSustain queue(first_stack, second_stack);
  std::string str;
  for (size_t i = 0; i < size; ++i) {
    std::cin >> str;
    ProgInput(str, queue);
  }
  return 0;
}
