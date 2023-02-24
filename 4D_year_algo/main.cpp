// 78984834
#include <iostream>
#include <vector>

struct Point {
  int x_cord = 0;
  int y_cord = 0;
  int z_cord = 0;
};

int FFunction(int place_in_array) {
  return place_in_array & (place_in_array + 1);
}

int GFunction(int place_in_array) {
  return place_in_array | (place_in_array + 1);
}

class FenvicTree {
 public:
  FenvicTree(int size);

  void Increment(Point& point, int val);

  int64_t GetPrefSum(int x_cord, int y_cord, int z_cord);

  int64_t Sum(Point& first, Point& second);

 private:
  std::vector<std::vector<std::vector<int>>> vect_ = {};
  int size_ = 0;
};

FenvicTree::FenvicTree(int size) : size_{size} {
  vect_.resize(size);
  for (int i = 0; i < size; ++i) {
    vect_[i].resize(size);
  }
  for (int i = 0; i < size; ++i) {
    for (int j = 0; j < size; ++j) {
      vect_[i][j].resize(size);
    }
  }
}

void FenvicTree::Increment(Point& point, int val) {
  for (int i = point.x_cord; i < size_; i = GFunction(i)) {
    for (int j = point.y_cord; j < size_; j = GFunction(j)) {
      for (int t_num = point.z_cord; t_num < size_; t_num = GFunction(t_num)) {
        vect_[i][j][t_num] += val;
      }
    }
  }
}

int64_t FenvicTree::GetPrefSum(int x_cord, int y_cord, int z_cord) {
  int64_t res = 0;
  for (int i = x_cord; i >= 0; i = FFunction(i) - 1) {
    for (int j = y_cord; j >= 0; j = FFunction(j) - 1) {
      for (int t_num = z_cord; t_num >= 0; t_num = FFunction(t_num) - 1) {
        res += vect_[i][j][t_num];
      }
    }
  }
  return res;
}

int64_t FenvicTree::Sum(Point& first, Point& second) {
  int64_t res = 0;
  res = GetPrefSum(second.x_cord, second.y_cord, second.z_cord) -
        GetPrefSum(second.x_cord, first.y_cord - 1, second.z_cord) -
        GetPrefSum(first.x_cord - 1, second.y_cord, second.z_cord) -
        GetPrefSum(second.x_cord, second.y_cord, first.z_cord - 1) +
        GetPrefSum(first.x_cord - 1, first.y_cord - 1, second.z_cord) +
        GetPrefSum(first.x_cord - 1, second.y_cord, first.z_cord - 1) +
        GetPrefSum(second.x_cord, first.y_cord - 1, first.z_cord - 1) -
        GetPrefSum(first.x_cord - 1, first.y_cord - 1, first.z_cord - 1);
  return res;
}

int main() {
  int size = 0;
  std::cin >> size;
  FenvicTree tree(size);
  int req = 0;
  Point first;
  Point second;
  int val = 0;
  while (req != 3) {
    std::cin >> req;
    if (req == 1) {
      std::cin >> first.x_cord >> first.y_cord >> first.z_cord >> val;
      tree.Increment(first, val);
    } else if (req == 2) {
      std::cin >> first.x_cord >> first.y_cord >> first.z_cord >>
          second.x_cord >> second.y_cord >> second.z_cord;
      std::cout << tree.Sum(first, second) << '\n';
    }
  }
}