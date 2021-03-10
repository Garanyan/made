//
// Created by Ovanes Garanyan on 6/24/20.
//
#include <iostream>
#include <utility>
#include <vector>
#include <memory>

struct Node{
    int value;
    std::shared_ptr<Node> previous;

    Node(int v, Node* p) : value(v), previous(p) {

    };
    Node(int v, std::shared_ptr<Node>  p) : value(v), previous(std::move(p)) {

    };
};

class PersistentStack{
public:
    PersistentStack();

    explicit PersistentStack(int size);

    ~PersistentStack();

    void push(int stack_version, int value);

    int pop(int stack_version);

private:
    std::vector<Node*> versions;
    int stack_size;
};

PersistentStack::PersistentStack(int size) {
    versions.clear();
    versions.reserve(size);
    for(size_t i = 0; i < size; ++i){
        versions.emplace_back(new Node(0, nullptr));
    }
    stack_size = 1;
}

PersistentStack::PersistentStack() : PersistentStack(8) {
}

PersistentStack::~PersistentStack() {
//    for (auto& n: versions){
//        if(n)
//        {
//            if(n->previous)
//            {
//                delete n->previous;
//                n->previous = nullptr;
//            }
//            delete n;
//            n = nullptr;
//        }
//    }
}

void PersistentStack::push(int stack_version, int value) {
    versions[stack_size++] = new Node(value, versions[stack_version]);
}

int PersistentStack::pop(int stack_version) {
    if(stack_version >= stack_size){
        stack_version = stack_size-1;
    }
    auto res = versions[stack_version]->value;
    versions[stack_size++] =  versions[stack_version]->previous;
    return res;
}

int main() {
    int n;
    std::cin >> n;
    PersistentStack s;
    int t, m;
    for (size_t i = 0; i < n; ++i){
        std::cin >> t >> m;
        if (m == 0){
            std::cout<<s.pop(t)<<std::endl;
        }
        else{
            s.push(t, m);
        }
    }
    return 0;
}
