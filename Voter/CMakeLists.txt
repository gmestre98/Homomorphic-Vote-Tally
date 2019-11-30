cmake_minimum_required(VERSION 3.12)

#set the project name
project(voter)


add_library(deal dealwithvotes.cpp)
#add the executable
add_executable(voter voter.cpp)


target_link_libraries(voter deal)
find_package(SEAL 3.4 REQUIRED)
target_link_libraries(voter SEAL::seal)
