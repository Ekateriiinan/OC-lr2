cmake_minimum_required(VERSION 3.10)
project(array_sum)

set(CMAKE_C_STANDARD 11)

add_executable(array_sum 
    main.c 
    thread_functions.c 
    thread_impl.c
)

if(UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(array_sum Threads::Threads)
endif()