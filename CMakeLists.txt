cmake_minimum_required (VERSION 2.6)

project(mcpelaunchershadersmod LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

find_library(log-lib log android-lib android)


add_library(mcpelaunchershadersmod SHARED src/main.cpp)
target_link_libraries(mcpelaunchershadersmod PUBLIC ${log-lib} android)
