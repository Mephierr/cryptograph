cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.

project(cryptograph)				# Название проекта

set(SOURCE_EXE main.cpp P_UI.cpp XOR.cpp)			# Установка переменной со списком исходников
set(HEADERS XOR.hpp P_UI.hpp)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(main ${HEADERS} ${SOURCE_EXE})	# Создает исполняемый файл с именем main

target_link_libraries(main sfml-graphics sfml-window sfml-system)
