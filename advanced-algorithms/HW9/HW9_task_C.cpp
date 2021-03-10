/*
 *
C. План эвакуации
ограничение по времени на тест 2 секунды
ограничение по памяти на тест 256 мегабайт
ввод стандартный ввод
вывод стандартный вывод

 В городе есть муниципальные здания и бомбоубежища, которые были специально построены для эвакуации служащих
 в случае ядерной войны. Каждое бомбоубежище имеет ограниченную вместительность по количеству людей,
 которые могут в нем находиться. В идеале все работники из одного муниципального здания должны были бы
 бежать к ближайшему бомбоубежищу. Однако, в таком случае, некоторые бомбоубежища могли бы переполниться,
 в то время как остальные остались бы наполовину пустыми.

 Чтобы разрешить эту проблему Городской Совет разработал специальный план эвакуации.
 Вместо того, чтобы каждому служащему индивидуально приписать, в какое бомбоубежище
 он должен бежать, для каждого муниципального здания определили, сколько служащих из
 него в какое бомбоубежище должны бежать. Задача индивидуального распределения была
 переложена на внутреннее управление муниципальных зданий.

 План эвакуации учитывает количество служащих в каждом здании — каждый служащий должен
 быть учтен в плане и в каждое бомбоубежище может быть направлено количество служащих,
 не превосходящее вместимости бомбоубежища.

 Городской Совет заявляет, что их план эвакуации оптимален в том смысле, что суммарное
 время эвакуации всех служащих города минимально.

 Мэр города, находящийся в постоянной конфронтации с Городским Советом, не слишком то
 верит этому заявлению. Поэтому он нанял Вас в качестве независимого эксперта для проверки
 плана эвакуации. Ваша задача состоит в том, чтобы либо убедиться в оптимальности плана
 Городского Совета, либо доказать обратное, представив в качестве доказательства другой план
 эвакуации с меньшим суммарным временем для эвакуации всех служащих.

 Карта города может быть представлена в виде квадратной сетки. Расположение муниципальных
 зданий и бомбоубежищ задается парой целых чисел, а время эвакуации из муниципального здания
 с координатами (X i, Y i) в бомбоубежище с координатами (P j, Q j)
 составляет D ij = |X i - P j| + |Y i - Q j| + 1 минут.

Входные данные
 Входной файл содержит описание карты города и плана эвакуации,
 предложенного Городским Советом. Первая строка входного файла содержит два целых числа
 N (1 ≤ N ≤ 100) и M (1 ≤ M ≤ 100), разделенных пробелом. N — число муниципальных зданий
 в городе (все они занумерованы числами от 1 до N), M — число бомбоубежищ (все они занумерованы числами от 1 до M).

 Последующие N строк содержат описания муниципальных зданий. Каждая строка содержит
 целые числа X i, Y i и B i, разделенные пробелами, где X i, Y i ( - 1000 ≤ X i, Y i ≤ 1000) — координаты здания,
 а B i (1 ≤ B i ≤ 1000) — число служащих в здании.

 Описание бомбоубежищ содержится в последующих M строках. Каждая строка содержит
 целые числа P j, Q j и C j, разделенные пробелами, где P j, Q j ( - 1000 ≤ P j, Q j ≤ 1000) — координаты бомбоубежища,
 а C j (1 ≤ C j ≤ 1000) — вместимость бомбоубежища.

 В последующих N строках содержится описание плана эвакуации. Каждая строка представляет собой
 описание плана эвакуации для отдельного здания. План эвакуации из i-го здания состоит из M целых чисел E ij,
 разделенных пробелами. E ij (0 ≤ E ij ≤ 10000) — количество служащих, которые должны эвакуироваться из i-го
 здания в j-е бомбоубежище.

 Гарантируется, что план, заданный во входном файле, корректен.

Выходные данные
 Если план эвакуации Городского Совета оптимален, то выведите одно слово OPTIMAL.
 В противном случае выведите на первой строке слово SUBOPTIMAL, а в последующих
 N строках выведите Ваш план эвакуации (более оптимальный) в том же формате, что
 и во входном файле. Ваш план не обязан быть оптимальным, но должен быть лучше
 плана Городского Совета.
 * */


#include <iostream>
#include <vector>
#include <zconf.h>
#include <deque>

struct edge {
    int a, b, capacity;
    long long flow, weight;
    int reversed;
};

class AssignmentProblem {
public:
    AssignmentProblem(int n, int m);

    void AddEdge(const int a, const int b, const int cap, int weight);

    long long Solve(bool pre_FB);

    void PrintAssignment();

    void setSink(int i);

private:

    void dijkstra(int v);

    void LevitAlgorithm();

    std::vector<std::vector<edge>> graph;
    std::vector<long long> d;
    std::vector<long long> potential;
    std::vector<edge *> p;

    int source = 0;
    int sink;
    int n;
    int m;

};

void AssignmentProblem::AddEdge(const int a, const int b, const int cap, int weight) {
    edge e1 = {a, b, cap, 0, weight, static_cast<int>(graph[b].size())};
    edge e2 = {b, a, 0, 0, -weight, static_cast<int>(graph[a].size())};

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

AssignmentProblem::AssignmentProblem(int N, int M) {
    n = N;
    m = M;
    graph.resize(m + n + 2);
    p.resize(m + n + 2);
    potential.resize(m + n + 2, 0);
    sink = m + n + 1;
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
        //LevitAlgorithm();
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
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            for (auto e: graph[i]) {
                if (e.b == j + n) {
                    std::cout << e.flow << " ";
                }
            }
        }
        std::cout << std::endl;
    }
}

struct Point {
    int x, y;
    int cap;
};

int main() {

    int n = 0, m = 0;
    std::cin >> n >> m;

    int x, y, w;
    bool all_positive = true;

    std::vector<Point> points;
    points.reserve(n + m);
    for (int i = 1; i <= n + m; i++) {
        std::cin >> x >> y >> w;
        Point point;
        point.x = x;
        point.y = y;
        point.cap = w;
        points.push_back(point);
        if (w < 0) {
            all_positive = false;
        }
    }

    long long granted_cost = 0;
    AssignmentProblem assignmentProblem(n, m);
    // Добавление ребер в граф
    for (int i = 0; i < points.size(); ++i) {
        if (i < n) {
            // Обработка зданий
            auto house = points[i];
            // 0-> [1-n], добавление ребра из источника в здание
            assignmentProblem.AddEdge(0, i + 1, house.cap, 0);

            // [1-n] -> [1-m], добавление ребер из здания в бомбоубежища
            for (int j = 0; j < m; ++j) {
                auto shelter = points[n + j];
                int len = std::abs(house.x - shelter.x) + std::abs(house.y - shelter.y) + 1;
                assignmentProblem.AddEdge(i + 1, n + j + 1, INT_MAX, len);

                std::cin >> x;//количество служащих, которые должны эвакуироваться из i-го здания в j-е бомбоубежище
                //std::cout << "flow from " << j + 1 << " to " << n + j + 1 << " = " << x << " ccost " << len
                //         << std::endl;
                granted_cost += x * len;
            }
        } else {
            // добавление ребер из бомбоубежища в сток
            assignmentProblem.AddEdge(i + 1, n + m + 1, points[i].cap, 0);
        }
    }

    //std::cout << "granted cost " << granted_cost << std::endl;
    auto min_cost = assignmentProblem.Solve(all_positive);
    if (granted_cost == min_cost) {
        std::cout << "OPTIMAL" << std::endl;
    } else {
        std::cout << "SUBOPTIMAL" << std::endl;
        assignmentProblem.PrintAssignment();
    }

    return 0;
}
