cmake_minimum_required(VERSION 3.5)
project(made)
set (CMAKE_CXX_STANDARD 11)


### Задачи
add_executable(task_1 Algorithms/task_1.cpp)
add_executable(task_2 Algorithms/task_2.cpp)
add_executable(task_3 Algorithms/task_3.cpp)
add_executable(task_4 Algorithms/task_4.cpp)


add_executable(task_6 Algorithms/task_6.cpp)
add_executable(task_7 Algorithms/task_7.cpp)
add_executable(task_8 Algorithms/task_8.cpp)
add_executable(task_9 Algorithms/task_9.cpp Algorithms/Huffman.h)
add_executable(task_11 Algorithms/task_11.cpp)
add_executable(task_17 Algorithms/task_17.cpp)

add_executable(task_14 Algorithms/task_14.cpp)
add_executable(task_16 Algorithms/task_16.cpp)





### Семинары
add_executable(seminar_2 Algorithms/seminar_2.cpp)
add_executable(seminar_3 Algorithms/seminar_3.cpp)
add_executable(seminar_4 Algorithms/seminar_4.cpp)

