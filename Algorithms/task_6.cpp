//
// Created by Ovanes on 03.11.2019.
//

//Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
//Требуется построить бинарное дерево, заданное наивным порядком вставки.
//Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
// то узел K добавляется в правое поддерево root;
// иначе в левое поддерево root.
//Рекурсия запрещена.

//6_4. Выведите элементы в порядке level-order (по слоям, “в ширину”).
//
//in
//3
//2 1 3
//out
//2 1 3

//in
//3
//1 2 3
//out
//1 2 3

//in
//3
//3 1 2
//out
//3 1 2

//in
//4
//3 1 4 2
//out
//3 1 4 2

#include <iostream>
#include <queue>

template<typename T>
struct Node
{
    T value = 0;
    Node *left = nullptr;
    Node *right = nullptr;

    explicit Node(T _value) : value(_value)
    {};
};

template<typename T>
void deleteNode(Node<T> *node)
{
    delete node;
}

template<typename T>
void printNode(Node<T> *node)
{
    std::cout << node->value << " ";
}

template<typename T>
class BinaryTrie
{
public:
    void Print() const;

    void Add(int value);

    ~BinaryTrie();

private:
    void levelOrder(void (*func)(Node<T> *)) const;

    Node<T> *root = nullptr;
};

template<typename T>
void BinaryTrie<T>::Add(int value)
{
    if (!root)
    {
        root = new Node<T>(value);
    }
    else
    {
        auto current_node = root;
        while (current_node)
        {
            if (value < current_node->value)
            {
                if (!current_node->left)
                {
                    current_node->left = new Node<T>(value);
                    return;
                }
                current_node = current_node->left;
            }
            else
            {
                if (!current_node->right)
                {
                    current_node->right = new Node<T>(value);
                    return;
                }
                current_node = current_node->right;
            }
        }
    }
}

template<typename T>
void BinaryTrie<T>::Print() const
{
    levelOrder(printNode);
}

template<typename T>
BinaryTrie<T>::~BinaryTrie()
{
    levelOrder(deleteNode);
}

template<typename T>
void BinaryTrie<T>::levelOrder(void (*func)(Node<T> *)) const
{
    std::queue<Node<T> *> queue;
    if (root)
        queue.push(root);

    while (!queue.empty())
    {
        Node<T> *current_node = queue.front();
        queue.pop();
        if (current_node->left)
            queue.push(current_node->left);

        if (current_node->right)
            queue.push(current_node->right);

        func(current_node);
    }
}

void task_6(size_t n)
{
    BinaryTrie<int> tree;
    for (size_t i = 0; i < n; i++)
    {
        int v;
        std::cin >> v;
        tree.Add(v);
    }
    tree.Print();
}

int main(void)
{
    size_t n;
    std::cin >> n;
    task_6(n);
    return 0;
}

