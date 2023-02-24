// 79787056
#include <iostream>
#include <optional>
#include <vector>

const long kModule = 1000000000;

template <size_t TreeRank>
class BTree {
 private:
  class Node;
  struct Item;

 public:
  BTree();

  ~BTree();

  void Insert(int element);

  void Delete(int element);

  bool Exists(int element);

  int Find(int element);

  std::optional<int> Next(int element);

  std::optional<int> Prev(int element);

  std::optional<int> Kth(size_t element);

  int GetSize() const;

 private:
  void InsertBTree(Item* item, Node* current);

  int FindBTree(int request, Node* current, int interm_res);

  int FindNextBTree(int request, Node* current, int interm_res);

  int FindPrevBTree(int request, Node* current, int interm_res);

  int KthBTree(Node* current, size_t& k_th, int res);

  bool FindBTree(int request, Node* current);

  void DeleteItemBTree(Item* item, Node* current);

  static void Shifting(Node* from, Node* where_to, Node* to_place, int num,
                       int sign);

  Node* Split(Node* current);

  void TraversalDelete(Node* current);

  Node* Merge(Node* left, Node* right, Node* current, size_t num);

  Node* RightWay(Node* where_to, size_t place);

  void BSwap(Node* from, Node* current, size_t place, int side);

  int FindClosiestLeft(Node* current);

  int FindClosiestRight(Node* current);

  Node* DeleteFromInterm(Node* current, size_t place);

  Node* root_ = nullptr;
  size_t size_ = 0;
};

template <size_t TreeRank>
struct BTree<TreeRank>::Item {
  int value = 0;
  BTree<TreeRank>::Node* left = nullptr;
  BTree<TreeRank>::Node* right = nullptr;
};

template <size_t TreeRank>
class BTree<TreeRank>::Node {
 public:
  bool CanInsert() const;

  size_t SearchPlaceInTheNode(BTree<TreeRank>::Item* item);

  void UpdateNode(BTree<TreeRank>::Item* item, size_t place, int sign);

  void InsertInTheNode(BTree<TreeRank>::Item* item);

  void InsertWhileShifting(BTree<TreeRank>::Item* item, int side);

  void DeleteFromNode(size_t place);

  void SetParent();

  void SetChildrens();

  Node();

  ~Node();

  friend BTree;

 private:
  size_t size_ = 0;
  bool is_leaf_ = true;
  std::vector<Item> items_ = {};
  Node* last_child_ = nullptr;
  Node* parent_ = nullptr;
};

template <size_t TreeRank>
bool BTree<TreeRank>::Node::CanInsert() const {
  return size_ < 2 * TreeRank - 1;
}

template <size_t TreeRank>
size_t BTree<TreeRank>::Node::SearchPlaceInTheNode(Item* item) {
  if (size_ == 0) {
    return 0;
  }
  if (item->value < items_[0].value) {
    return 0;
  }
  if (size_ == 1 && item->value >= items_[0].value) {
    return 1;
  }
  for (size_t i = 0; i < size_ - 1; ++i) {
    if (item->value >= items_[i].value && item->value < items_[i + 1].value) {
      return i + 1;
    }
  }
  return size_;
}

template <size_t TreeRank>
void BTree<TreeRank>::Node::UpdateNode(Item* item, size_t place, int sign) {
  if (place == 0 && size_ == 0) {
    items_[size_] = *item;
    if (item->right != nullptr) {
      last_child_ = item->right;
    } else if (item->left != nullptr) {
      last_child_ = item->left;
    }
    return;
  }
  if (place == size_) {
    items_[size_] = *item;
    if (sign == 0) {
      items_[place - 1].right = items_[place].left;
    } else {
      items_[place].left = items_[place - 1].right;
    }
    if (item->right != nullptr) {
      last_child_ = item->right;
    } else if (item->left != nullptr) {
      last_child_ = item->left;
    }
    return;
  }
  Item frst_hold;
  Item scnd_hold;
  frst_hold = items_[place];
  items_[place] = *item;
  for (size_t j = place; j < size_; ++j) {
    scnd_hold = items_[j + 1];
    items_[j + 1] = frst_hold;
    frst_hold = scnd_hold;
  }
  if (place == 0) {
    if (sign == 0) {
      items_[place + 1].left = items_[place].right;
    } else {
      items_[place].right = items_[place + 1].left;
    }
  } else if (place == 2 * TreeRank - 1 - 1) {
    if (sign == 0) {
      items_[place - 1].right = items_[place].left;
    } else {
      items_[place].left = items_[place - 1].right;
    }
  } else {
    if (sign == 0) {
      items_[place - 1].right = items_[place].left;
      items_[place + 1].left = items_[place].right;
    } else {
      items_[place].left = items_[place - 1].right;
      items_[place].right = items_[place + 1].left;
    }
  }
}

template <size_t TreeRank>
void BTree<TreeRank>::Node::InsertInTheNode(BTree<TreeRank>::Item* item) {
  int place = SearchPlaceInTheNode(item);
  UpdateNode(item, place, 0);
  ++size_;
}

template <size_t TreeRank>
void BTree<TreeRank>::Node::InsertWhileShifting(BTree<TreeRank>::Item* item,
                                                int side) {
  if (side == 1) {
    Item frst_hold;
    Item scnd_hold;
    frst_hold = items_[0];
    *(items_.data()) = *item;
    for (size_t j = 0; j < size_; ++j) {
      scnd_hold = items_[j + 1];
      items_[j + 1] = frst_hold;
      frst_hold = scnd_hold;
    }
    items_[0].right = items_[1].left;
  } else {
    items_[size_] = *item;
    items_[size_].left = items_[size_ - 1].right;
  }
  ++size_;
}

template <size_t TreeRank>
void BTree<TreeRank>::Node::DeleteFromNode(size_t place) {
  if (size_ == 1) {
    if (last_child_ != nullptr) {
      delete this;
      return;
    }
    items_[0].left = nullptr;
    items_[0].right = nullptr;
    items_[0].value = 0;
    size_ = 0;
    return;
  }
  if (place != size_ - 1) {
    for (size_t j = place; j < size_ - 1; ++j) {
      items_[j] = items_[j + 1];
    }
    if (place != 0) {
      items_[place - 1].right = items_[place].left;
    }
  } else {
    last_child_ = items_[size_ - 1 - 1].right;
  }
  items_[size_ - 1].left = nullptr;
  items_[size_ - 1].right = nullptr;
  items_[size_ - 1].value = 0;
  --size_;
}

template <size_t TreeRank>
void BTree<TreeRank>::Node::SetParent() {
  for (size_t i = 0; i < size_; ++i) {
    if (items_[i].left != nullptr) {
      items_[i].left->parent_ = this;
    }
  }
  if (items_[size_ - 1].right != nullptr) {
    items_[size_ - 1].right->parent_ = this;
  }
}

template <size_t TreeRank>
void BTree<TreeRank>::Node::SetChildrens() {
  if (!is_leaf_) {
    for (size_t i = 0; i < size_; ++i) {
      items_[i].left->parent_ = this;
    }
    items_[size_ - 1].right->parent_ = this;
  }
}

template <size_t TreeRank>
BTree<TreeRank>::Node::Node() {
  items_.resize(2 * TreeRank - 1);
}

template <size_t TreeRank>
BTree<TreeRank>::Node::~Node() {}

template <size_t TreeRank>
BTree<TreeRank>::BTree() {
  Node* node = new Node();
  root_ = node;
}

template <size_t TreeRank>
BTree<TreeRank>::~BTree() {
  TraversalDelete(root_);
}

template <size_t TreeRank>
void BTree<TreeRank>::Insert(int element) {
  typename BTree<TreeRank>::Item item;
  item.value = element;
  if (size_ == 0) {
    InsertBTree(&item, root_);
  } else {
    if (!FindBTree(item.value, root_)) {
      InsertBTree(&item, root_);
    }
  }
}

template <size_t TreeRank>
void BTree<TreeRank>::Delete(int element) {
  if (size_ == 0) {
  } else {
    BTree<TreeRank>::Item item;
    item.value = element;
    if (FindBTree(item.value, root_)) {
      DeleteItemBTree(&item, root_);
    }
  }
}

template <size_t TreeRank>
bool BTree<TreeRank>::Exists(int element) {
  if (size_ == 0) {
    return false;
  }

  return FindBTree(element, root_);
}

template <size_t TreeRank>
int BTree<TreeRank>::Find(int element) {
  return FindBTree(element, root_, -1);
}

template <size_t TreeRank>
int BTree<TreeRank>::FindBTree(int request, Node* current, int interm_res) {
  if (request < current->items_[0].value) {
    if (current->is_leaf_) {
      return current->items_[0].value;
    }
    return FindBTree(request, current->items_[0].left,
                     current->items_[0].value);
  }
  if (current->items_[current->size_ - 1].value == request) {
    return request;
  }
  if (request > current->items_[current->size_ - 1].value) {
    if (current->is_leaf_) {
      return interm_res;
    }
    return FindBTree(request, current->items_[current->size_ - 1].right,
                     interm_res);
  }
  for (size_t i = 0; i < current->size_ - 1; ++i) {
    if (current->items_[i].value == request) {
      return request;
    }
    if (request > current->items_[i].value &&
        request < current->items_[i + 1].value) {
      if (current->is_leaf_) {
        return current->items_[i + 1].value;
      }
      return FindBTree(request, current->items_[i + 1].left,
                       current->items_[i + 1].value);
    }
  }
  return 0;
}

template <size_t TreeRank>
std::optional<int> BTree<TreeRank>::Next(int element) {
  if (size_ == 0) {
    return {};
  }

  int res = FindNextBTree(element, root_, -1);
  if (res != -1) {
    return res;
  }
  return {};
}

template <size_t TreeRank>
std::optional<int> BTree<TreeRank>::Prev(int element) {
  if (size_ == 0) {
    return {};
  }

  int res = FindPrevBTree(element, root_, -1);
  if (res != -1) {
    return res;
  }
  return {};
}

template <size_t TreeRank>
std::optional<int> BTree<TreeRank>::Kth(size_t element) {
  int res = 0;
  res = KthBTree(root_, element, res);
  if (res != -1) {
    return res;
  }
  return {};
}

template <size_t TreeRank>
typename BTree<TreeRank>::Node* BTree<TreeRank>::Split(Node* current) {
  Node* left_node = new Node();
  Node* right_node = new Node();
  Item interm;
  for (size_t i = 0; i < (2 * TreeRank - 1) / 2; ++i) {
    interm = current->items_[i];
    left_node->InsertInTheNode(&interm);
  }
  for (size_t i = (2 * TreeRank - 1) / 2 + 1; i < (2 * TreeRank - 1); ++i) {
    interm = current->items_[i];
    right_node->InsertInTheNode(&interm);
  }
  left_node->SetParent();
  right_node->SetParent();
  left_node->is_leaf_ = current->is_leaf_;
  right_node->is_leaf_ = current->is_leaf_;
  interm = current->items_[(2 * TreeRank - 1) / 2];
  interm.left = left_node;
  interm.right = right_node;
  if (current == root_) {
    Node* node = new Node();
    node->is_leaf_ = false;
    if (!root_->is_leaf_) {
      interm.left->is_leaf_ = false;
      interm.right->is_leaf_ = false;
    }
    node->InsertInTheNode(&interm);
    left_node->parent_ = node;
    right_node->parent_ = node;
    root_ = node;
    delete current;
    return node;
  }
  current->parent_->InsertInTheNode(&interm);
  left_node->parent_ = current->parent_;
  right_node->parent_ = current->parent_;
  delete current;
  return left_node->parent_;
}

template <size_t TreeRank>
void BTree<TreeRank>::InsertBTree(Item* item, Node* current) {
  if (current->is_leaf_ && current->CanInsert()) {
    current->InsertInTheNode(item);
    ++size_;
    return;
  }
  if (!current->CanInsert()) {
    current = Split(current);
  }
  size_t place = current->SearchPlaceInTheNode(item);
  if (place == current->size_) {
    return InsertBTree(item, current->items_[place - 1].right);
  }
  if (place == 0 || place == current->size_) {
    if (item->value < current->items_[place].value) {
      return InsertBTree(item, current->items_[place].left);
    }
    return InsertBTree(item, current->items_[place].right);
  }
  return InsertBTree(item, current->items_[place].left);
}

template <size_t TreeRank>
void BTree<TreeRank>::TraversalDelete(Node* current) {
  if (current->is_leaf_ && root_ == current) {
    delete current;
    return;
  }
  if (current->is_leaf_) {
    if (current->parent_->last_child_ == current) {
      current->parent_->is_leaf_ = true;
    }
    return;
  }
  for (size_t i = 0; i < current->size_; ++i) {
    if (current->items_[i].left != nullptr) {
      TraversalDelete(current->items_[i].left);
      delete current->items_[i].left;
    }
  }
  if (current->items_[current->size_ - 1].right != nullptr) {
    TraversalDelete(current->items_[current->size_ - 1].right);
    delete current->items_[current->size_ - 1].right;
  }
  if (root_ == current) {
    delete current;
    return;
  }
}

template <size_t TreeRank>
int BTree<TreeRank>::FindNextBTree(int request, Node* current, int interm_res) {
  if (request < current->items_[0].value) {
    if (current->is_leaf_) {
      return current->items_[0].value;
    }
    return FindNextBTree(request, current->items_[0].left,
                         current->items_[0].value);
  }
  if (current->items_[current->size_ - 1].value == request) {
    return request;
  }
  if (request > current->items_[current->size_ - 1].value) {
    if (current->is_leaf_) {
      return interm_res;
    }
    return FindNextBTree(request, current->items_[current->size_ - 1].right,
                         interm_res);
  }
  for (size_t i = 0; i < current->size_ - 1; ++i) {
    if (current->items_[i].value == request) {
      return request;
    }
    if (request > current->items_[i].value &&
        request < current->items_[i + 1].value) {
      if (current->is_leaf_) {
        return current->items_[i + 1].value;
      }
      return FindNextBTree(request, current->items_[i + 1].left,
                           current->items_[i + 1].value);
    }
  }
  return 0;
}

template <size_t TreeRank>
int BTree<TreeRank>::FindPrevBTree(int request, Node* current, int interm_res) {
  if (request < current->items_[0].value) {
    if (current->is_leaf_) {
      return interm_res;
    }
    return FindPrevBTree(request, current->items_[0].left, interm_res);
  }
  if (current->items_[current->size_ - 1].value == request) {
    return request;
  }
  if (request > current->items_[current->size_ - 1].value) {
    if (current->is_leaf_) {
      return current->items_[current->size_ - 1].value;
    }
    return FindPrevBTree(request, current->items_[current->size_ - 1].right,
                         current->items_[current->size_ - 1].value);
  }
  for (size_t i = 0; i < current->size_ - 1; ++i) {
    if (current->items_[i].value == request) {
      return request;
    }
    if (request > current->items_[i].value &&
        request < current->items_[i + 1].value) {
      if (current->is_leaf_) {
        return current->items_[i].value;
      }
      return FindPrevBTree(request, current->items_[i + 1].left,
                           current->items_[i].value);
    }
  }
  return 0;
}

template <size_t TreeRank>
bool BTree<TreeRank>::FindBTree(int request, Node* current) {
  if (current->items_[0].value == request) {
    return true;
  }
  if (request < current->items_[0].value) {
    if (current->is_leaf_) {
      return false;
    }
    return (FindBTree(request, current->items_[0].left));
  }
  for (size_t i = 0; i < current->size_ - 1; ++i) {
    if (current->items_[i].value == request) {
      return true;
    }
    if (current->items_[i].value < request &&
        request < current->items_[i + 1].value) {
      if (current->is_leaf_) {
        return false;
      }
      return FindBTree(request, current->items_[i + 1].left);
    }
  }
  if (request == current->items_[current->size_ - 1].value) {
    return true;
  }
  if (request > current->items_[current->size_ - 1].value) {
    if (current->is_leaf_) {
      return false;
    }
    return FindBTree(request, current->items_[current->size_ - 1].right);
  }
  return false;
}

template <size_t TreeRank>
void BTree<TreeRank>::Shifting(Node* from, Node* where_to, Node* to_place,
                               int num, int sign) {
  Item interm = where_to->items_[num];
  if (sign == 0) {
    where_to->items_[num].value = from->items_[from->size_ - 1].value;
    to_place->InsertWhileShifting(&interm, 1);
    to_place->items_[0].left = from->items_[from->size_ - 1].right;
    if (to_place->items_[0].left != nullptr) {
      to_place->items_[0].left->parent_ = to_place;
    }
    from->DeleteFromNode(from->size_ - 1);
    return;
  }
  where_to->items_[num].value = from->items_[0].value;
  to_place->InsertWhileShifting(&interm, 0);
  to_place->items_[to_place->size_ - 1].right = from->items_[0].left;
  if (to_place->items_[to_place->size_ - 1].right != nullptr) {
    to_place->items_[to_place->size_ - 1].right->parent_ = to_place;
  }
  to_place->last_child_ = to_place->items_[to_place->size_ - 1].right;
  from->DeleteFromNode(0);
}

template <size_t TreeRank>
typename BTree<TreeRank>::Node* BTree<TreeRank>::Merge(Node* left, Node* right,
                                                       Node* current,
                                                       size_t num) {
  Node* node = new Node;
  for (size_t i = 0; i < TreeRank - 1; ++i) {
    node->items_[i] = left->items_[i];
  }
  node->items_[TreeRank - 1] = current->items_[num];
  for (size_t i = 0; i < TreeRank - 1; ++i) {
    node->items_[i + TreeRank - 1 + 1] = right->items_[i];
  }
  node->items_[TreeRank - 1].left = node->items_[TreeRank - 1 - 1].right;
  node->items_[TreeRank - 1].right = node->items_[TreeRank].left;
  if (num != current->size_ - 1) {
    current->items_[num + 1].left = node;
  }
  if (num != 0) {
    current->items_[num - 1].right = node;
  }
  if (current == root_ && current->size_ == 1) {
    root_ = node;
  } else {
    node->parent_ = current;
  }
  node->size_ = 2 * TreeRank - 1;
  node->last_child_ = node->items_[node->size_ - 1].right;
  if (left->last_child_ != nullptr) {
    node->is_leaf_ = false;
  }
  node->SetChildrens();
  current->DeleteFromNode(num);
  delete left;
  delete right;
  return node;
}

template <size_t TreeRank>
typename BTree<TreeRank>::Node* BTree<TreeRank>::RightWay(Node* where_to,
                                                          size_t place) {
  if (where_to->size_ >= TreeRank) {
    return where_to;
  }
  if (place != 0 && place != where_to->parent_->size_) {
    if (where_to->parent_->items_[place - 1].left->size_ >= TreeRank) {
      Shifting(where_to->parent_->items_[place - 1].left, where_to->parent_,
               where_to->parent_->items_[place].left, place - 1, 0);
      return where_to;
    }
  } else if (place == 0) {
    if (where_to->parent_->items_[place].right->size_ >= TreeRank) {
      Shifting(where_to->parent_->items_[place].right, where_to->parent_,
               where_to->parent_->items_[place].left, place, 1);
      return where_to;
    }
  } else if (place == where_to->parent_->size_) {
    if (where_to->parent_->items_[place - 1].left->size_ >= TreeRank) {
      Shifting(where_to->parent_->items_[place - 1].left, where_to->parent_,
               where_to->parent_->items_[place - 1].right, place - 1, 0);
      return where_to;
    }
  }
  if (place != where_to->parent_->size_) {
    if (where_to->parent_->items_[place].right->size_ >= TreeRank) {
      Shifting(where_to->parent_->items_[place].right, where_to->parent_,
               where_to->parent_->items_[place].left, place, 1);
      return where_to->parent_->items_[place].left;
    }
  }
  Node* interm = nullptr;
  if (place != where_to->parent_->size_) {
    interm =
        Merge(where_to->parent_->items_[place].left,
              where_to->parent_->items_[place].right, where_to->parent_, place);
  } else if (place == where_to->parent_->size_) {
    interm = Merge(where_to->parent_->items_[place - 1].left,
                   where_to->parent_->items_[place - 1].right,
                   where_to->parent_, place - 1);
  }
  return interm;
}

template <size_t TreeRank>
void BTree<TreeRank>::BSwap(Node* from, Node* current, size_t place, int side) {
  if (side == 0) {
    current->items_[place].value = FindClosiestLeft(from);
    DeleteItemBTree(&current->items_[place], from);
    return;
  }
  current->items_[place].value = FindClosiestRight(from);
  DeleteItemBTree(&current->items_[place], from);
}

template <size_t TreeRank>
int BTree<TreeRank>::FindClosiestLeft(Node* current) {
  if (current->is_leaf_) {
    return current->items_[current->size_ - 1].value;
  }
  return FindClosiestLeft(current->items_[current->size_ - 1].right);
}

template <size_t TreeRank>
int BTree<TreeRank>::FindClosiestRight(Node* current) {
  if (current->is_leaf_) {
    return current->items_[0].value;
  }
  return FindClosiestRight(current->items_[0].left);
}

template <size_t TreeRank>
typename BTree<TreeRank>::Node* BTree<TreeRank>::DeleteFromInterm(
    Node* current, size_t place) {
  if (current->items_[place].left->size_ >= TreeRank) {
    BSwap(current->items_[place].left, current, place, 0);
    ++size_;
    return nullptr;
  }
  if (current->items_[place].right->size_ >= TreeRank) {
    BSwap(current->items_[place].right, current, place, 1);
    ++size_;
    return nullptr;
  }
  Node* interm = Merge(current->items_[place].left,
                       current->items_[place].right, current, place);
  return interm;
}

template <size_t TreeRank>
void BTree<TreeRank>::DeleteItemBTree(Item* item, Node* current) {
  if (item->value == current->items_[0].value) {
    if (current->is_leaf_) {
      current->DeleteFromNode(0);
      --size_;
      return;
    }
    Node* interm = DeleteFromInterm(current, 0);
    if (interm != nullptr) {
      return DeleteItemBTree(item, interm);
    }
    --size_;
    return;
  }
  if (item->value < current->items_[0].value) {
    Node* interm_2 = RightWay(current->items_[0].left, 0);
    return DeleteItemBTree(item, interm_2);
  }
  if (item->value == current->items_[current->size_ - 1].value) {
    if (current->is_leaf_) {
      current->DeleteFromNode(current->size_ - 1);
      --size_;
      return;
    }
    Node* interm = DeleteFromInterm(current, current->size_ - 1);
    if (interm != nullptr) {
      return DeleteItemBTree(item, interm);
    }
    --size_;
    return;
  }
  if (item->value > current->items_[current->size_ - 1].value) {
    Node* interm_2 =
        RightWay(current->items_[current->size_ - 1].right, current->size_);
    return DeleteItemBTree(item, interm_2);
  }
  for (size_t i = 0; i < current->size_ - 1; ++i) {
    if (item->value == current->items_[i].value) {
      if (current->is_leaf_) {
        current->DeleteFromNode(i);
        --size_;
        return;
      }
      Node* interm = DeleteFromInterm(current, i);
      if (interm != nullptr) {
        return DeleteItemBTree(item, interm);
      }
      --size_;
      return;
    }
    if (item->value > current->items_[i].value &&
        item->value < current->items_[i + 1].value) {
      Node* interm_2 = RightWay(current->items_[i + 1].left, i + 1);
      return DeleteItemBTree(item, interm_2);
    }
  }
}

template <size_t TreeRank>
int BTree<TreeRank>::KthBTree(Node* current, size_t& k_th, int res) {
  if (current->is_leaf_) {
    if (k_th < current->size_) {
      return current->items_[k_th].value;
    }
    k_th -= current->size_;
    return -1;
  }
  for (size_t i = 0; i < current->size_; ++i) {
    if (current->items_[i].left != nullptr) {
      res = KthBTree(current->items_[i].left, k_th, res);
      if (res != -1) {
        return res;
      }
      if (k_th == 0) {
        return current->items_[i].value;
      }
      --k_th;
    }
  }
  if (current->items_[current->size_ - 1].right != nullptr) {
    res = KthBTree(current->items_[current->size_ - 1].right, k_th, res);
    if (res != -1) {
      return res;
    }
  }
  return -1;
}

template <size_t TreeRank>
int BTree<TreeRank>::GetSize() const {
  return size_;
}

int main() {
  BTree<2>* tree = new BTree<2>;
  int num_of_requests = 0;
  std::cin >> num_of_requests;
  std::string request = {};
  std::string previous = "+";
  int res = 0;
  for (int i = 0; i < num_of_requests; ++i) {
    std::cin >> request;
    if (request == "+") {
      int item = 0;
      if (previous == "+") {
        previous = "+";
        std::cin >> item;
        tree->Insert(item);
      } else if (previous == "?") {
        std::cin >> item;
        previous = "+";
        item = (item + res) % kModule;
        tree->Insert(item);
      }
    } else if (request == "?") {
      previous = "?";
      int item = 0;
      std::cin >> item;
      res = tree->Find(item);
      std::cout << res << '\n';
    }
  }
  delete tree;
}