/*
 Дано число N ≤ 104 и последовательность целых чисел из [-231..231] длиной N. Требуется построить бинарное дерево, заданное наивным порядком вставки.
 Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
 Выведите элементы в порядке post-order (снизу вверх). Рекурсия запрещена.
 */
#include <iostream>
#include <algorithm>
#include <cassert>
#include <vector>
#include <set>

template <class T>
struct IsLess{
public:
    bool operator()(const T& A, const T& B){
        return A < B;
    }
};

template <class T>
class BinarySearchTree{
private:
    struct Node{
        T value;
        Node* parent;
        Node* right;
        Node* left;
    };
    Node* root;
    size_t len;
    IsLess<T> compr;
public:
    BinarySearchTree();
    BinarySearchTree&operator = (const BinarySearchTree&);
    void Add(const T&);
    void Delete(const T&);
    size_t size();
    std::vector<T> PostOrder();
    ~BinarySearchTree();
};

template <class T>
BinarySearchTree<T>::BinarySearchTree(): len(0), root(nullptr){
}

template <class T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(const BinarySearchTree<T> &other) {
    if (this == &other)
        return *this;
    len = other.len;
    root = other.root;
    other.root = nullptr; other.len = 0;
    return *this;
}

template <class T>
size_t BinarySearchTree<T>::size() {
    return len;
}

template <class T>
void BinarySearchTree<T>::Add(const T & elem) {
    if (!len){
        root = new Node;
        root->right = nullptr;
        root->left = nullptr;
        root->parent = nullptr;
        root->value = elem;
        len++;
        return;
    }
    Node* tmp = root;
    while(tmp){
        if (compr(elem,tmp->value)){
            if (tmp->left)
                tmp = tmp->left;
            else {
                tmp->left = new Node;
                tmp->left->left = nullptr;
                tmp->left->right = nullptr;
                tmp->left->parent = tmp;
                tmp->left->value = elem;
                len++;
                return;
            }
        }
        else{
            if (tmp->right)
                tmp = tmp->right;
            else{
                tmp->right = new Node;
                tmp->right->left = nullptr;
                tmp->right->right = nullptr;
                tmp->right->parent = tmp;
                tmp->right->value = elem;
                len++;
                return;
            }
        }
    }
}

template <class T>
void BinarySearchTree<T>::Delete(const T &elem) {
    Node* tmp = root;
    while(tmp){
        if(tmp->value == elem)
            break;
        if(compr(elem,tmp->value))
            tmp = tmp->left;
        else
            tmp = tmp->right;
    }
    if(!tmp)
        return;
    Node* to_delete = tmp ;
    if (to_delete->left)
        to_delete = to_delete->left;
    while (to_delete->right)
        to_delete = to_delete->right;
    if (to_delete != root){
        if (to_delete->parent->right == to_delete)
            to_delete->parent->right = nullptr;
        else
            to_delete->parent->left = nullptr;
        tmp->value = to_delete->value;
    }
    delete to_delete;
    len--;
}
template <class T>
BinarySearchTree<T>::~BinarySearchTree(){
    Node* tmp = root;
    while (len){
        while (!(!tmp->left && !tmp->right)){
            if (tmp->left)
                tmp = tmp->left;
            else
                tmp = tmp->right;
        }
        Node* to_delete = tmp;
        if(root != tmp) {
            tmp = tmp->parent;
            if (tmp->left == to_delete)
                tmp->left = nullptr;
            else
                tmp->right = nullptr;
        }
        delete to_delete;
        len--;
    }
}

template <class T>
std::vector<T> BinarySearchTree<T>::PostOrder() {
    std::vector<T> result;
    Node* tmp = this->root;
    std::set<BinarySearchTree::Node*> visited;
    for (int i = 0; i < this->len; i++){
        while (!(!tmp->left && !tmp->right)){
            if (tmp->left && visited.find(tmp->left) == visited.end())
                tmp = tmp->left;
            else if(tmp->right && visited.find(tmp->right) == visited.end())
                tmp = tmp->right;
            else
                break;
        }
        T value = tmp->value;
        result.push_back(value);
        visited.insert(tmp);
        tmp = tmp->parent;
    }
    return result;
}

int main() {
    BinarySearchTree<int> tree;
    size_t n = 0;
    std::cin >> n;
    for (int i = 0; i < n; i++){
        int value = 0;
        std::cin >> value;
        tree.Add(value);
    }
    std::vector<int> result;
    result = tree.PostOrder();
    for (auto i : result){
        std::cout << i << " ";
    }
    return 0;
}