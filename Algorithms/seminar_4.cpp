//
// Created by Ovanes on 09.11.2019.
//
// 6_4, деструктор тоже не рекурсивнно можно в обходе удалить
// 7_3 было quict select. Тут за логарифм можно, но надо хранить дополнительную информацию
// можно хранить количество узлов в поддереве (лист-1), когда добавляется  элемент необходимо одовить количество узлов
// на всем пусти к этому пути
//при сплитов и мержей каунты необходимо обновить

// при повороте прямо в функции поворота можно написать
// 89057360730

#include <iostream>
#include <string>
#include <vector>
#include <cassert>
//ранункулюсы мисти баблс


class HashTable{
public:
    explicit HashTable(size_t initial_size);

    ~HashTable();

    HashTable(const HashTable &) = delete;

    HashTable(HashTable &&) = delete;

    HashTable &operator=(const HashTable &) = delete;

    HashTable &operator=(HashTable &&) = delete;

    bool Has(const std::string &key) const;

    bool Add(const std::string &key);

    bool Remove(const std::string &key);

private:
    struct HashTableNode{
        std::string key;
        HashTableNode *next = nullptr;

        HashTableNode(std::string key_) : key(std::move(key_)){};
    };

    std::vector<HashTableNode *> table;
};

HashTable::HashTable(size_t initial_size) : table(initial_size, nullptr){

}

HashTable::~HashTable(){
    for(HashTableNode *head: table){

        while(head != nullptr){
            HashTableNode *next = head->next;
            delete head;
            head = next;
        }
    }

}

bool HashTable::Has(const std::string &key) const{
    //методом горнера
    assert(!key.empty());

    const size_t hash = key[0] % table.size();
    const HashTableNode *node = table[hash];
    for(HashTableNode *head = table[hash]; head; head = head->next){
        if(head->key == key)
            return true;
    }
    return false;
}

bool HashTable::Add(const std::string &key){
    assert(!key.empty());

    const size_t hash = key[0] % table.size();
    const HashTableNode *node = table[hash];
    for(HashTableNode *head = table[hash]; head; head = head->next){
        if(head->key == key)
            return false;
    }
    HashTableNode *new_node = new HashTableNode(key);
    new_node->next = table[hash];
    table[hash] = new_node;
    return true;
}

bool HashTable::Remove(const std::string &key){
    assert(!key.empty());

    const size_t hash = key[0] % table.size();
    HashTableNode *&head = table[hash];
    if(!head) return false;

    if(head->key == key){
        HashTableNode *to_delete = head;
        head = head->next;
        delete to_delete;
        return true;
    }
    HashTableNode *parent = head;
    for(; parent->next != nullptr; parent = parent->next){
        if(parent->next->key == key){
            auto to_delete = parent->next;
            parent->next = parent->next->next;
            delete to_delete;
            return true;
        }
    }

    return false;
}

int main(){
    HashTable hashTable(8);
    char command = ' ';
    std::string value;
    while(std::cin >> command >> value){
        switch(command){
            case '?':
                std::cout << (hashTable.Has(value) ? "OK" : "FAIL");
                std::cout << std::endl;
                break;
            case '+':
                std::cout << (hashTable.Add(value) ? "OK" : "FAIL");
                std::cout << std::endl;
                break;
            case '-':
                std::cout << (hashTable.Remove(value) ? "OK" : "FAIL");
                std::cout << std::endl;
                break;
        }
    }
    return 0;
}
