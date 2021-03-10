//
// Created by Ovanes on 3/29/20.
//

// RMQ2
//        ограничение по времени на тест 2 секунды
//        ограничение по памяти на тест 256 мегабайт
//        ввод стандартный ввод
//        вывод стандартный вывод
//
//        Входные данные
//        В первой строке находится число n — размер массива. (1 ≤ n ≤ 10^5)
//        Во второй строке находится n чисел a_i — элементы массива.
//        Далее содержится описание операций, их количество не превышает 2·10^5.
//        В каждой строке находится одна из следующих операций:
//        set i j x — установить все a[k], i ≤ k ≤ j в x.
//        add i j x — увеличить все a[k], i ≤ k ≤ j на x.
//        min i j — вывести значение минимального элемента в массиве на отрезке с i по j, гарантируется, что (1 ≤ i ≤ j ≤ n).
//
// Все числа во входном файле и результаты выполнения всех операций не превышают по модулю 10^18.
// Выходные данные
// Выведите последовательно результат выполнения всех операций min. Следуйте формату выходного файла из примера.


//
//Входные данные
//
//5
//1 2 3 4 5
//min 2 5
//min 1 5
//min 1 4
//min 2 4
//set 1 3 10
//add 2 4 4
//min 2 5
//min 1 5
//min 1 4
//min 2 4
//
//Выходные данные
//2
//1
//1
//2
//5
//5
//8
//8

#include <vector>
#include <iostream>
#include <cassert>

long long func(long long left_value, long long right_value) {
    if (left_value < right_value){
        return left_value;
    }
    return right_value;
}

long long neutral_element() {
    return INT64_MAX;
}

enum update_type{
    ADD = 0,
    SET = 1
};

class SegmentTree{
public:
    explicit SegmentTree(std::vector<long long> &array);

    void set(long long i, long long j, long long x);

    void add(long long i, long long j, long long x);

    long long min(long long i, long long j);

//private:
    void buildTree_(std::vector<long long> &array, long long pos, long long left, long long right);

    long long
    func_on_segment_(long long pos, long long left, long long right, long long left_border, long long right_border);

    void update_(long long pos, long long left, long long right, long long a, long long b, long long val);

    void update(long long pos, long long val);

    long long get_(long long pos, long long l, long long r);

    void push_(long long pos, long long l, long long r);

    void set_(long long pos, long long left, long long right, long long a, long long b, long long val);

    void set_(long long pos, long long val);

    std::vector<long long> tree_;

    std::vector<long long> upd_;

    std::vector<bool> set_ind_;

    long long x_;

    long long real_size_;
};

SegmentTree::SegmentTree(std::vector<long long> &array) {
    real_size_ = array.size();
    long long k = 0;
    while ((1 << k) < array.size()){
        ++k;
    }
    x_ = 1 << k;
    tree_.resize(2 * (1 << k) - 1, neutral_element());
    upd_.resize(2 * (1 << k) - 1, 0);
    set_ind_ = std::vector<bool>(2 * (1 << k) - 1, false);
    buildTree_(array, 0, 0, (1 << k) - 1);
}

void SegmentTree::buildTree_(std::vector<long long> &array, long long pos, long long left, long long right) {
    if (left > array.size()){
        return;
    }
    if (left == right){
        if (left < array.size()){
            tree_[pos] = array[left];
        }
        else{
            tree_[pos] = neutral_element();
        }
        return;
    }

    auto mid = (left + right) / 2;
    buildTree_(array, 2 * pos + 1, left, mid);
    buildTree_(array, 2 * pos + 2, mid + 1, right);
    tree_[pos] = func(tree_[2 * pos + 1], tree_[2 * pos + 2]);
}

long long SegmentTree::min(long long i, long long j) {
    return func_on_segment_(0, 0, x_ - 1, std::min(i, real_size_ - 1), std::min(j, real_size_ - 1));
}

void SegmentTree::add(long long i, long long j, long long x) {
    update_(0, 0, x_ - 1, i, j, x);
}

void SegmentTree::set(long long i, long long j, long long x) {
    set_(0, 0, x_ - 1, i, j, x);
}


/// O(log_2(n))
/// \param pos позиция вершины в массиве дерева
/// \param left левая граница отрезка, за которую отвечает вершина
/// \param right правая граница отрезка за которую отвечает вершина
/// \param left_border левая граница отрезка запроса
/// \param right_border правая граница отрезка запроса
/// \return результат func на отрезке запроса
long long
SegmentTree::func_on_segment_(long long pos, long long left, long long right, long long left_border,
                              long long right_border) {
    push_(pos, left, right);
    if (left > right || right < left_border || right_border < left){
        return neutral_element();
    }

    if (left_border <= left && right <= right_border){
        return get_(pos, left, right);
    }


    return func(func_on_segment_(2 * pos + 1, left, (left + right) / 2, left_border, right_border),
                func_on_segment_(2 * pos + 2, (left + right) / 2 + 1, right, left_border, right_border));
}

void SegmentTree::update(long long pos, long long val) {
    tree_[pos + x_ - 1] = val;
    auto v = pos;
    while (v != 0){
        v = (v - 1) / 2;
        tree_[v] = func(tree_[2 * v + 1], tree_[2 * v + 2]);
    }
}

// для rmq
long long SegmentTree::get_(long long pos, long long l, long long r) {
    if (set_ind_[pos]){
        return upd_[pos];
    }

    return tree_[pos] + upd_[pos];
}

void SegmentTree::push_(long long pos, long long l, long long r) {
    if (l == r){
        if (set_ind_[pos]){
            tree_[pos] = upd_[pos];
            set_ind_[pos] = false;
        }
        else{
            tree_[pos] += upd_[pos];
        }
        upd_[pos] = 0;
    }
    else{
        if (set_ind_[pos]){
            upd_[2 * pos + 1] = upd_[pos];
            upd_[2 * pos + 2] = upd_[pos];
            set_ind_[2 * pos + 1] = true;
            set_ind_[2 * pos + 2] = true;
        }
        else{
            upd_[2 * pos + 1] += upd_[pos];
            upd_[2 * pos + 2] += upd_[pos];
        }
        auto mid = (l + r) / 2;
        upd_[pos] = 0;
        tree_[pos] = func(get_(2 * pos + 1, l, mid), get_(2 * pos + 2, mid + 1, r));
    }


}

void SegmentTree::update_(long long pos, long long left, long long right, long long a, long long b, long long val) {

    if (set_ind_[pos] || upd_[pos]){
        push_(pos, left, right);
    }

    if (left > right || right < a || b < left || left > real_size_){
        return;
    }
    if (a <= left && right <= b){
        upd_[pos] = val;
        set_ind_[pos] = false;
        return;
    }

    auto mid = (left + right) >> 1;
    auto left_child = (pos << 1) + 1;
    update_(left_child, left, mid, a, b, val);
    update_(left_child + 1, mid + 1, right, a, b, val);
    tree_[pos] = func(get_(left_child, left, mid), get_(left_child + 1, mid + 1, right));
}


void SegmentTree::set_(long long pos, long long left, long long right, long long a, long long b, long long val) {

    if (set_ind_[pos] || upd_[pos]){
        push_(pos, left, right);
    }

    if (left > right || right < a || b < left || left > real_size_){
        return;
    }
    if (a <= left && right <= b){
        upd_[pos] = val;
        set_ind_[pos] = true;
        return;
    }

    auto mid = (left + right) >> 1;
    auto left_child = (pos << 1) + 1;
    update_(left_child, left, mid, a, b, val);
    update_(left_child + 1, mid + 1, right, a, b, val);
    tree_[pos] = func(get_(left_child, left, mid), get_(left_child + 1, mid + 1, right));
}

void SegmentTree::set_(long long pos, long long val) {
    if (pos >= tree_.size()){
        return;
    }

    tree_[pos] = val;
    upd_[pos] = 0;
    set_((pos << 1) + 1, val);
    set_((pos << 1) + 2, val);
}

long long Min(const std::vector<long long> &a, long long i, long long j) {
    long long res = INT64_MAX;
    for (size_t k = i; k <= j; ++k){
        if (a[k] < res){
            res = a[k];
        }
    }
    return res;
}

void Set(std::vector<long long> &a, long long i, long long j, long long val) {
    for (size_t k = i; k <= j; ++k){
        a[k] = val;
    }
}

void Add(std::vector<long long> &a, long long i, long long j, long long val) {
    for (size_t k = i; k <= j; ++k){
        a[k] += val;
    }
}

struct info{
    std::string command;
    unsigned long long i;
    unsigned long long j;
};

void test() {
    std::vector<long long> a(100000, 0);
    size_t p = 0;
    for (auto &i:a){
        i = p++;
    }

    SegmentTree segmentTree(a);

    for (long long i = segmentTree.x_ - 1, j = 0; i < a.size(); ++i, ++j){
        assert(a[j] == segmentTree.tree_[i]);
    }
    unsigned long long u = 1, v = 9;
    std::vector<info> inf;
    p = 2;
    while (p < 1000000){
        u = ((17 * u + 2 * (p + 1)) % a.size());
        v = ((13 * v + 5 * (p + 1)) % a.size());
        if (u > v){
            std::swap(u, v);
        }
        if (p % 2 == 0){
            Add(a, u, v, p % 500);
            segmentTree.add(u, v, p % 500);
            //std::cout << "add " << u << " " << v << " " << p % 500 << std::endl;
            inf.push_back({"add", u, v});
        }
        else if (p % 3 == 0){
            Set(a, u, v, p % 1000);
            segmentTree.set(u, v, p % 1000);
            //std::cout << "set " << u << " " << v << " " << p % 1000 << std::endl;
            inf.push_back({"set", u, v});
        }
        else{
            auto ans = Min(a, u, v);
            auto my = segmentTree.min(u, v);

            if (my != ans){
                std::cout << "----- " << std::endl;
                for (auto &p: inf){
                    if (p.i <= u && u <= p.j){
                        std::cout << p.command << " " << p.i << " " << p.j << std::endl;
                    }
                }
                std::cout << "wrong ans " << my << " " << ans << std::endl;
            }
        }
        ++p;
    }

}

int main() {
    //test();
    //std::cout<<"done"<<std::endl;
    std::ios::sync_with_stdio(false), std::cin.tie(nullptr), std::cout.tie(nullptr);
    int n;
    std::cin >> n;
    std::vector<long long> a(n, 0);
    for (auto &i:a){
        std::cin >> i;
    }

    std::string command;
    long long i, j, x;
    SegmentTree segmentTree(a);
    while (std::cin >> command){
        if (command == "min"){
            std::cin >> i >> j;
            auto min = segmentTree.min(i - 1, j - 1);
            std::cout << min << "\n";
        }
        else if (command == "add"){
            std::cin >> i >> j >> x;
            segmentTree.add(i - 1, j - 1, x);
        }
        else if (command == "set"){
            std::cin >> i >> j >> x;
            segmentTree.set(i - 1, j - 1, x);
        }
        //std::cout<<std::endl;
    }

    return 0;
}
