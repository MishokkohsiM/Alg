/*
Дано число N < 106 и последовательность пар целых чисел из [-231, 231] длиной N.
Построить декартово дерево из N узлов, характеризующихся парами чисел (Xi, Yi).
Каждая пара чисел (Xi, Yi) определяет ключ Xi и приоритет Yi в декартовом дереве.

Вычислить разницу глубин наивного дерева поиска и декартового дерева.
Разница может быть отрицательна, необходимо вывести модуль разности.
*/

#include <iostream>
#include <set>
#include <cassert>
#include <vector>

template <class T>
struct IsLess {
public:
    bool operator()(const T &one, const T &other) {
        return one < other;
    }
};

template <class Key, class Prior>
class Treap{
private:
    struct Node{
        Key key;
        Prior prior;
        Node* parent;
        Node* left;
        Node* right;
    };
    IsLess<Key> compr_key;
    IsLess<Prior> compr_prior;
    Node* root;
    size_t len;
    void split(Node*, const Key&, Node*&, Node*&);
    Node* merge(Node*, Node*);
    void find_root(Node*);
public:
    Treap();
    Treap&operator = (const Treap&);
    void Add(const Key&, const Prior&);
    void Delete(const Key&);
    int GetDepth ();
    ~Treap();
};

template <class Key, class Prior>
Treap<Key,Prior>::Treap(): root(nullptr), len(0) {}

template <class Key, class Prior>

Treap<Key, Prior>& Treap<Key, Prior>::operator = (const Treap &other) {
    if (this == &other)
        return *this;
    len = other.len;
    root = other.root;
    other.root = nullptr; other.len = 0;
    return *this;
}

template <class Key, class Prior>
void Treap<Key, Prior>::split(typename Treap<Key, Prior>::Node* curr, const Key& key, Treap<Key, Prior>::Node*& treap1, Treap<Key, Prior>::Node*& treap2) {
    if (!curr) {
        treap1 = nullptr;
        treap2 = nullptr;
        return;
    }
    else {
        if (compr_key(key, curr->key)) {
            split(curr->left, key, treap1, curr->left);
            treap2 = curr;
            if (treap2->left)
                treap2->left->parent = treap2;
            if (treap2->right)
                treap2->right->parent = treap2;
            if (treap1) {
                if (treap1->left)
                    treap1->left->parent = treap1;
                if (treap1->right)
                    treap1->right->parent = treap1;
            }
            return;
        }
        else {
            split(curr->right, key, curr->right, treap2);
            treap1 = curr;
            if (treap2) {
                if (treap2->left)
                    treap2->left->parent = treap2;
                if (treap2->right)
                    treap2->right->parent = treap2;
            }
            if (treap1->left)
                treap1->left->parent = treap1;
            if (treap1->right)
                treap1->right->parent = treap1;
            return;
        }
    }
}

template <class Key, class Prior>
typename Treap<Key, Prior>::Node* Treap<Key, Prior>::merge(Treap<Key, Prior>::Node* treap1, Treap<Key, Prior>::Node* treap2) {
    if (!treap1)
        return treap2;
    if (!treap2)
        return treap1;
    if (prior_compr(treap1->prior, treap2->prior)) {
        treap2->left = merge(treap1, treap2->left);
        if (treap2->left)
            treap2->left->parent = treap2;
        return treap2;
    }
    else {
        treap1->right = merge(treap1->right, treap2);
        if (treap1->right)
            treap1->right->parent = treap1;
        return treap1;
    }
}

template <class Key, class Prior>
void Treap<Key, Prior>::find_root(Node *curr) {
    while(curr->parent)
        curr = curr->parent;
    root = curr;
}

template <class Key, class Prior>
void Treap<Key, Prior>::Add(const Key &key, const Prior &prior) {
    Node* tmp = root;
    Node* par = nullptr;
    while (tmp) {
        if (compr_prior(tmp->prior,prior))
            break;
        if (compr_key(key, tmp->key)) {
            par = tmp;
            tmp = tmp->left;
        }
        else {
            par = tmp;
            tmp = tmp->right;
        }
    }
    if (!tmp) {
        tmp = new Node;
        tmp->prior = prior;
        tmp->key = key;
        tmp->left = nullptr;
        tmp->right = nullptr;
        tmp->parent = par;
        if (par) {
            if (compr_key(key, par->key))
                par->left = tmp;
            else
                par->right = tmp;
        }
        if (!len)
            root = tmp;
        else
            find_root(tmp);
    }
    else {
        Node* T1;
        Node* T2;
        split(tmp, key, T1, T2);
        Node* new_node = new Node;
        if(par) {
            if(par->left == tmp)
                par->left = new_node;
            else
                par->right = new_node;
        }
        new_node->key = key;
        new_node->prior = prior;
        new_node->left = T1;
        new_node->right = T2;
        if (T1)
            T1->parent = new_node;
        if (T2)
            T2->parent = new_node;

        new_node->parent = par;
        find_root(new_node);
    }
    len++;
}

template <class Key, class Prior>
void Treap<Key, Prior>::Delete(const Key & key) {
    if (!len)
        return;
    Node* tmp = root;
    while (tmp) {
        if (tmp->key == key)
            break;
        if (key_compr(key, tmp->key))
            tmp = tmp->left;
        else
            tmp = tmp->right;
    }
    if (tmp) {
        Node* par = tmp->parent;
        Node* new_node = merge(tmp->left, tmp->right);
        if (par) {
            if (par->left == tmp)
                par->left = new_node;
            else
                par->right = new_node;
        }
        if (new_node->right)
            new_node->right->parent = new_node;
        if (new_node->left)
            new_node->left->parent = new_node;
        new_node->parent = par;
        tmp->left = nullptr;
        tmp->right = nullptr;
        tmp->parent = nullptr;
        delete tmp;
        find_root(new_node);
        len--;
    }
}

template <class Key, class Prior>
Treap<Key, Prior>::~Treap() {
    Node* tmp = root;
    while (len) {
        while (!(!tmp->left && !tmp->right)) {
            if (tmp->left)
                tmp = tmp->left;
            else if (!tmp->left && tmp->right)
                tmp = tmp->right;
        }
        Node* to_delete = tmp;
        if (root != tmp) {
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

template <class Key, class Prior>
int Treap<Key,Prior>::GetDepth() {
    std::set<Treap::Node*> visited;
    if (!this->len)
        return 0;
    Node* tmp = this->root;
    size_t max_depth = 1;
    size_t tmp_depth = 1;
    for (int i =0; i < this->len; ++i) {
        while (!(!tmp->left && !tmp->right)) {
            if (tmp->left && visited.find(tmp->left) == visited.end()) {
                tmp = tmp->left;
                ++tmp_depth;
            }
            else if (tmp->right && visited.find(tmp->right) == visited.end()) {
                tmp = tmp->right;
                ++tmp_depth;
            }
            else
                break;
        }
        if (max_depth < tmp_depth)
            max_depth = tmp_depth;
        visited.insert(tmp);
        tmp = tmp->parent;
        --tmp_depth;
    }
    return max_depth;
}




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
    int GetDepth();
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
    for (int i = 0; i < this->len; i++){
        while (!(!tmp->left && !tmp->right)){
            if (tmp->left && tmp->left->isbe)
                tmp = tmp->left;
            else if (tmp->right && tmp->right->isbe)
                tmp = tmp->right;
            else
                break;
        }
        T value = tmp->value;
        result.push_back(value);
        tmp->isbe = false;
        tmp = tmp->parent;
    }
    return result;
}

template <class T>
int BinarySearchTree<T>::GetDepth() {
    std::set<BinarySearchTree::Node*> visited;
    if (!this->len)
        return 0;
    Node* tmp = this->root;
    size_t max_depth = 1;
    size_t tmp_depth = 1;
    for (int i =0; i < this->len; ++i) {
        while (!(!tmp->left && !tmp->right)) {
            if (tmp->left && visited.find(tmp->left) == visited.end()) {
                tmp = tmp->left;
                ++tmp_depth;
            }
            else if (tmp->right && visited.find(tmp->right) == visited.end()) {
                tmp = tmp->right;
                ++tmp_depth;
            }
            else
                break;
        }
        if (max_depth < tmp_depth)
            max_depth = tmp_depth;
        visited.insert(tmp);
        tmp = tmp->parent;
        --tmp_depth;
    }
    return max_depth;
}


int main()
{
    Treap<int, int> treap;
    BinarySearchTree<int> tree;
    int n;
    std::cin >> n;
    for (int i = 0; i < n; ++i) {
        int key1, key2;
        std::cin >> key1 >> key2;
        treap.Add(key1, key2);
        tree.Add(key1);
    }
    std::cout << abs(treap.GetDepth()- tree.GetDepth());
    return 0;
}