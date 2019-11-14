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


#include <iostream>
#include <random>

struct Node
{
    int value = 0;
    int priority = 0;
    size_t subtree_size = 0;
    Node *left = nullptr;
    Node *right = nullptr;

    explicit Node(int _value, int _priority) : value(_value) ,priority(_priority)
    {};
};


void Split(Node *node, int key, Node *&left, Node *&right)
{
    if (!node)
    {
        left = right = nullptr;
    } else if (node->value <= key)
    {
        Split(node->right, key,
              node->right, right);

        left = node;
    } else
    {
        Split(node->left, key,
              left, node->left);

        right = node;
    }
}

Node *Merge(Node *left, Node *right)
{
    if (!left || !right)
        return left == nullptr ? right : left;
    if (left->priority > right->priority)
    {
        left->right = Merge(left->right, right);
        return left;
    } else
    {
        right->left = Merge(left, right->left);
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

    size_t size();

private:
    size_t size_ = 0;
    Node * root = nullptr;
};

void DecatrTree::Add(int value)
{
    std::random_device random_device;
    std::mt19937 random_engine(random_device());
    std::uniform_int_distribution<int> distribution(-3*size_, 3*size_);

    Add(value, distribution(random_engine));
}

void DecatrTree::Add(int value, int priority)
{
    std::cout<<priority<<std::endl;
    Node *left;
    Node *right;
    Split(root, value, left, right);
    Node *new_node = new Node(value, priority);
    left = Merge(left, new_node);
    root = Merge(left, right);
    ++size_;
}

void DecatrTree::Remove(int value)
{
    Node *left;
    Node *T2;
    Split(root, value, left, T2);
    Node *T1;
    Node *tmp;
    Split(left, value - 0.1,T1, tmp);
    root = Merge(T1, T2);

}

int DecatrTree::GetStatistic(size_t order) const
{
    return -1;
}


void task_7(size_t n )
{

    DecatrTree tree;
    for (size_t i = 0; i < n; i++)
    {
        int value, staticsticId;
        std::cin >> value >> staticsticId;

        if(value < 0)
        {
            tree.Remove(-value);
        }
        else
        {
            tree.Add(value);
        }
        std::cout<<tree.GetStatistic(staticsticId)<<std::endl;
    }
}

int main(void)
{
    size_t n;
    std::cin >> n;
    task_7(n);
    std::cout << std::endl;
    return 0;
}
