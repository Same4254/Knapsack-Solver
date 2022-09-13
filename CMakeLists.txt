cmake_minimum_required (VERSION 3.8)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

project("Knapsack")

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable (Knapsack "src/Main.cpp" "src/BruteForce.cpp" "src/Utils.cpp" "src/DP.cpp" "src/BranchAndBound.cpp")

add_custom_target(copy-test-cases ALL
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/TestCases ${PROJECT_BINARY_DIR}/TestCases
		DEPENDS Knapsack)
