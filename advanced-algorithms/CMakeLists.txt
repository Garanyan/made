cmake_minimum_required(VERSION 3.5)
project(made)
set(CMAKE_CXX_STANDARD 17)


### Задачи
### HW 1
add_executable(HW1_task_A HW1/task_A.cpp)
add_executable(HW1_task_B HW1/task_B.cpp)
add_executable(HW1_task_C HW1/task_C.cpp)
add_executable(HW1_task_D HW1/task_D.cpp)
add_executable(HW1_task_E HW1/task_E.cpp)

### HW 2
add_executable(HW2_task_A HW2/task_A.cpp)
add_executable(HW2_task_B HW2/task_B.cpp)
add_executable(HW2_task_C HW2/task_C.cpp)
add_executable(HW2_task_D HW2/task_D.cpp)
add_executable(HW2_task_E HW2/task_E.cpp)
add_executable(HW2_task_F HW2/task_F.cpp)

### HW 3
add_executable(HW3_task_A HW3/HW3_task_A.cpp)
add_executable(HW3_task_B HW3/HW3_task_B.cpp)
add_executable(HW3_task_D HW3/HW3_task_D.cpp)
add_executable(HW3_task_E HW3/HW3_task_E.cpp)

## HW 4
add_executable(HW4_task_A HW4/HW4_task_A.cpp)
add_executable(HW4_task_B HW4/HW4_task_B.cpp)
add_executable(HW4_task_C HW4/HW4_task_C.cpp)
add_executable(HW4_task_D HW4/HW4_task_D.cpp)
add_executable(HW4_task_E HW4/HW4_task_E.cpp)

## HW 5
add_executable(HW5_task_A HW5/HW5_task_A.cpp)
add_executable(HW5_task_B HW5/HW5_task_B.cpp)
add_executable(HW5_task_C HW5/HW5_task_C.cpp)
add_executable(HW5_task_D HW5/HW5_task_D.cpp)

# HW 6
add_executable(HW6_task_A HW6/HW6_task_A.cpp)

add_executable(HW6_task_D HW6/HW6_task_D.cpp)
add_executable(HW6_task_E HW6/HW6_task_E.cpp)

# HW 8
add_executable(HW8_task_A HW8/HW8_task_A.cpp)
add_executable(HW8_task_B HW8/HW8_task_B.cpp)
add_executable(HW8_task_C HW8/HW8_task_C.cpp)

## HW 9
add_executable(HW9_task_A HW9/HW9_task_A.cpp)
add_executable(HW9_task_B HW9/HW9_task_B.cpp)
add_executable(HW9_task_C HW9/HW9_task_C.cpp)
add_executable(HW9_task_D HW9/HW9_task_D.cpp)

## HW 10
add_executable(HW10 HW10/HW10.cpp
        HW10/tests.cpp
        HW10/Parser/Expression.cpp
        HW10/Parser/Expression.hpp

        HW10/Parser/Parser.cpp
        HW10/Parser/Parser.hpp

        )

# HW11
add_executable(HW11_taskA HW11/HW11_taskA.cpp)
add_executable(HW11_taskB HW11/HW11_taskB.cpp)
add_executable(HW11_taskC HW11/HW11_taskC.cpp)
add_executable(HW11_taskD HW11/HW11_taskD.cpp)

# HW12
add_executable(HW12_taskA HW12/HW12_A.cpp)

# HW13
add_executable(HW13_taskA HW13/HW13_A.cpp)
add_executable(HW13_taskB HW13/HW13_B.cpp)
add_executable(HW13_taskE HW13/HW13_E.cpp)