#include "../include/bst.h"
#include "gmock/gmock.h"
#include <cstddef>
#include <iostream>
#include <ostream>
#include <queue>
#include <iomanip>

BST::Node::Node(int value, Node* left, Node* right) : value{value}, left{left}, right{right} {}

BST::Node::Node() : value{0}, left{nullptr}, right{nullptr} {}

BST::Node::Node(const Node& node) : value(node.value), left(node.left), right(node.right) {}

std::ostream& operator<<(std::ostream& os, const BST::Node& node) {
    os << left << setw(18) << &node << "==> value:";
    os << setw(5) << left << node.value;
    os << "left:" << setw(18) << left << &node.left->value;
    os << "right:" << setw(18) << &node.right->value;
    return os;
}

std::ostream& operator<<(std::ostream& os, const BST& bst) {
    os << string(80, '*') << endl;
    size_t size = 0;
    queue<BST::Node*> q;
    if (bst.root != nullptr) {
        q.push(bst.root);
    }
    while (!q.empty()) {
        BST::Node*& node = q.front();
        os << *node << endl;
        if (node->left != nullptr) {
            q.push(node->left);
        }
        if (node->right != nullptr) {
            q.push(node->right);
        }
        q.pop();
        ++size;
    }
    os << "binary search tree size: " << size << endl;
    os << string(80, '*') << endl;
    return os;
}

bool operator>(const BST::Node& node, const int value) {
    return node.value > value;
}

bool operator>=(const BST::Node& node, int value) {
    return node.value >= value;
}

bool operator<(const BST::Node& node, int value) {
    return node.value < value;
}

bool operator<=(const BST::Node& node, int value) {
    return node.value <= value;
}

bool operator==(const BST::Node& node, int value) {
    return node.value == value;
}

bool operator!=(const BST::Node& node, int value) {
    return node.value != value;
}

bool operator>(const int value, const BST::Node& node) {
    return value > node.value;
}

bool operator>=(const int value, const BST::Node& node) {
    return value >= node.value;
}

bool operator<(const int value, const BST::Node& node) {
    return value < node.value;
}

bool operator<=(const int value, const BST::Node& node) {
    return value <= node.value;
}

bool operator==(const int value, const BST::Node& node) {
    return value == node.value;
}

bool operator!=(const int value, const BST::Node& node) {
    return value != node.value;
}

BST::Node*& BST::get_root() {
    return root;
}

void BST::bfs(std::function<void(Node*& node)> func) const{
    if (this->root == nullptr) {
        return;
    }

    queue<Node *> q;
    q.push(this->root);
    while (!q.empty()) {
        func(q.front());
        if (q.front()->left != nullptr) {
            q.push(q.front()->left);
        }
        if (q.front()->right != nullptr) {
            q.push(q.front()->right);
        }
        q.pop();
    }
    return;
}

size_t BST::length() const {
    size_t len = 0;
    if (root == nullptr) {
        return len;
    }
    queue<Node *> q;
    q.push(root);
    ++len;
    while (!q.empty()) {
        if (q.front()->left != nullptr) {
            q.push(q.front()->left);
            ++len;
        }
        if (q.front()->right != nullptr) {
            q.push(q.front()->right);
            ++len;
        }
        q.pop();
    }
    return len;
}

bool add_helper(BST::Node*& node, int const value) {
    if (node == nullptr) {
        node = new BST::Node(value, nullptr, nullptr);
        return true;
    }
    if (node->value == value) {
        return false;
    } else if (node->value > value) {
        return add_helper(node->left, value);
    } else {
        return add_helper(node->right, value);
    }
}

bool BST::add_node(const int value) {
    return add_helper(root, value);
}

BST::Node** BST::find_node(int value) {
    if (root == nullptr) {
        return nullptr;
    }
    Node** node_ptr = &root;
    while (*node_ptr != nullptr) {
        if ((*node_ptr)->value == value) {
            return node_ptr;
        } else if ((*node_ptr)->value > value) {
            node_ptr = &(*node_ptr)->left;
        } else {
            node_ptr = &(*node_ptr)->right;
        }
    }
    return nullptr;
}

BST::Node** BST::find_parent(const int value) {
    if (root == nullptr || root->value == value) {
        return nullptr;
    }
    Node** par_node = &root;
    Node** child_node = &root;
    while (*child_node != nullptr) {
        if ((*child_node)->value == value) {
            return par_node;
        } else if ((*child_node)->value > value) {
            par_node = child_node;
            child_node = &(*child_node)->left;
        } else {
            par_node = child_node;
            child_node = &(*child_node)->right;
        }
    }
    return nullptr;
}

BST::Node** BST::find_successor(const int value) {
    Node** ance_node = find_node(value);
    if (*ance_node == nullptr) {
        return nullptr;
    }
    Node** suc_node = &(*ance_node)->left;
    while ((*suc_node)->right != nullptr) {
        suc_node = &(*suc_node)->right;
    }
    return suc_node;
}

bool BST::delete_node(const int value) {
    Node** node = find_node(value);
    if (node == nullptr) {
        /* Empty tree */
        return false;
    } else if ((*node)->left == nullptr && (*node)->right == nullptr) {
        /* Node has no child */
        Node** par_node = find_parent(value);
        if ((*par_node)->left == *node) {
            (*par_node)->left = nullptr;
        } else {
            (*par_node)->right = nullptr;
        }
        delete *node;
        return true;
    }  else if ((*node)->left == nullptr && (*node)->right != nullptr) {
        /* Only right child. */
        Node** par_node = find_parent(value);
        if ((*par_node)->left == *node) {
            (*par_node)->left = (*node)->right;
        } else {
            (*par_node)->right = (*node)->right;
        }
        return true;
    } else if ((*node)->left != nullptr && (*node)->right == nullptr) {
        cout << "original node: " << **node << endl;
        /* Only left child. */
        Node** par_node = find_parent(value);
        if ((*par_node)->left == *node) {
            (*par_node)->left = (*node)->left;
        } else {
            (*par_node)->right = (*node)->left;
        }
        return true;
    } else {
        /* Both children exist.*/
        cout << **node << endl;
        Node** suc_node = find_successor(value);
        cout << **suc_node << endl;
        /* Delete the branch to suc_node. */
        Node** par_node = find_parent((*suc_node)->value);
        /* Replace the node with the successor. */
        (*node)->value = (*suc_node)->value;
        cout << **par_node << endl;
        if ((*par_node)->left == *suc_node) {
            (*par_node)->left = nullptr;
        } else {
            (*par_node)->right = nullptr;
        }
        return true;
    }
}

BST operator++(BST& bst) {
    BST::Node* node = bst.get_root();
    queue<BST::Node*> q;
    if (node == nullptr) {
        return bst;
    }

    q.push(node);
    while (!q.empty()) {
        node = q.front();
        if (node->left != nullptr) {
            q.push(node->left);
        }
        if (node->right != nullptr) {
            q.push(node->right);
        }
        node->value += 1;
        q.pop();
    }
    return bst;
}

BST operator++(BST& bst, int) {
    BST temp(bst);  // Save current state
    BST::Node* node = bst.get_root();
    std::queue<BST::Node*> q;
    if (node != nullptr) {
        q.push(node);
    }
    while (!q.empty()) {
        node = q.front();
        if (node->left != nullptr) { q.push(node->left); }
        if (node->right != nullptr) { q.push(node->right); }
        node->value += 1;
        q.pop();
    }
    return temp;
}

BST::BST() : root{nullptr} {}

BST::BST(BST& other) : root{nullptr} {
    if (this == &other) {
        return;
    }

    Node *node = other.get_root();
    if (node == nullptr) {
        this->root = nullptr;
        return;
    }

    queue<BST::Node*> q;
    q.push(node);
    cout << "test" << endl;
    while (!q.empty()) {
        node = q.front();
        add_node(node->value);
        if (node->left != nullptr) {
            q.push(node->left);
        }
        if (node->right != nullptr) {
            q.push(node->right);
        }
        q.pop();
        cout << *this << endl;
    }
}

BST::BST(BST&& other) noexcept{
    this->root = other.root;
    other.root = nullptr;
}

 BST::~BST()
 {
 	std::vector<Node*> nodes;
 	bfs([&nodes](BST::Node*& node){nodes.push_back(node);});
 	for(auto& node: nodes)
 		delete node;
 }

 BST::BST(initializer_list<int> values) : root{nullptr} {
    for (auto& value : values) {
        add_node(value);
    }
 }

 BST &BST::operator=(BST& other) {
    if (this == &other) {
        return *this;
    }
    Node* node = other.get_root();
    if (other.get_root() == nullptr) {
        this->root = nullptr;
        return *this;
    }
    queue<BST::Node*> q;
    q.push(node);
    while (!q.empty()) {
        node = q.front();
        add_node(node->value);
        if (node->left != nullptr) {
            q.push(node->left);
        }
        if (node->right != nullptr) {
            q.push(node->right);
        }
        q.pop();
    }
    return *this;
 }

 BST &BST::operator=(BST&& other) noexcept{
    this->root = other.root;
    other.root = nullptr;
    return *this;
 }
