//
// Created by Ovanes on 23.11.2019.
//

//Задача 8. Хеш-таблица (10 баллов)
//Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
// Хранимые строки непустые и состоят из строчных латинских букв.
//Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
//Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае,
// когда коэффициент заполнения таблицы достигает 3/4.
//Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и
// проверки принадлежности данной строки множеству.

//1_2. Для разрешения коллизий используйте двойное хеширование.
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

class HashTable{
public:
    explicit HashTable(size_t initial_size = 8);

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
        //HashTableNode *next = nullptr;

        HashTableNode(std::string key_) : key(std::move(key_)){}
    };

    unsigned int firstHash(const std::string &str) const;

    unsigned int secondHash(const std::string &str) const;

    unsigned int hornerHash(const char *str, int a) const;

    void doubleHashTable();

    std::vector<HashTableNode *> table;
    size_t filled_size_;
};

HashTable::HashTable(size_t initial_size) : table(initial_size, nullptr), filled_size_(0){}

HashTable::~HashTable(){
    for(HashTableNode *head : table){
        delete head;
    }
}

bool HashTable::Has(const std::string &key) const{
    assert(!key.empty());

    for(size_t hash = firstHash(key); table.at(hash) != nullptr; hash = (hash+secondHash(key))%table.size()){
        if(table.at(hash)->key == key){
            return true;
        }
    }

    return false;
}

bool HashTable::Add(const std::string &key){
    assert(!key.empty());

    if(filled_size_ >= (table.size() * 3) / 4){
        doubleHashTable();
    }

    size_t hash = firstHash(key);
    const size_t startHash = hash;
    do{
        if(table[hash] == nullptr){
            HashTableNode *new_node = new HashTableNode(key);
            //new_node->next = table[startHash];
            table[hash] = new_node;
            ++filled_size_;
            return true;
        }
        else if(table[hash]->key == key){
            return false;
        }
        hash = (hash+secondHash(key))%table.size();
    } while(hash != startHash);

    return false;
}

bool HashTable::Remove(const std::string &key){
    assert(!key.empty());

    for(size_t hash = firstHash(key); table.at(hash) != nullptr; hash = (hash+secondHash(key))%table.size()){
        if(table[hash]->key == key){
            delete table[hash];
            table[hash] = nullptr;
            --filled_size_;
            return true;
        }
    }

    return false;
}

void HashTable::doubleHashTable(){
    //std::cout<<"double tree"<<std::endl;
    auto prevTable = table;
    std::vector<HashTableNode *> newTable(table.size() * 2, nullptr);
    table = newTable;

    for(size_t i = 0; i < prevTable.size(); ++i){
        if(prevTable[i] != nullptr){
            Add(prevTable[i]->key);
            delete prevTable[i];
        }
    }
}

unsigned int HashTable::hornerHash(const char *str, int a) const{
    unsigned int hash = 0;
    for(; *str != 0; ++str)
        hash = (hash * a + *str) % table.size();
    return hash;
}

unsigned int HashTable::firstHash(const std::string &str) const{
    unsigned int hash = hornerHash(str.data(), 31);
    //std::cout<<"1 hash " + str+" :"<<hash<<std::endl;
    return hash;
}

unsigned int HashTable::secondHash(const std::string &str) const{
    unsigned int hash = hornerHash(str.data(), 19);
    //std::cout<<"2 hash " + str+" :"<<hash<<std::endl;

    return (str[0] * table.size() - 1) % table.size();
}


//stdin
//+ hello + bye ? bye + bye - bye ? bye ? hello + hello + bye ? bye + bye - bye ? bye ? hello ? ova + ova + ova ? ova ? ova  + asd + asd

int main(){
    HashTable table;
    char command = ' ';
    std::string value;
    while(std::cin >> command >> value){
        switch(command){
            case '?':
                std::cout << (table.Has(value) ? "OK" : "FAIL") << std::endl;
                break;
            case '+':
                std::cout << (table.Add(value) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (table.Remove(value) ? "OK" : "FAIL") << std::endl;
                break;
        }
    }
    return 0;
}