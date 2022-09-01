cmake_minimum_required (VERSION 3.8)

project("Knapsack")

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable (Knapsack "src/Main.cpp")

add_custom_target(copy-test-cases ALL
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/TestCases ${PROJECT_BINARY_DIR}/TestCases
		DEPENDS Knapsack)
