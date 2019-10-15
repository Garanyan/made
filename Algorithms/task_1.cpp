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

struct queue
{
	size_t size;
	size_t buffer_size;
	int* data;

	queue(): size(0), buffer_size(8)
	{
		data = new int[buffer_size];
	}

	~queue()
	{
		delete[] data;
	}

	void double_buffer()
	{
		int* new_data = new int[2*buffer_size];//check null ?

		for(size_t i=0; i < size; ++i)
		{
			new_data[i] = data[i];
		}

		delete[] data;
		data = new_data;
	}

	void push_back(int value)
	{
		if(size >= buffer_size)
		{
			double_buffer();
		}

		data[size] = value;
		++size;
	}

	int pop_front()
	{
		if(size > 0)
		{
			size--;
			return data[size]; //fix
		}
        return -1;
	}
};

int main(int argc, char** argv)
{
	std::string result = "YES";

	int n = 0;
	std::cin>>n;
	int command = -1;
	int value = -1;
    queue v;
	for(size_t i=0; i< n; ++i)
    {
        std::cin>>command;
        std::cin>>value;

        switch (command)
        {
//            case 1:
//                break;
            case 2:
                if(value != v.pop_front())
                {
                    result = "NO";
                }
                break;
            case 3:
                v.push_back(value);
                break;
//            case 4:
//                break;
        }
    }

	std::cout<<result;
	return 0;
}