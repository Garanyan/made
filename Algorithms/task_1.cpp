//
// Created by Ovanes on 13.10.2019.
//
//
//Задача № 1 (5 баллов)
//Во всех задачах из следующего списка следует написать структуру данных, обрабатывающую команды push* и pop*.
//Формат входных данных.
//В первой строке количество команд n. n ≤ 1000000.
//Каждая команда задаётся как 2 целых числа: a b.
//        a = 1 - push front
//        a = 2 - pop front
//        a = 3 - push back
//        a = 4 - pop back
//Для очереди используются команды 2 и 3. Для дека используются все четыре команды.
//Если дана команда pop*, то число b - ожидаемое значение.Если команда pop вызвана для пустой структуры данных, то ожидается “-1”.
//Формат выходных данных.
//Требуется напечатать YES - если все ожидаемые значения совпали. Иначе, если хотя бы одно ожидание не оправдалось, то напечатать NO.

//1_3 Реализовать очередь с помощью двух стеков. Использовать стек, реализованный с помощью динамического буфера.
//
//3
//3 44
//3 50
//2 44
//YES
//
//2
//2 -1
//3 10
//YES
//
//2
//3 44
//2 66
//NO


#include <string>
#include <cstdio>
#include <iostream>
#include <cstddef>
#include <cassert>
template<typename T>
class stack {
public:
    stack();

    ~stack();

    void push(T);

    bool empty() const;

    size_t size() const;

    T pop();

    T top() const;

private:
    struct node {
        T data;
        node *next;
    };
    node *top_node;
    size_t len;
};

template<typename T>
bool stack<T>::empty() const {
    return len == 0 && top_node == nullptr;
}

template<typename T>
stack<T>::stack() {
    top_node = nullptr;
    len = 0;
}

template<typename T>
void stack<T>::push(T val) {
    node *new_top = new node;
    new_top->data = val;
    new_top->next = top_node;
    top_node = new_top;
    ++len;
}

template<typename T>
T stack<T>::top() const {
    if (!empty())
        return top_node->data;
    else
        return -1;//fail
}

template<typename T>
size_t stack<T>::size() const {
    return len;
}

template<typename T>
T stack<T>::pop() {
    if(top_node != nullptr) {
        node *to_delete = top_node;
        T result = to_delete->data;
        top_node = top_node->next;
        delete to_delete;
        --len;
        return result;
    } else{
        return -1;
    }
}

template<typename T>
stack<T>::~stack() {
    while (top_node != nullptr) {
        node *tmp = top_node;
        top_node = top_node->next;
        delete tmp;
    }
}

class queue {
private:
    stack<int> in;
    stack<int> out;
public:
    void push(int v);

    int pop();

    int front();

    //int back();
};

int queue::front() {
    if (out.empty()) {
        while (!in.empty()) {
//            in->out
            out.push(in.pop());
        }
    }

    if (!out.empty()) {
        return out.top();
    } else {
        return -1;
    }
}

void queue::push(int v) {
    in.push(v);
}

int queue::pop() {
    if (out.empty()) {
        while (!in.empty()) {
            out.push(in.pop());
        }
    }

    if (!out.empty()) {
        return out.pop();
    } else {
        return -1;
    }
}

void test()
{
    stack<int> st;
    assert(st.empty());
    assert(st.top()==-1);
    assert(st.pop()==-1);
    st.push(5);
    assert(st.size()==1);
    st.push(2);
    assert(st.size()==2);
    assert(st.pop()==2);
    assert(st.pop()==5);

    queue q;
    assert(q.pop()==-1);
    q.push(1);
    q.push(2);
    q.push(3);
    q.push(4);
    q.push(5);
    q.push(6);
    assert(q.pop()==1);
    assert(q.pop()==2);
    assert(q.pop()==3);
    assert(q.pop()==4);
    assert(q.pop()==5);
    assert(q.pop()==6);
}

int main(int argc, char **argv) {
    std::string result = "YES";

    //test();
    int n = 0;
    std::cin >> n;
    int command = -1;
    int value = -1;
    queue v;
    for (size_t i = 0; i < n; ++i) {
        std::cin >> command;
        std::cin >> value;

        switch (command) {
//            case 1:
//                break;

            case 2: {
                const int tmp = v.pop();
                if (value != tmp) {
                    result = "NO";
                }
            }
                break;
            case 3:
                v.push(value);
                break;
//            case 4:
//                break;
        }
    }

    std::cout << result;
    return 0;
}