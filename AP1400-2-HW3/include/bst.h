#ifndef BST_H
#define BST_H

#include <initializer_list>
#include <iostream>
#include <functional>

using namespace std;

class BST {
public:
    class Node {
    public:
        Node(int value, Node* left, Node* right) ;
        Node() ;
        Node(const Node& node) ;

        friend std::ostream& operator<<(std::ostream& os, const BST::Node& node);

        friend bool operator>(const BST::Node& node, const int value);
        friend bool operator>=(const BST::Node& node, int value);
        friend bool operator<(const BST::Node& node, int value);
        friend bool operator<=(const BST::Node& node, int value);
        friend bool operator==(const BST::Node& node, int value);
        friend bool operator!=(const BST::Node& node, int value);

        friend bool operator>(const int value, const BST::Node& node);
        friend bool operator>=(const int value, const BST::Node& node);
        friend bool operator<(const int value, const BST::Node& node);
        friend bool operator<=(const int value, const BST::Node& node);
        friend bool operator==(const int value, const BST::Node& node);

        int value;
        Node* left;
        Node* right;
    };

    BST();
    BST(BST& other);                // Copy
    BST(BST&& other) noexcept;               // Move
    BST &operator=(BST& other);     // Copy
    BST &operator=(BST&& other) noexcept;    // Move
    BST(initializer_list<int> values);
    
    ~BST();

    Node*& get_root() ;
    void bfs(std::function<void(Node*& node)> func) const;
    size_t length() const;
    bool add_node(const int value);
    Node** find_node(const int value);
    Node** find_parent(const int value);
    Node** find_successor(const int value);
    bool delete_node(const int value);

    friend std::ostream& operator<<(std::ostream& os, const BST& bst);
    friend BST operator++(BST& bst);
    friend BST operator++(BST& bst, int);

private:
    Node* root;
};

#endif //BST_H