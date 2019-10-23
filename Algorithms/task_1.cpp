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
class stack{
public:
    stack();

    ~stack();

    void push(T value);

    int pop();

    bool empty();

    size_t getSize() const;

private:
    void double_buffer();

    size_t size;
    size_t buffer_size;
    T *data;
};

template<typename T>
int stack<T>::pop(){
    assert(!empty());
    size--;
    return data[size];
}

template<typename T>
bool stack<T>::empty(){
    return size == 0;
}

template<typename T>
void stack<T>::push(T value){
    if(size >= buffer_size){
        double_buffer();
    }

    data[size] = value;
    ++size;
}

template<typename T>
void stack<T>::double_buffer(){
    T *new_data = new T[2 * buffer_size];//check null ?
    buffer_size = buffer_size * 2;
    for(size_t i = 0; i < size; ++i){
        new_data[i] = data[i];
    }

    delete[] data;
    data = new_data;
}

template<typename T>
stack<T>::~stack(){
    delete[] data;
}

template<typename T>
stack<T>::stack() : size(0), buffer_size(8){
    data = new T[buffer_size];
}

template<typename T>
size_t stack<T>::getSize() const{
    return size;
}

class queue{
private:
    stack<int> in;
    stack<int> out;
public:
    void push(int v);

    int pop();

    bool empty();

};

void queue::push(int v){
    in.push(v);
}

int queue::pop(){
    assert(!empty());
    if(out.empty()){
        while(!in.empty()){
            out.push(in.pop());
        }
    }
    return out.pop();
}

bool queue::empty(){
    return in.empty() && out.empty();
}

int main(int argc, char **argv){
    std::string result = "YES";

    int n = 0;
    std::cin >> n;
    int command = -1;
    int value = -1;
    queue q;
    for(size_t i = 0; i < n; ++i){
        std::cin >> command;
        std::cin >> value;

        switch(command){
            case 2: {
                int tmp = -1;
                if(!q.empty()){
                    tmp = q.pop();
                }
                if(value != tmp){
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