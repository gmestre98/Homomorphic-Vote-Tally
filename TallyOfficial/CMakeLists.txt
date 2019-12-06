cmake_minimum_required(VERSION 3.12)

#set the project name
project(tally)

set(WARNING_FLAGS -Wall)

add_library(tallyfileops tallyfileops.cpp)

#add the executable
add_executable(tally tally.cpp)

find_package(SEAL 3.4 REQUIRED)
target_link_libraries(tallyfileops SEAL::seal)
target_link_libraries(tally tallyfileops)
target_link_libraries(tally SEAL::seal)
