
 **Суффиксный массив**
 
 ограничение по времени на тест ```2``` секунды
 
 ограничение по памяти на тест ```512``` мегабайт
 
 ввод стандартный ввод
 
 вывод стандартный вывод
 

 Постройте суффиксный массив для заданной строки ```s```,
 для каждых двух соседних суффиксов найдите длину максимального общего префикса.

 Входные данные
 Первая строка входного файла содержит строку ```s (1 ≤ |s| ≤ 400 000)```. 
 Строка состоит из строчных латинских букв.
 
 Выходные данные
 В первой строке выведите ```|s|``` различных чисел — номера первых 
 символов суффиксов строки ```s``` так, чтобы соответствующие суффиксы были 
 упорядочены в лексикографически возрастающем порядке. 
 Во второй строке выведите ```|s| - 1``` чисел — длины наибольших общих префиксов.
 
 |   |   | 
 |---|---|
 | Входные данные  |   ababb|   
 | Выходные данные  |  1 3 5 2 4 <br> 2 0 1 1   |  