//
// Created by Garanyan Ovanes (Ovanes-X) on 23.11.2019.
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

    ~HashTable() = default;

    HashTable(const HashTable &) = delete;

    HashTable(HashTable &&) = delete;

    HashTable &operator=(const HashTable &) = delete;

    HashTable &operator=(HashTable &&) = delete;

    bool Has(const std::string &key) const;

    bool Add(const std::string &key);

    bool Remove(const std::string &key);

private:
    unsigned int firstHash(const std::string &str) const;

    unsigned int secondHash(const std::string &str) const;

    static unsigned int hornerHash(const char *str, int a, size_t size);

    void doubleHashTable();

    std::vector<std::string> table;
    std::vector<bool> removed;
    size_t filled_size_;
};

HashTable::HashTable(size_t initial_size) : table(initial_size, ""), removed(initial_size, false), filled_size_(0){}

bool HashTable::Has(const std::string &key) const{
    assert(!key.empty());

    size_t startHash = firstHash(key);
    size_t shift = secondHash(key);
    for(size_t hash = firstHash(key), i = 1;
        (!table[hash].empty() || removed[hash]) && i <= table.size(); hash = (startHash + i * shift) % table.size(), i++){
        if(table[hash] == key){
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
    size_t shift = secondHash(key);
    size_t i = 1;

    size_t new_node_hash = table.size();
    const size_t startHash = hash;
    do{
        if(table[hash].empty()){
            if(new_node_hash == table.size()){
                new_node_hash = hash;
            }
            if(!removed[hash])
                break;
        }
        else if(table[hash] == key){
            return false;
        }
        hash = (startHash + i * shift) % table.size();
        ++i;
    } while(hash != startHash && i <= table.size());

    if(new_node_hash != table.size()){
        table[new_node_hash] = key;
        ++filled_size_;
        return true;
    }

    return false;
}

bool HashTable::Remove(const std::string &key){
    assert(!key.empty());

    size_t startHash = firstHash(key);
    size_t shift = secondHash(key);
    for(size_t hash = startHash, i = 1;
        i < table.size(); hash = (startHash + i * shift) % table.size(), i++){
        if(table[hash].empty() && !removed[hash])
            break;

        if(table[hash] == key){
            table[hash].clear();
            --filled_size_;
            removed[hash] = true;
            return true;
        }
    }

    return false;
}

void HashTable::doubleHashTable(){
    auto prevTable = std::move(table);
    std::vector<std::string> newTable(prevTable.size() * 4, "");
    removed.resize(newTable.size());
    std::fill(removed.begin(), removed.end(), false);
    table = std::move(newTable);

    for(size_t i = 0; i < prevTable.size(); ++i){
        if(!prevTable[i].empty()){
            Add(prevTable[i]);
            prevTable[i].clear();
        }
    }
}

unsigned int HashTable::hornerHash(const char *str, int a, size_t size) {
    unsigned int hash = 0;
    for(; *str != 0; ++str)
        hash = (hash * a + *str) % size;
    return hash;
}

unsigned int HashTable::firstHash(const std::string &str) const{
    return hornerHash(str.data(), 31, table.size());
}

unsigned int HashTable::secondHash(const std::string &str) const{
    return (str[0] * table.size() * str.length() - 1) % table.size();
}

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
