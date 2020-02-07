//
// Created by Ovanes on 26.10.2019.
//

// 3_1 брать по К их сливать но сливать до момента набора К элементов
//

// 4_3

// 7_1 7_2 декартово/ сплей

#include <iostream>
#include <random>

struct TreapNode{
    int value = 0;
    int priority = 0;
    TreapNode *left = nullptr;
    TreapNode *right = nullptr;

    explicit TreapNode(int _value) : value(_value), priority(std::rand()){};
};

struct TreeNode{
    int value = 0;
    TreeNode *left = nullptr;
    TreeNode *right = nullptr;

    explicit TreeNode(int _value) : value(_value){};

};


class Tree{
public:
    ~Tree();

    void Print() const;

    void Add(int value);

private:
    TreeNode *root = nullptr;

    void print_subtree(TreeNode *node) const;

    void delete_subtree(TreeNode *node);
};

void Tree::print_subtree(TreeNode *node) const{
    if(!node) return;
    print_subtree(node->left);
    std::cout << node->value << " ";
    print_subtree(node->right);
}

void Tree::Print() const{
    print_subtree(root);
}

void Tree::Add(int value){
    if(!root){
        root = new TreeNode(value);
    }
}

Tree::~Tree(){
    delete_subtree(root);
}

void Tree::delete_subtree(TreeNode *node){
    if(!node)
        return;
    delete_subtree(node->left);
    delete_subtree(node->right);
    delete node;
}


class Treap{
public:
    ~Treap();

private:
    TreapNode *root = nullptr;

    std::pair<TreapNode *, TreapNode *> Split(TreapNode *node, int x);

    void delete_subtree(TreapNode *node);
};

Treap::~Treap(){
    delete_subtree(root);
}

// поворот
TreapNode* Rotate(TreapNode* root){
    if(!root){
        return nullptr;
    }
    if(!root->left){
        //сигнализировать ошибку?
        return root;
    }
    TreapNode* new_root = root->left;
    root->left = new_root->right;
    new_root->right = root;
    return  new_root;
}

std::pair<TreapNode *, TreapNode *> Treap::Split(TreapNode *node, int x){
    if(!node){
        return std::make_pair(nullptr, nullptr);
    }

    if(node->value < x){// a>=b !a<b
        const auto right_pair = Split(node->right, x);
        node->right = right_pair.first;
        return std::make_pair(node, right_pair.second);
    }
    else{
        const auto left_pair = Split(node->left, x);
        node->left = left_pair.second;
        return std::make_pair(left_pair.first, node);
    }
}

int main(){
    Tree tree;
    tree.Add(5);
    tree.Print();
    return 0;
}