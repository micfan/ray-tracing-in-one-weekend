cmake_minimum_required(VERSION 3.12)
project(ray_tracing_in_a_weekend)

set(CMAKE_CXX_STANDARD 11)

file (GLOB RT_SOURCE_FIELS "./*.hpp")

add_executable(ray_tracing_in_a_weekend main.cpp ${RT_SOURCE_FIELS})