cmake_minimum_required(VERSION 3.12)

#set the project name
project(admin)

add_library(tweet tweetnacl.c)
add_library(randombytes randombytes.c)
add_library(hazmat hazmat.c)
add_library(sss sss.c)
add_library(Generations generations.cpp)
add_library(adminsealops adminsealops.cpp)

#add the executable
add_executable(admin admin.cpp)


target_link_libraries(admin Generations)
target_link_libraries(hazmat randombytes)
target_link_libraries(sss hazmat)
target_link_libraries(sss tweet)
target_link_libraries(sss randombytes)
target_link_libraries(adminsealops sss)
target_link_libraries(adminsealops SEAL::seal)
target_link_libraries(admin adminsealops)
find_package(SEAL 3.4 REQUIRED)
target_link_libraries(admin SEAL::seal)
