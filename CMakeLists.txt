cmake_minimum_required(VERSION 2.8)

project(AdventOfCode2023)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_executable(AdventOfCode2023
	AdventOfCode2023.cpp
	AdventOfCode2023.h
	Problem1.cpp
	Problem2.cpp
	Problem3.cpp
	_clang-format
)

