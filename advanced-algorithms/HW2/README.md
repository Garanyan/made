----

**A. Разбиение на пары**

|   |   |
|---|---|
|ограничение по времени на тест| 2 секунды|
|ограничение по памяти на тест|64 мегабайт|
|ввод|стандартный ввод|
|вывод|стандартный вывод|

Однажды n ребят решили поучаствовать в VK Cup. Для этого им нужно разбиться на команды по два человека. Однако выяснилось, что не все ребята хорошо ладят друг с другом. Вы выяснили для каждой пары ребят, могут ли они участвовать в одной команде. Помогите им сформировать максимальное число команд.

**Входные данные**

Первая строка содержит число n (1 ≤ n ≤ 18), далее n строк по n символов описывают отношения ребят. Символ j строки i равен «Y», если ребята i и j могут участвовать в команде.

**Выходные данные**

Выведите одно число — максимальное число участников в командах, которые можно сформировать.

*Пример*

**Входные данные**
```
5
NYYYY
YNNNN
YNNNN
YNNNN
YNNNN
```

**Выходные данные**

```
2
```
---

**B. Удаление скобок 2.0**

|   |   |
|---|---|
|ограничение по времени на тест| 2 секунды|
|ограничение по памяти на тест|256 мегабайт|
|ввод|стандартный ввод|
|вывод|стандартный вывод|

Дана строка, составленная из круглых, квадратных и фигурных скобок. Определите, какое наименьшее количество символов необходимо удалить из этой строки, чтобы оставшиеся символы образовывали правильную скобочную последовательность.

**Входные данные**

Во входном файле записана строка из круглых, квадратных и фигурных скобок. Длина строки не превосходит {100} символов.

**Выходные данные**

Выведите строку максимальной длины, являющейся правильной скобочной последовательностью, которую можно получить из исходной строки удалением некоторых символов.

*Пример*

**Входные данные**
```
([)]
```

**Выходные данные**

```
[]
```

---

**C. Выбор вершин дерева**

|   |   |
|---|---|
|ограничение по времени на тест| 2 секунды|
|ограничение по памяти на тест|256 мегабайт|
|ввод|стандартный ввод|
|вывод|стандартный вывод|

Дан граф, являющийся деревом. Необходимо найти размер наибольшего по мощности независимого множества. 
Множество вершин графа называется **независимым**, если никакие две вершины этого множества не соединены ребром.

**Входные данные**

Граф в этой задаче задан в виде *корневого дерева*. В графе выделена вершина — *корень дерева*. 
Для каждой вершины i, не являющейся корнем, задан номер вершины-предка p_i в корневом дереве. 
Дерево, заданное таким образом, состоит из рёбер i — p_i для всех вершин i, кроме корня.

В первой строке входного файла записано целое число n — количество вершин в графе (1 ≤ n ≤ 100). 
В следующих n строках задан граф. В i-й из этих строк записано целое число p_i — номер вершины-предка i-й 
вершины. Для корня дерева p_i = 0; для всех остальных вершин 1 ≤ p_i ≤ n.

Гарантируется, что заданный во входном файле граф является деревом.

**Выходные данные**

В первой строке выходного файла выведите одно число — размер наибольшего по мощности независимого множества.

*Пример*

**Входные данные**
```
5
0
1
1
2
3
```

**Выходные данные**

```
3
```
---


**D. Продавец аквариумов**

|   |   |
|---|---|
|ограничение по времени на тест| 1 секунды|
|ограничение по памяти на тест|256 мегабайт|
|ввод|стандартный ввод|
|вывод|стандартный вывод|

Продавец аквариумов для кошек хочет объехать n городов, посетив каждый из них ровно один раз. Помогите ему найти кратчайший путь.

**Входные данные**

Первая строка входного файла содержит натуральное число n (1 ≤ n ≤ 13) — количество городов. 
Следующие n строк содержат по  n чисел — длины путей между городами.

В  i-й строке j-е число — a_{i,j} — это расстояние между городами i и j 
(0 ≤ a_{i,j} ≤ 10^6; a_{i,j} = a_{j,i}; a_{i,i} = 0).

**Выходные данные**

В первой строке выходного файла выведите длину кратчайшего пути. 
Во второй строке выведите n чисел — порядок, в котором нужно посетить города.

*Пример*

**Входные данные**
```
5
0 183 163 173 181
183 0 165 172 171
163 165 0 189 302
173 172 189 0 167
181 171 302 167 0
```

**Выходные данные**

```
666
4 5 2 3 1 
```
---
**E. Замощение доминошками**

|   |   |
|---|---|
|ограничение по времени на тест| 1 секунды|
|ограничение по памяти на тест|256 мегабайт|
|ввод|стандартный ввод|
|вывод|стандартный вывод|

Вам дана прямоугольная сетка из квадратных ячеек. 
Символ 'X' представляет собой уже покрытую ячейку, символ '.' - это ячейка, которую еще нужно покрыть.

Вы хотите покрыть все ячейки '.', непересекающимися доминошками 2×1. 
Найдите количество способов сделать это. 
Два способа считаются различными, если две ячейки покрыты одним и тем же домино в одном замощении и двумя разными домино в другом замощении.

**Входные данные**

Первая строка содержит числа n и m (1≤n,m≤12). Следующие n строк содержат по m символов — описание доски.

**Выходные данные**

Выведите число замощений доминошками.

*Пример*

**Входные данные**
```
2 2
..
..
```

**Выходные данные**

```
2 
```