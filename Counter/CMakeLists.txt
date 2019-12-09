cmake_minimum_required(VERSION 3.12)

#set the project name
project(counter)

add_library(tweet tweetnacl.c)
add_library(randombytes randombytes.c)
add_library(hazmat hazmat.c)
add_library(sss sss.c)
add_library(counterops counterops.cpp)

#add the executable
add_executable(counter counter.cpp)


target_link_libraries(hazmat randombytes)
target_link_libraries(sss hazmat)
target_link_libraries(sss tweet)
target_link_libraries(sss randombytes)
target_link_libraries(counterops sss)
target_link_libraries(counterops SEAL::seal)
target_link_libraries(counter counterops)
find_package(SEAL 3.4 REQUIRED)
target_link_libraries(counter SEAL::seal)
