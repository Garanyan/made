//
// Created by ovanes on 14.11.2019.
//
// Порядковые статистики. Дано число N и N строк.
// Каждая строка содержит команду добавления или удаления натуральных чисел,
// а также запрос на получение k-ой порядковой статистики.
// Команда добавления числа A задается положительным числом A,
// команда удаления числа A задается отрицательным числом “-A”.
// Запрос на получение k-ой порядковой статистики задается числом k.

// Вариант 7_3.  Требуемая скорость выполнения запроса - O(log n) в среднем.
// В реализации используйте декартово дерево.
//
//in
//5
//40 0
//10 1
//4 1
//-10 0
//50 2

//out
//40
//40
//10
//4
//50


#include <iostream>
#include <random>
#include <queue>
#include <cassert>

struct Node
{
    int value = 0;
    int priority = 0;
    size_t subtree_size = 1;
    Node* left = nullptr;
    Node* right = nullptr;

    explicit Node(int _value, int _priority) : value(_value), priority(_priority)
    {};

    void update()
    {
        subtree_size = 1;
        if (left != nullptr)
        {
            subtree_size += left->subtree_size;
        }
        if (right != nullptr)
        {
            subtree_size += right->subtree_size;
        }
    }
};


void Split(Node* node, int key, Node*& left, Node*& right)
{
    if (!node)
    {
        left = right = nullptr;
    }
    else if (node->value <= key)
    {
        Split(node->right, key,
            node->right, right);

        left = node;
        left->update();
    }
    else
    {
        Split(node->left, key,
            left, node->left);

        right = node;
        right->update();
    }
}

Node* Merge(Node* left, Node* right)
{
    if (!left || !right)
        return left == nullptr ? right : left;
    if (left->priority > right->priority)
    {
        left->right = Merge(left->right, right);
        left->update();
        return left;
    }
    else
    {
        right->left = Merge(left, right->left);
        right->update();
        return right;
    }
}

class DecatrTree
{
public:
    void Add(int value);

    void Add(int value, int priority);

    void Remove(int value);

    void Print() const;

    int GetStatistic(size_t order) const;

    size_t size() const;

    ~DecatrTree();

private:

    int GetStatistic(Node* node, size_t order) const;

    size_t size_ = 0;
    Node* root = nullptr;
};


void DecatrTree::Add(int value)
{
    std::random_device random_device;
    std::mt19937 random_engine(random_device());

    int priority = 0;
    Node* left;
    Node* right;
    Split(root, value, left, right);
    if (!left && !right)
    {
        std::uniform_int_distribution<int> distribution(-300, 3 * 100);
        priority = distribution(random_engine);
    }
    else if (left && !right)
    {
        int t = std::abs(left->priority);
        std::uniform_int_distribution<int> distribution(-t, t);
        priority = distribution(random_engine);
    }
    priority = 0;
    Node* new_node = new Node(value, priority);
    left = Merge(left, new_node);
    root = Merge(left, right);

    ++size_;
}

void DecatrTree::Remove(int value)
{
    Node* left;
    Node* T2;
    Split(root, value, left, T2);
    Node* T1;
    Node* tmp;
    Split(left, value - 0.1, T1, tmp);
    root = Merge(T1, T2);
    delete tmp;

}

int DecatrTree::GetStatistic(size_t order) const
{
    if (root)
    {
        return GetStatistic(root, order);
    }
    else
    {
        assert(false);
    }
}

int DecatrTree::GetStatistic(Node* node, size_t order) const
{
    if (order <= node->subtree_size)
    {
        if (node->left != nullptr && node->left->subtree_size == order)
        {
            return node->value;
        }
        if (node->left)
            return GetStatistic(node->left, order);
        else
            return node->value;

    }
    else
    {
        if (node->right)
            return GetStatistic(node->right, order - node->left->subtree_size - 1);
        else
            return node->value;
    }

}

size_t DecatrTree::size() const
{
    return size_;
}

void DecatrTree::Print() const
{
    std::queue<Node*> queue;
    if (root)
        queue.push(root);

    while (!queue.empty())
    {
        Node* current_node = queue.front();
        queue.pop();
        if (current_node->left)
            queue.push(current_node->left);

        if (current_node->right)
            queue.push(current_node->right);

        std::cout << "(" << current_node->value << "," << current_node->priority << ", " << current_node->subtree_size << ")" << std::endl;
    }
}

DecatrTree::~DecatrTree()
{
    std::queue<Node*> queue;
    if (root)
        queue.push(root);

    while (!queue.empty())
    {
        Node* current_node = queue.front();
        queue.pop();
        if (current_node->left)
            queue.push(current_node->left);

        if (current_node->right)
            queue.push(current_node->right);

        delete current_node;
    }
}


void task_7(size_t n)
{
    DecatrTree tree;
    for (size_t i = 0; i < n; i++)
    {
        int value, staticsticId;
        std::cin >> value >> staticsticId;

        if (value < 0)
        {
            tree.Remove(-value);
        }
        else
        {
            tree.Add(value);
        }
        std::cout << tree.GetStatistic(staticsticId) << std::endl;
    }
    //tree.Print();
    std::cout << std::endl;
}

int main(void)
{
    size_t n;
    std::cin >> n;
    task_7(n);
    std::cout << std::endl;
    return 0;
}
