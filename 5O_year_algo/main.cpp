// 79778994
#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <vector>

template <typename HashFunction = std::hash<int>>
class HashTable {
 public:
  HashTable() : table_(1) {}

  ~HashTable() {}

  void Insert(int item) {
    auto& bucket = GetBucket(item);
    if (std::find(bucket.begin(), bucket.end(), item) == bucket.end()) {
      bucket.push_back(item);
      ++size_;
    }
    if (size_ > table_.size() * kLoadFactor) {
      Resize();
    }
  }

  bool Find(int item) {
    const auto& bucket = GetBucket(item);
    return (std::find(bucket.begin(), bucket.end(), item) != bucket.end());
  }

  void Erase(int item) {
    auto& bucket = GetBucket(item);
    auto item_iterator = std::find(bucket.begin(), bucket.end(), item);
    if (item_iterator != bucket.end()) {
      bucket.erase(item_iterator);
    }
  }

 private:
  void Resize() {
    std::vector<std::list<int>> new_table;
    new_table.resize(std::max(static_cast<int>(table_.size() * 2), 1));

    std::vector<std::list<int>> hold = table_;
    table_ = new_table;
    size_ = 0;

    for (auto& bucket : hold) {
      for (auto& item_iterator : bucket) {
        Insert(item_iterator);
      }
    }
  }

  auto& GetBucket(int item) {
    return table_[HashFunction{}(item) % table_.size()];
  }

  const double kLoadFactor = 2.0;
  std::vector<std::list<int>> table_;
  size_t size_ = 0;
};

int main() {
  HashTable table{};
  int num_of_requests = 0;
  std::cin >> num_of_requests;
  std::string request;
  int item = 0;
  for (int i = 0; i < num_of_requests; ++i) {
    std::cin >> request;
    if (request == "+") {
      std::cin >> item;
      table.Insert(item);
    } else if (request == "-") {
      std::cin >> item;
      table.Erase(item);
    } else if (request == "?") {
      std::cin >> item;
      if (table.Find(item)) {
        std::cout << "YES\n";
      } else {
        std::cout << "NO\n";
      }
    }
  }
}
