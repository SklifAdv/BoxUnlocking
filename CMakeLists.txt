cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)

project(BoxUnlocking VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(NAME_SRC
	main.cpp
)
add_executable(BoxUnlocking ${NAME_SRC} ${NAME_HEADERS})