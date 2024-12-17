cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.

project(cryptograph)				# Название проекта

set(SOURCE_EXE main.cpp P_UI.cpp XOR.cpp)			# Установка переменной со списком исходников
set(HEADERS XOR.hpp P_UI.hpp)
add_executable(main ${HEADERS} ${SOURCE_EXE})	# Создает исполняемый файл с именем main
