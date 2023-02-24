// 79788239
#include <iostream>
#include <vector>

class SplayTree {
 private:
  struct Node;

 public:
  SplayTree();

  ~SplayTree();

  void Insert(std::pair<std::string, std::string> element);

  std::string Find(std::string element);

 private:
  void TraversalDelete(Node* current);

  void SplayName(Node* item);

  void SplayCar(Node* item);

  void BasicInsertName(Node* item, Node* current);

  void BasicInsertCar(Node* item, Node* current);

  Node* BasicFindName(Node* item, Node* current);

  Node* BasicFindCar(Node* item, Node* current);

  std::string SplayFind(Node* item);

  void SplayInsert(Node* item);

  static void LeftZigName(Node* item);

  static void LeftZigCar(Node* item);

  static void RightZigName(Node* item);

  static void RightZigCar(Node* item);

  int size_ = 0;
  Node* root_name_ = nullptr;
  Node* root_car_ = nullptr;
};

struct SplayTree::Node {
  std::string name;
  std::string car;
  Node* left_name = nullptr;
  Node* right_name = nullptr;
  bool isleaf_name = true;
  Node* parent_name = nullptr;
  Node* left_car = nullptr;
  Node* right_car = nullptr;
  bool isleaf_car = true;
  Node* parent_car = nullptr;
};

SplayTree::SplayTree() {}

SplayTree::~SplayTree() {
  TraversalDelete(root_name_);
  delete root_name_;
}

void SplayTree::Insert(std::pair<std::string, std::string> element) {
  Node* request = new Node;
  request->name = element.first;
  request->car = element.second;
  SplayInsert(request);
}

std::string SplayTree::Find(std::string element) {
  Node request;
  request.name = element;
  request.car = request.name;
  return SplayFind(&request);
}

void SplayTree::TraversalDelete(Node* current) {
  if (current->isleaf_name) {
    return;
  }
  if (current->left_name != nullptr) {
    TraversalDelete(current->left_name);
    delete current->left_name;
  }
  if (current->right_name != nullptr) {
    TraversalDelete(current->right_name);
    delete current->right_name;
  }
}

void SplayTree::LeftZigName(Node* item) {
  Node* hold = item->left_name;
  item->isleaf_name = item->parent_name->isleaf_name;
  item->left_name = item->parent_name;
  item->left_name->right_name = hold;
  if (hold != nullptr) {
    item->left_name->right_name->parent_name = item->left_name;
  }
  if (item->parent_name != nullptr &&
      item->parent_name->parent_name != nullptr) {
    if (item->parent_name->parent_name->left_name == item->parent_name) {
      item->parent_name->parent_name->left_name = item;
    } else {
      item->parent_name->parent_name->right_name = item;
    }
  }
  item->parent_name = item->left_name->parent_name;
  item->left_name->parent_name = item;
  if (!(item->left_name->left_name == nullptr &&
        item->left_name->right_name == nullptr)) {
    item->left_name->isleaf_name = false;
  }
}

void SplayTree::LeftZigCar(Node* item) {
  Node* hold = item->left_car;
  item->isleaf_car = item->parent_car->isleaf_car;
  item->left_car = item->parent_car;
  item->left_car->right_car = hold;
  if (hold != nullptr) {
    item->left_car->right_car->parent_car = item->left_car;
  }
  if (item->parent_car != nullptr && item->parent_car->parent_car != nullptr) {
    if (item->parent_car->parent_car->left_car == item->parent_car) {
      item->parent_car->parent_car->left_car = item;
    } else {
      item->parent_car->parent_car->right_car = item;
    }
  }
  item->parent_car = item->left_car->parent_car;
  item->left_car->parent_car = item;
  if (!(item->left_car->left_car == nullptr &&
        item->left_car->right_car == nullptr)) {
    item->left_car->isleaf_car = false;
  }
}

void SplayTree::RightZigName(Node* item) {
  Node* hold = item->right_name;
  item->isleaf_name = item->parent_name->isleaf_name;
  item->right_name = item->parent_name;
  item->right_name->left_name = hold;
  if (hold != nullptr) {
    item->right_name->left_name->parent_name = item->right_name;
  }
  if (item->parent_name != nullptr &&
      item->parent_name->parent_name != nullptr) {
    if (item->parent_name->parent_name->left_name == item->parent_name) {
      item->parent_name->parent_name->left_name = item;
    } else {
      item->parent_name->parent_name->right_name = item;
    }
  }
  item->parent_name = item->right_name->parent_name;
  item->right_name->parent_name = item;
  if (!(item->right_name->left_name == nullptr &&
        item->right_name->right_name == nullptr)) {
    item->right_name->isleaf_name = false;
  }
}

void SplayTree::RightZigCar(Node* item) {
  Node* hold = item->right_car;
  item->isleaf_car = item->parent_car->isleaf_car;
  item->right_car = item->parent_car;
  item->right_car->left_car = hold;
  if (hold != nullptr) {
    item->right_car->left_car->parent_car = item->right_car;
  }
  if (item->parent_car != nullptr && item->parent_car->parent_car != nullptr) {
    if (item->parent_car->parent_car->left_car == item->parent_car) {
      item->parent_car->parent_car->left_car = item;
    } else {
      item->parent_car->parent_car->right_car = item;
    }
  }
  item->parent_car = item->right_car->parent_car;
  item->right_car->parent_car = item;
  if (!(item->right_car->left_car == nullptr &&
        item->right_car->right_car == nullptr)) {
    item->right_car->isleaf_car = false;
  }
}

void SplayTree::SplayName(Node* item) {
  if (item == root_name_) {
    return;
  }
  if (item->parent_name == root_name_) {
    root_name_ = item;
  }
  if (item->parent_name->right_name == item) {
    LeftZigName(item);
    return SplayName(item);
  }
  RightZigName(item);
  return SplayName(item);
}

void SplayTree::SplayCar(Node* item) {
  if (item == root_car_) {
    return;
  }
  if (item->parent_car == root_car_) {
    root_car_ = item;
  }
  if (item->parent_car->right_car == item) {
    LeftZigCar(item);
    return SplayCar(item);
  }
  RightZigCar(item);
  return SplayCar(item);
}

void SplayTree::BasicInsertName(Node* item, Node* current) {
  if (root_name_ == nullptr) {
    root_name_ = item;
    return;
  }
  if (item->name >= current->name) {
    if (current->right_name == nullptr) {
      current->right_name = item;
      item->parent_name = current;
      current->isleaf_name = false;
      return;
    }
    return BasicInsertName(item, current->right_name);
  }
  if (current->left_name == nullptr) {
    current->left_name = item;
    item->parent_name = current;
    current->isleaf_name = false;
    return;
  }
  return BasicInsertName(item, current->left_name);
}

void SplayTree::BasicInsertCar(Node* item, Node* current) {
  if (root_car_ == nullptr) {
    root_car_ = item;
    return;
  }
  if (item->car >= current->car) {
    if (current->right_car == nullptr) {
      current->right_car = item;
      item->parent_car = current;
      current->isleaf_car = false;
      return;
    }
    return BasicInsertCar(item, current->right_car);
  }
  if (current->left_car == nullptr) {
    current->left_car = item;
    item->parent_car = current;
    current->isleaf_car = false;
    return;
  }
  return BasicInsertCar(item, current->left_car);
}

SplayTree::Node* SplayTree::BasicFindName(Node* item, Node* current) {
  if (item->name == current->name) {
    return current;
  }
  if (item->name > current->name) {
    if (current->right_name == nullptr) {
      return nullptr;
    }
    return BasicFindName(item, current->right_name);
  }
  if (current->left_name == nullptr) {
    return nullptr;
  }
  return BasicFindName(item, current->left_name);
}

SplayTree::Node* SplayTree::BasicFindCar(Node* item, Node* current) {
  if (item->car == current->car) {
    return current;
  }
  if (item->car > current->car) {
    if (current->right_car == nullptr) {
      return nullptr;
    }
    return BasicFindCar(item, current->right_car);
  }
  if (current->left_car == nullptr) {
    return nullptr;
  }
  return BasicFindCar(item, current->left_car);
}

std::string SplayTree::SplayFind(Node* item) {
  Node* result = BasicFindName(item, root_name_);
  if (result != nullptr) {
    SplayName(result);
    SplayCar(result);
    return result->car;
  }
  result = BasicFindCar(item, root_car_);
  if (result != nullptr) {
    SplayCar(result);
    SplayName(result);
    return result->name;
  }
  return "ERROR\n";
}

void SplayTree::SplayInsert(Node* item) {
  BasicInsertName(item, root_name_);
  BasicInsertCar(item, root_car_);
  SplayName(item);
  SplayCar(item);
  ++size_;
}

int main() {
  int num_of_items = 0;
  std::cin >> num_of_items;
  SplayTree* tree = new SplayTree;
  for (int i = 0; i < num_of_items; ++i) {
    std::pair<std::string, std::string> request;
    std::cin >> request.first >> request.second;
    tree->Insert(request);
  }
  int num_of_requests = 0;
  std::cin >> num_of_requests;
  for (int i = 0; i < num_of_requests; ++i) {
    std::string request;
    std::cin >> request;
    std::cout << tree->Find(request) << '\n';
  }
  delete tree;
}