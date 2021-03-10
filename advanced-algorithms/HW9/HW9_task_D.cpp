/*
 *
    D. Автоматное программирование
ограничение по времени на тест 5 секунд
ограничение по памяти на тест 256 мегабайт
ввод стандартный ввод
вывод стандартный вывод

 В один замечательный день в компанию «X» завезли k автоматов. И не простых автоматов,
 а автоматов-программистов! Это был последний неудачный шаг перед переходом на андроидов-программистов,
 но это уже совсем другая история.

 В компании сейчас n задач, для каждой из которых известно время начала ее выполнения si,
 длительность ее выполнения ti и прибыль компании от ее завершения ci. Любой автомат может
 выполнять любую задачу, ровно одну в один момент времени. Если автомат начал выполнять задачу,
 то он занят все моменты времени с si по si+ti−1 включительно и не может переключиться на другую задачу.

 Вам требуется выбрать набор задач, которые можно выполнить с помощью этих k автоматов и который принесет
 максимальную суммарную прибыль.

 Входные данные
 В первой строке записаны два целых числа n и k (1≤n≤1000, 1≤k≤50 ) — количество задач и
 количество автоматов, соответственно.

 В следующих n
 строках через пробелы записаны тройки целых чисел si,ti,ci (1≤si,ti≤10^9, 1≤ci≤10^6),
 si — время начала выполнения i-го задания, ti — длительность i-го задания, а ci — прибыль от его выполнения.

 Выходные данные
 Выведите n
 целых чисел x1,x2,…,xn. Число xi должно быть равно 1, если задачу i следует выполнить, и 0 в противном случае.
 Если оптимальных решений несколько, то выведите любое из них.
 */

/*
Самое главное было — увидеть граф, где вершины = задания, множества заданий, выполняемые автоматами =
 вершиннонепересекающиеся пути, а ребра можно было строить двумя способами (см. ниже).

Как сделать пути вершиннонепересекающимися? Раздвоить вершины.

Как добавить стоимость? Между двумя половинками вершины должна быть стоимость  - c i.

Первый способ, граф G 1 : вершины A, B соединены ребром, если после задания A можно успеть выполнить задание
 B тем же автоматом. Тогда ребер O(N 2).

Второй способ, граф G 2 : все вершины упорядочены по s i из вершины i ведут два ребра — в i + 1
 (не берем i задание, пробуем взять следующее) и в вершину с минимальным
 j : s j ≥ s i + t i (берем задание, переходим к минимальному из возможных следующих). Тогда ребер O(N).

Если вы пользовались первым способом и для поиска пути реализовали одну из вариаций алгоритма Форда-Беллмана,
 то вы могли получить TL. Собственно пока из-за того, что на java все медленно, TL не подняли, все Форд-Беллманы
 кроме одного особенного получали заслуженный TL.

Чтобы в G 1 использовать Дейкстру с потенциалами, удобнее всего было заметить, что граф ацикличен и первый
 раз расстояния посчитать за O(E).
*/

#include <iostream>
#include <vector>
#include <zconf.h>
#include <deque>
#include <algorithm>

struct edge {
    int a, b, capacity;
    long long flow, weight;
    int reversed;
    int task_id;
};

class AssignmentProblem {
public:
    AssignmentProblem(int n, int task_num);

    void AddEdge(const int a, const int b, const int cap, int weight, int task_id);

    long long Solve(bool pre_FB);

    void PrintAssignment();

    void setSink(int i);

private:

    void dijkstra(int s);

    void LevitAlgorithm();

    std::vector<std::vector<edge>> graph;
    std::vector<long long> d;
    std::vector<long long> potential;
    std::vector<edge *> p;

    int source = 0;
    int sink;
    int n;
    int tusk_num;

};

void AssignmentProblem::AddEdge(const int a, const int b, const int cap, int weight, int task_id) {
    edge e1 = {a, b, cap, 0, weight, static_cast<int>(graph[b].size()), task_id};
    edge e2 = {b, a, 0, 0, -weight, static_cast<int>(graph[a].size()), task_id};

    graph[a].push_back(e1);
    graph[b].push_back(e2);
}

void AssignmentProblem::dijkstra(int s) {
    d = std::vector<long long>(graph.size(), LONG_LONG_MAX);
    std::vector<bool> used(graph.size(), false);

    d[s] = 0;
    for (int i = 0; i < graph.size(); i++) {
        auto v = -1;
        for (int j = 0; j < graph.size(); j++) {
            if (!used[j] && (v == -1 || d[j] < d[v])) {
                v = j;
            }
        }

        if (d[v] == LONG_LONG_MAX) {
            break;
        }

        used[v] = true;

        for (int id = 0; id < graph[v].size(); id++) {
            auto &e = graph[v][id];
            long long len = e.weight + potential[e.a] - potential[e.b];//вес с учетом потенциала

            // если еще можно пустить поток с учетом новых весов
            if (e.flow < e.capacity && d[v] + len < d[e.b]) {
                d[e.b] = d[v] + len;
                p[e.b] = &e;
            }
        }
    }
}

AssignmentProblem::AssignmentProblem(int N, int task_num) {
    n = N;
    graph.resize(n);
    p.resize(n);
    potential.resize(n, 0);
    sink = n - 1;
    tusk_num = task_num;
}

void AssignmentProblem::LevitAlgorithm() {
    std::deque<int> q;
    d = std::vector<long long>(graph.size(), LONG_LONG_MAX);
    std::vector<int> state(graph.size(), 2); // хранит номер множества, к которому относится вершина
    d[source] = 0;
    state[source] = 0;
    q.push_back(source);

    while (!q.empty()) {
        int u = q.front();
        q.pop_front();
        state[u] = 0;

        for (int i = 0; i < graph[u].size(); ++i) {
            auto &e = graph[u][i];
            if (e.flow < e.capacity && d[u] + e.weight < d[e.b]) {
                d[e.b] = d[u] + e.weight;
                potential[e.b] = d[e.b];

                if (state[e.b] == 2) {
                    q.push_back(e.b);
                } else if (state[e.b] == 0) {
                    q.push_front((e.b));
                }

                p[e.b] = &e; // ребро в кратчайшем пути от вершины
                state[e.b] = 1;
            }
        }
    }
}

long long AssignmentProblem::Solve(bool pre_FB) {

    // Если есть отрицательные веса
    if (pre_FB) {
        LevitAlgorithm();
    }

    long long min_cost = 0;
    while (true) {

        // пересчитываем d[]
        dijkstra(0);

        // сток недостижим
        if (d[sink] == LONG_LONG_MAX) {
            break;
        }

        long long adjust_flow = LONG_LONG_MAX;
        for (int u = sink; u != source; u = p[u]->a) {
            // идем по пути от стока к источнику, считаем на сколько можем увеличить поток
            auto e1 = p[u];
            adjust_flow = std::min(adjust_flow, static_cast<long long>(e1->capacity - e1->flow));
        }

        for (int u = sink; u != source; u = p[u]->a) {
            // увеличиваем поток вдоль пути
            auto e1 = p[u];
            auto e2 = &graph[e1->b][e1->reversed];

            e1->flow += adjust_flow;
            e2->flow -= adjust_flow;

            min_cost += adjust_flow * e1->weight;
        }

        // Обновляем потенциалы
        for (int i = 0; i < graph.size(); i++) {
            potential[i] = potential[i] + d[i];
        }
    }

    return min_cost;
}

void AssignmentProblem::setSink(int i) {
    sink = i;
}

void AssignmentProblem::PrintAssignment() {
    std::vector<bool> jud(tusk_num, false);
    for (auto &i : graph) {
        for (auto e: i) {
            if (e.flow > 0 && e.task_id != -1) {
                jud[e.task_id] = true;
            }
        }
    }

    for (auto i : jud) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

struct Task {
    int start_time;
    int end_time;
    int cost;
    int task_id;
};

int main() {
    int task_num = 0, automate_num = 0;
    std::cin >> task_num >> automate_num;
    int duration;

    std::vector<Task> tasks;
    for (int i = 0; i < task_num; ++i) {
        Task task;
        std::cin >> task.start_time >> duration >> task.cost;
        task.task_id = i;
        task.end_time = task.start_time + duration - 1;
        tasks.push_back(task);
    }
    //Сортировка заданий по времени начала
    std::sort(tasks.begin(), tasks.end(),
              [](const Task &a, const Task &b) -> bool {
                  return a.start_time < b.start_time;
              });

    AssignmentProblem solver(tasks.size() + 2, task_num);

    solver.AddEdge(0, 1, automate_num, 0, -1);
    for (int i = 0; i < tasks.size(); i++) {
        // Ребро в следующее по порядку задание, если пропускается текущее
        solver.AddEdge(i + 1, i + 2, automate_num, 0, -1);
        auto next_task = std::find_if(tasks.begin(), tasks.end(),
                                      [&](Task a) { return tasks[i].end_time < a.start_time; });
        if (next_task != tasks.end()) {
            // ребро в следующее задание, которое можно сделать выполнив текущее
            solver.AddEdge(i + 1, next_task - tasks.begin() + 1, 1, -tasks[i].cost, tasks[i].task_id);
        } else {
            // ребро в сток
            solver.AddEdge(i + 1, tasks.size() + 1, 1, -tasks[i].cost, tasks[i].task_id);
        }
    }

    solver.Solve(true);
    solver.PrintAssignment();

    return 0;

}
