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
#include <iostream>
#include <cstddef>
#include <cassert>

template<typename T>
struct stack {
    size_t size;
    size_t buffer_size;
    T *data;

    stack() : size(0), buffer_size(8) {
        data = new T[buffer_size];
    }

    ~stack() {
        delete[] data;
    }

    void double_buffer() {
        T *new_data = new T[2 * buffer_size];//check null ?
        buffer_size = buffer_size * 2;
        for (size_t i = 0; i < size; ++i) {
            new_data[i] = data[i];
        }

        delete[] data;
        data = new_data;
    }

    void push(T value) {
        if (size >= buffer_size) {
            double_buffer();
        }

        data[size] = value;
        ++size;
    }

    int pop() {
        assert(!empty());
        size--;
        return data[size];
    }

    bool empty() {
        return size == 0;
    }
};

class queue {
private:
    stack<int> in;
    stack<int> out;
public:
    void push(int v);

    int pop();

    bool empty();

};

void queue::push(int v) {
    in.push(v);
}

int queue::pop() {
    assert(!empty());
    if (out.empty()) {
        while (!in.empty()) {
            out.push(in.pop());
        }
    }
    return out.pop();
}

bool queue::empty() {
    return in.empty() && out.empty();
}

void test() {
    stack<int> st;
    assert(st.empty());
    //assert(st.top()==-1);
    assert(st.pop() == -1);
    st.push(5);
    assert(st.size == 1);
    st.push(2);
    // std::cout<<st.pop()<<std::endl;
    assert(st.size == 2);
    assert(st.pop() == 2);
    assert(st.pop() == 5);

    queue q;
    assert(q.pop() == -1);
    q.push(1);
    q.push(2);
    q.push(3);
    q.push(4);
    q.push(5);
    q.push(6);
    assert(q.pop() == 1);
    assert(q.pop() == 2);
    assert(q.pop() == 3);
    assert(q.pop() == 4);
    assert(q.pop() == 5);
    assert(q.pop() == 6);
}

int main(int argc, char **argv) {
    std::string result = "YES";

    //test();
    int n = 0;
    std::cin >> n;
    int command = -1;
    int value = -1;
    queue q;
    for (size_t i = 0; i < n; ++i) {
        std::cin >> command;
        std::cin >> value;

        switch (command) {
            case 2: {
                int tmp = -1;
                if (!q.empty()) {
                    tmp = q.pop();
                }
                if (value != tmp) {
                    result = "NO";
                }
            }
                break;
            case 3:
                q.push(value);
                break;
        }
    }

    std::cout << result << std::endl;
    return 0;
}