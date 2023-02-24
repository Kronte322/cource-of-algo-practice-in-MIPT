// 78986095
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

struct Item {
  int value = 0;
  int index = 0;
};

struct Pair {
  std::pair<int64_t, int64_t> first;
  std::pair<int64_t, int64_t> second;
};

void SortBubble(std::vector<std::pair<int64_t, int64_t>>& vect, long start,
                long end) {
  for (long i = 0; i < end - start; ++i) {
    for (long j = start; j < end - i; ++j) {
      if (vect[j].first > vect[j + 1].first && j + 1 <= end) {
        std::swap(vect[j], vect[j + 1]);
      }
    }
  }
}

class SparseTable {
 public:
  SparseTable(std::vector<int>& vect);

  int64_t Answer(int& l_board, int& r_board);

 private:
  std::vector<std::vector<Pair>> table_ = {};
  int power_of_two_ = 0;
  int num_of_items_ = 0;
};

SparseTable::SparseTable(std::vector<int>& vect)
    : power_of_two_{static_cast<int>(floor(log2(vect.size())))},
      num_of_items_{static_cast<int>(vect.size())} {
  table_.resize(power_of_two_ + 1);
  for (int i = 0; i < power_of_two_ + 1; ++i) {
    table_[i].resize(num_of_items_);
  }

  for (int i = 0; i <= power_of_two_; ++i) {
    for (int j = 0; j < num_of_items_ - (1 << i) + 1; ++j) {
      if (i == 0) {
        std::pair<int64_t, int64_t> item;
        item.first = vect[j];
        item.second = j;
        table_[i][j].first = item;
        table_[i][j].second = item;
      } else if (i == 1) {
        table_[i][j].first = table_[i - 1][j].first;
        table_[i][j].second = table_[i - 1][j + 1].first;
      } else {
        std::vector<std::pair<int64_t, int64_t>> interm = {
            table_[i - 1][j].first, table_[i - 1][j].second,
            table_[i - 1][j + (1 << (i - 1))].first,
            table_[i - 1][j + (1 << (i - 1))].second};
        SortBubble(interm, 0, 3);
        table_[i][j].first = interm[0];
        table_[i][j].second = interm[1];
      }
    }
  }
}

int64_t SparseTable::Answer(int& l_board, int& r_board) {
  int power = static_cast<int>(floor(log2(r_board - l_board)));

  std::vector<std::pair<int64_t, int64_t>> interm = {
      table_[power][l_board].first, table_[power][l_board].second,
      table_[power][r_board - (1 << (power)) + 1].first,
      table_[power][r_board - (1 << (power)) + 1].second};

  SortBubble(interm, 0, 3);
  std::pair<int64_t, int64_t> item = interm[0];
  for (int i = 1; i <= 3; ++i) {
    if (interm[i].second != item.second) {
      return interm[i].first;
    }
  }
  return 0;
}

int main() {
  int num = 0;
  std::cin >> num;
  int req = 0;
  std::cin >> req;
  std::vector<int> vect = {};
  vect.reserve(num);
  int interm = 0;
  for (int i = 0; i < num; ++i) {
    std::cin >> interm;
    vect.push_back(interm);
  }
  SparseTable table(vect);
  int l_board = 0;
  int r_board = 0;
  for (int i = 0; i < req; ++i) {
    std::cin >> l_board >> r_board;
    --l_board;
    --r_board;
    std::cout << table.Answer(l_board, r_board) << '\n';
  }
}