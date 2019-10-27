//
// Created by Ovanes on 13.10.2019.
//

//Задача № 2 (5 баллов)
//Решение всех задач данного раздела предполагает использование кучи.
//2_4. Скользящий максимум.
//Дан массив целых чисел A[0..n), n не превосходит 10^8. Также задан размер некоторого окна (последовательно расположенных элементов массива) в этом массиве k, k<=n. Требуется для каждого положения окна (от 0 и до n-k) вывести значение максимума в окне. Скорость работы O(n log n), память O(n).
//Формат входных данных. Вначале вводится n - количество элементов массива. Затем вводится n строк со значением каждого элемента. Затем вводится k  - размер окна.
//Формат выходных данных. Разделенные пробелом значения максимумов для каждого положения окна.
// set на дереве
// куча , можно не извлекать мининмум, но смотреть на координату при извлечении (извлекать только при проверке?)
// очередь
// массив максимум на префиксах , массив  максимум на суффиксах


//3
//1 2 3
//2
//2 3
//
//9
//0 7 3 8 4 5 10 4 6
//4
//8 8 8 10 10 10


#include <cstdio>
#include <iostream>
#include <cassert>
#include <vector>

template<typename T>
class heap{
public:
    heap() : size(0), buffer_size(8){
        data = new T[buffer_size];
        indexes = new size_t[buffer_size];
    }

    ~heap(){
        delete[] data;
        delete[] indexes;
    }

    void push(T value, size_t index);

    T pop();

    T top(size_t min_index);

    bool empty() const;

    void delete_val(T val);

    //bool make_heap();

    T &operator[](size_t i);

    size_t index(T val);

private:
    size_t size;
    size_t buffer_size;
    T *data;
    size_t *indexes;

    T &left(size_t i);

    T &right(size_t i);

    T &parent(size_t i);

    size_t parent_id(size_t i);

    size_t left_id(size_t i);

    size_t right_id(size_t i);

    void double_buffer();

    void sift_up(size_t i);

    void sift_down(size_t i);


};

template<typename T>
void heap<T>::sift_up(size_t i){
    if(i > 0 && data[i] > parent(i)){
        std::swap(data[i], parent(i));
        std::swap(indexes[i], indexes[parent_id(i)]);
        sift_up(parent_id(i));
    }
}

template<typename T>
bool heap<T>::empty() const{
    return size == 0;
}

template<typename T>
size_t heap<T>::parent_id(size_t i){
    assert(i > 0 && (i - 1) / 2 < size);
    return (i - 1) / 2;
}

template<typename T>
void heap<T>::sift_down(size_t i){
    size_t max_id = i;

    max_id = left_id(i) < size && data[i] < left(i) ? left_id(i) : max_id;
    max_id = right_id(i) < size && data[max_id] < right(i) ? right_id(i) : max_id;

    if(i != max_id){
        std::swap(data[i], data[max_id]);
        std::swap(indexes[i], indexes[max_id]);
        sift_down(max_id);
    }
}

template<typename T>
size_t heap<T>::right_id(size_t i){
    return 2 * i + 2;
}

template<typename T>
size_t heap<T>::left_id(size_t i){
    return 2 * i + 1;
}

template<typename T>
void heap<T>::push(T value, size_t index){
    if(size >= buffer_size){
        double_buffer();
    }

    data[size] = value;
    indexes[size] = index;
    ++size;
    sift_up(size - 1);
}

template<typename T>
T heap<T>::pop(){
    assert(!empty());
    T max = data[0];
    size--;
    data[0] = data[size];
    indexes[0] = indexes[size];
    sift_down(0);
    return max;
}

template<typename T>
T &heap<T>::operator[](size_t i){
    return data[i];
}

template<typename T>
size_t heap<T>::index(T val){
    for(size_t i = 0; i < size; ++i){
        if(val == data[i])
            return i;
    }
    assert(false);
}

template<typename T>
T heap<T>::top(size_t min_index){
    assert(!empty());
    if(indexes[0] >= min_index){
        return data[0];
    }
    else{
        pop();
        return top(min_index);
    }
}

template<typename T>
void heap<T>::delete_val(T val){
    for(size_t i = 0; i < size; ++i){
        if(val == data[i]){
            size--;
            //std::cout<<"delte "<<data[i] << " val " << data[size] <<std::endl;
            data[i] = data[size];
            sift_down(i);
        }
    }
}

template<typename T>
void heap<T>::double_buffer(){
    T *new_data = new T[2 * buffer_size];//check null ?
    size_t *new_indexes = new size_t[2 * buffer_size];
    buffer_size = buffer_size * 2;
    for(size_t i = 0; i < size; ++i){
        new_data[i] = data[i];
        new_indexes[i] = indexes[i];
    }

    delete[] data;
    delete[] indexes;
    data = new_data;
    indexes = new_indexes;
}

template<typename T>
T& heap<T>::parent(size_t i){
    assert((i - 1) / 2 < size);
    return data[(i - 1) / 2];
}

template<typename T>
T& heap<T>::right(size_t i){
    assert(size > 2 * i + 2);
    return data[2 * i + 2];
}

template<typename T>
T& heap<T>::left(size_t i){
    assert(size > 2 * i + 1);
    return data[2 * i + 1];
}

void task_2(size_t n, std::vector<int> &input, size_t window_size){
    heap<int> h;
    size_t i = 0;
    for(; i < window_size; i++){
        h.push(input[i], i);
    }
    std::cout << h.top(0) << " ";
    for(; i < n; i++){
        h.push(input[i], i);
        std::cout << h.top(i - window_size+1) << " ";
    }
    std::cout << std::endl;
}

void test(){
    size_t n = 5;
    std::vector<int> input({4, 8, 7, 0, 10});
    size_t window_size = 1;
    task_2(n, input, window_size);

    n = 6;
    input = {10, 18, 2, 3, 5, 6};
    window_size = 2;
    task_2(n, input, window_size);

    n = 6;
    input = {20, 1, 0, 7, 8, 3};
    window_size = 3;
    task_2(n, input, window_size);

    n = 6;
    input = {20, 1, 0, 7, 8, 3};
    window_size = 6;
    task_2(n, input, window_size);

}

int main(int argc, char **argv){

    size_t n;
    std::cin >> n;
    std::vector<int> input;

    for(size_t i = 0; i < n; i++){
        int val;
        std::cin >> val;
        input.push_back(val);
    }
    size_t window_size = 0;
    std::cin >> window_size;
    task_2(n, input, window_size);
    return 0;
}