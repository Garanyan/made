
2 - куча sift up/ down, скользящий максимум
3 - merge sort, первые k элементов длинной последовательнсоти
4 - сортировка partition, случайный опорный элемент

6 - level order очередь
7 -
8 - хеш таблица
9 - 
11_2 - количество различных кратчайших путей в грайфе
16 - Поиск подстроки префикс-функции (алгоритм Кнута-Морриса-Пратта

Обход в глубину DFS графа
помечаем посещенные и идем по всем соседям


Обход в ширину BFS гарфа
очередь и помечаем посещенные
---
advanced algo

3А - разложение на множители
4Е - факторизация ро алгоритм Полларда 
5B - RSQ Fenwick tree
5C - RMQ Segment tree
5D - Разреженные таблицы RMQ
6D - LCA общий предок в дереве
8A - максимальный поток диниц
8B - минимальный разрез
8С - 
9А  - Беллман Форд максимальный поток минимальной стоимости
9B - Задача о назначениях
11 -  Алгоритм Аримуры — Арикавы — Касаи — Ли — Пака
12 - 
13 - геометрия
14 - fft


///***
class ThreadPool {
public:
    ThreadPool(size_t);
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;
    ~ThreadPool();
private:
    // need to keep track of threads so we can join them
    std::vector< std::thread > workers;
    // the task queue
    std::queue< std::function<void()> > tasks;
    
    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};
 
// the constructor just launches some amount of workers
inline ThreadPool::ThreadPool(size_t threads)
    :   stop(false)
{
    for(size_t i = 0;i<threads;++i)
        workers.emplace_back(
            [this]
            {
                for(;;)
                {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock,
                            [this]{ return this->stop || !this->tasks.empty(); });
                        if(this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }

                    task();
                }
            }
        );
}

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // don't allow enqueueing after stopping the pool
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task](){ (*task)(); });
    }
    condition.notify_one();
    return res;
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &worker: workers)
        worker.join();
}

///---


 2 especially 0 - 10, 15, 16, 20, 30 - 32, 64
 2^4 = 16
 2^5 = 32
 2^6 = 64
 2^8 = 128
 2^9 = 512
 2^10 = 1024
 2^11 = 2048
 2^15 = 32768
 2^16 = 65536
 2^32 = 4294967296

 Сортировки

Сортировка с лямбда

#include <algorithm>
#include <functional>
O(N·log(N))
std::sort(s.begin(), s.end(), [](int a, int b) {
        return a > b;
    });

---
Multimap

#include <map>
std::multimap<std::string, std::string> m;
m.emplace("d", "ddd");

---
Битовые маски

	size_t getBit(size_t mask, size_t i) {
        return (mask >> i) & 1;
    }
    
    vector<vector<int>> subsets(vector<int>& nums) {
        vector<vector<int>> result;
        
        for (size_t mask = 0; mask < 1 << nums.size(); ++mask) {
            vector<int> subset;
            for (size_t i = 0; i < nums.size(); ++i) {
                if (getBit(mask, i) == 1) {
                    subset.emplace_back(nums[i]);
                }
            }
            result.push_back(subset);
        }
        
        return result;
    }
---

Тесты
Edge case
---

Алгоритм Флойда поиска цикла

def floyd(f, x0):
    # Основная часть алгоритма: находим повторение x_i = x_2i.
    # Заяц движется вдвое быстрее черепахи,
    # и расстояние между ними увеличивается на единицу от шага к шагу.
    # Однажды они окажутся внутри цикла, и тогда расстояние между ними
    # будет делиться на λ.
    tortoise = f(x0) # f(x0) является элементом, следующим за x0.
    hare = f(f(x0))
    while tortoise != hare:
        tortoise = f(tortoise)
        hare = f(f(hare))
  
    # В этот момент позиция черепахи ν, 
    # которая равна расстоянию между черепахой и зайцем,
    # делится на период λ. Таким образом, заяц, двигаясь 
    # по кольцу на одну позицию за один раз, 
    # и черепаха, опять начавшая движение со стартовой точки x0 и
    # приближающаяся к кольцу, встретятся в начале кольца
    # Находим позицию μ встречи.    
    mu = 0
    tortoise = x0
    while tortoise != hare:
        tortoise = f(tortoise)
        hare = f(hare)   # Заяц и черепаха двигаются с одинаковой скоростью
        mu += 1
 
    # Находим длину кратчайшего цикла, начинающегося с позиции x_μ
    # Заяц движется на одну позицию вперёд, 
    # в то время как черепаха стоит на месте.
    lam = 1
    hare = f(tortoise)
    while tortoise != hare:
        hare = f(hare)
        lam += 1
 
    return lam, mu
---

Бинарный поиск 

int binarySearch(int arr[], int l, int r, int x) 
{ 
    if (r >= l) { 
        int mid = l + (r - l) / 2; 
  
        // If the element is present at the middle 
        // itself 
        if (arr[mid] == x) 
            return mid; 
  
        // If element is smaller than mid, then 
        // it can only be present in left subarray 
        if (arr[mid] > x) 
            return binarySearch(arr, l, mid - 1, x); 
  
        // Else the element can only be present 
        // in right subarray 
        return binarySearch(arr, mid + 1, r, x); 
    } 
  
    // We reach here when element is not 
    // present in array 
    return -1; 
} 

int binarySearch(int array[], int x, int low, int high) {
  
    // Repeat until the pointers low and high meet each other
  while (low <= high) {
    int mid = low + (high - low) / 2;

    if (array[mid] == x)
      return mid;

    if (array[mid] < x)
      low = mid + 1;

    else
      high = mid - 1;
  }

  return -1;
}
---
  void reverse()
    {
        // Initialize current, previous and
        // next pointers
        Node* current = head;
        Node *prev = NULL, *next = NULL;
 
        while (current != NULL) {
            // Store next
            next = current->next;
 
            // Reverse current node's pointer
            current->next = prev;
 
            // Move pointers one position ahead.
            prev = current;
            current = next;
        }
        head = prev;
    }

