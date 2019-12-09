cmake_minimum_required(VERSION 3.12)

#set the project name
project(counter)

#add the executable
add_executable(counter counter.cpp)

find_package(SEAL 3.4 REQUIRED)
target_link_libraries(counter SEAL::seal)
