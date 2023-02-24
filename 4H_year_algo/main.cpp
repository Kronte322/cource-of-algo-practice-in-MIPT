// 78986543
#include <algorithm>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

struct Node {
  int64_t priority = 0;
  int64_t key = 0;
  int64_t num_of_subtree = 0;
  int64_t min_value_on_subtree = 0;
  int64_t value = 0;
  bool need_reverse = false;
  Node* parent = nullptr;
  Node* left_child = nullptr;
  Node* right_child = nullptr;
};

struct Section {
  int l_board = 0;
  int r_board = 0;
};

struct Item {
  int64_t priority = 0;
  int64_t key = 0;
  int64_t value = 0;
};

class UnorderedDecartTree {
 public:
  Node* GetRoot();

  int64_t GetSize() const;

  UnorderedDecartTree();

  ~UnorderedDecartTree();

  void DeleteTree(Node* current);

  std::pair<Node*, Node*> Split(int64_t& key, Node* current);

  Node* Merge(Node* l_board, Node* r_board);

  void Insert(Item& item, int64_t idx);

  void Rebuild(Item& pair, Node* current);

  void GetMin(Section& request, Section& under_control, Node* current,
              int64_t& min);

  void Reverse(int64_t l_board, int64_t r_board);

 private:
  Node* root_ = nullptr;
  int64_t size_ = 0;
};

Node* UnorderedDecartTree::GetRoot() { return root_; }

int64_t UnorderedDecartTree::GetSize() const { return size_; }

UnorderedDecartTree::UnorderedDecartTree() {}

UnorderedDecartTree::~UnorderedDecartTree() {
  DeleteTree(root_);
  delete root_;
}

void UnorderedDecartTree::DeleteTree(Node* current) {
  if (current->left_child == nullptr && current->right_child == nullptr) {
    return;
  }
  if (current->left_child != nullptr) {
    DeleteTree(current->left_child);
    delete current->left_child;
  }
  if (current->right_child != nullptr) {
    DeleteTree(current->right_child);
    delete current->right_child;
  }
}

void PushFlag(Node* current) {
  if (current == nullptr) {
    return;
  }
  if (!current->need_reverse) {
    return;
  }
  if (current->left_child != nullptr) {
    current->key -= current->left_child->num_of_subtree;
  }
  current->need_reverse = false;
  Node* buffer = nullptr;
  buffer = current->left_child;
  current->left_child = current->right_child;
  if (current->left_child != nullptr) {
    current->key += current->left_child->num_of_subtree;
    current->left_child->parent = current;
    current->left_child->need_reverse ^= 1;
  }
  current->right_child = buffer;
  if (current->right_child != nullptr) {
    current->right_child->parent = current;
    current->right_child->need_reverse ^= 1;
  }
}

std::pair<Node*, Node*> UnorderedDecartTree::Split(int64_t& key,
                                                   Node* current) {
  PushFlag(current);
  if (current == nullptr) {
    return std::pair<Node*, Node*>(nullptr, nullptr);
  }
  Node* tree1 = nullptr;
  Node* tree2 = nullptr;
  std::pair<Node*, Node*> pair(tree1, tree2);
  if (key >= current->key) {
    tree1 = current;
    key -= current->key;
    if (tree1->right_child != nullptr) {
      tree1->num_of_subtree -= tree1->right_child->num_of_subtree;
      if (tree1->left_child != nullptr) {
        tree1->min_value_on_subtree =
            std::min(tree1->left_child->min_value_on_subtree, tree1->value);
      } else {
        tree1->min_value_on_subtree = tree1->value;
      }
    }
    std::tie(tree1->right_child, tree2) = Split(key, current->right_child);
    if (tree1->right_child != nullptr) {
      tree1->min_value_on_subtree =
          std::min(tree1->min_value_on_subtree,
                   tree1->right_child->min_value_on_subtree);
      tree1->num_of_subtree += tree1->right_child->num_of_subtree;
      tree1->right_child->parent = tree1;
    }
    return std::pair<Node*, Node*>(tree1, tree2);
  }
  tree2 = current;
  if (tree2->left_child != nullptr) {
    if (tree2->right_child != nullptr) {
      tree2->min_value_on_subtree =
          std::min(tree2->right_child->min_value_on_subtree, tree2->value);
    } else {
      tree2->min_value_on_subtree = tree2->value;
    }
    tree2->num_of_subtree -= tree2->left_child->num_of_subtree;
    tree2->key -= tree2->left_child->num_of_subtree;
  }
  std::tie(tree1, tree2->left_child) = Split(key, current->left_child);
  if (tree2->left_child != nullptr) {
    tree2->min_value_on_subtree = std::min(
        tree2->min_value_on_subtree, tree2->left_child->min_value_on_subtree);
    tree2->num_of_subtree += tree2->left_child->num_of_subtree;
    tree2->key += tree2->left_child->num_of_subtree;
    tree2->left_child->parent = tree2;
  }
  return std::pair<Node*, Node*>(tree1, tree2);
}

Node* UnorderedDecartTree::Merge(Node* l_board, Node* r_board) {
  PushFlag(l_board);
  PushFlag(r_board);
  if (l_board == nullptr && r_board == nullptr) {
    return nullptr;
  }
  if (r_board == nullptr) {
    return l_board;
  }
  if (l_board == nullptr) {
    return r_board;
  }
  Node* tree;
  if (l_board->priority < r_board->priority) {
    tree = l_board;
    if (r_board != nullptr) {
      tree->num_of_subtree += r_board->num_of_subtree;
      tree->min_value_on_subtree =
          std::min(tree->min_value_on_subtree, r_board->min_value_on_subtree);
    }
    tree->right_child = Merge(tree->right_child, r_board);
    if (tree->right_child == root_) {
      root_ = tree;
    }
    if (tree->right_child != nullptr) {
      tree->right_child->parent = tree;
    }
    return tree;
  }
  tree = r_board;
  if (l_board != nullptr) {
    tree->num_of_subtree += l_board->num_of_subtree;
    tree->key += l_board->num_of_subtree;
    tree->min_value_on_subtree =
        std::min(tree->min_value_on_subtree, l_board->min_value_on_subtree);
  }
  tree->left_child = Merge(l_board, tree->left_child);
  if (tree->left_child != nullptr) {
    if (tree->left_child == root_) {
      root_ = tree;
    }
    tree->left_child->parent = tree;
  }
  return tree;
}

void UnorderedDecartTree::Insert(Item& item, int64_t idx) {
  Node* node = new Node;
  if (size_ == 0) {
    ++size_;
    if (idx == 0) {
      node->key = 1;
      node->num_of_subtree = 1;
    }
    node->priority = item.priority;
    node->min_value_on_subtree = item.value;
    node->value = item.value;
    root_ = node;
    return;
  }
  ++size_;
  Node* tree1 = nullptr;
  Node* tree2 = nullptr;
  node->priority = item.priority;
  node->key = 1;
  node->num_of_subtree = 1;
  node->value = item.value;
  node->min_value_on_subtree = node->value;
  std::tie(tree1, tree2) = Split(idx, root_);
  Merge(tree1, Merge(node, tree2));
}

void UnorderedDecartTree::Rebuild(Item& pair, Node* current) {
  current->min_value_on_subtree =
      std::min(pair.value, current->min_value_on_subtree);
  if (pair.key == current->key) {
    return;
  }
  if (pair.key > current->key) {
    return Rebuild(pair, current->right_child);
  }
  ++current->key;
  return Rebuild(pair, current->left_child);
}

void UnorderedDecartTree::GetMin(Section& request, Section& under_control,
                                 Node* current, int64_t& min) {
  PushFlag(current);
  if (current == nullptr) {
    return;
  }
  if (under_control.l_board + current->key - 1 >= request.l_board &&
      under_control.l_board + current->key - 1 <= request.r_board) {
    if (min == std::numeric_limits<int64_t>::max()) {
      min = current->value;
    }
    min = std::min(current->value, min);
    if (under_control.l_board + current->key - 1 == request.l_board &&
        under_control.l_board + current->key - 1 == request.r_board) {
      return;
    }
  }
  if (request.l_board <= under_control.l_board &&
      request.r_board >= under_control.r_board) {
    if (min == std::numeric_limits<int64_t>::max()) {
      min = current->min_value_on_subtree;
    }
    min = std::min(current->min_value_on_subtree, min);
  } else {
    if (!(request.l_board > under_control.r_board)) {
      Section copy_under_control;
      copy_under_control.l_board = under_control.l_board;
      copy_under_control.r_board = under_control.r_board - 1;
      if (current->right_child != nullptr) {
        copy_under_control.r_board =
            copy_under_control.r_board - current->right_child->num_of_subtree;
      }
      GetMin(request, copy_under_control, current->left_child, min);
    }
    if (!(request.r_board < under_control.l_board)) {
      Section copy_under_control;
      copy_under_control.l_board = under_control.l_board + 1;
      copy_under_control.r_board = under_control.r_board;
      if (current->left_child != nullptr) {
        copy_under_control.l_board =
            copy_under_control.l_board + current->left_child->num_of_subtree;
      }
      GetMin(request, copy_under_control, current->right_child, min);
    }
  }
}

void UnorderedDecartTree::Reverse(int64_t l_board, int64_t r_board) {
  Node* left_tree = nullptr;
  Node* right_tree = nullptr;
  Node* middle_tree = nullptr;
  int64_t left = l_board;
  int64_t right = r_board;
  std::tie(left_tree, right_tree) = Split(left -= 1, root_);
  std::tie(middle_tree, right_tree) = Split(right -= l_board - 1, right_tree);
  middle_tree->need_reverse ^= 1;
  Merge(left_tree, Merge(middle_tree, right_tree));
}

int main() {
  std::random_device random_device;
  std::mt19937 gen(random_device());
  std::uniform_int_distribution<int64_t> distrib(
      0, std::numeric_limits<int64_t>::max());
  int64_t num_of_array = 0;
  int64_t num_of_req = 0;
  std::cin >> num_of_array;
  std::cin >> num_of_req;
  int req;
  int64_t priority = 0;
  int64_t value = 0;
  int64_t l_board = 0;
  int64_t r_board = 0;
  UnorderedDecartTree tree;
  for (int64_t i = 0; i < num_of_array; ++i) {
    priority = distrib(gen);
    std::cin >> value;
    Item item;
    item.priority = priority;
    item.value = value;
    int64_t idx = i;
    tree.Insert(item, idx);
  }
  for (int64_t i = 0; i < num_of_req; ++i) {
    std::cin >> req;
    if (req == 1) {
      std::cin >> l_board >> r_board;
      if (l_board == r_board) {
        continue;
      }
      tree.Reverse(l_board, r_board);
    } else if (req == 2) {
      std::cin >> l_board >> r_board;
      int64_t min = std::numeric_limits<int64_t>::max();
      int64_t left = 1;
      int64_t size = tree.GetSize();
      Section request;
      request.l_board = l_board;
      request.r_board = r_board;
      Section current;
      current.l_board = left;
      current.r_board = size;
      tree.GetMin(request, current, tree.GetRoot(), min);
      std::cout << min << '\n';
    }
  }
}