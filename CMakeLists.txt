cmake_minimum_required(VERSION 4.1)
project(obligatory_assignment_1 C)

set(ENV{OpenMP_ROOT} "/opt/homebrew/opt/libomp")

set(CMAKE_C_STANDARD 11)

find_package(OpenMP REQUIRED)

add_executable(obligatory_assignment_1
        problem_1.c
        problem_3.c
)

target_link_libraries(obligatory_assignment_1 OpenMP::OpenMP_C)