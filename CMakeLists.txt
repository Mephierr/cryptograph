cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.

project(cryptograph)				# Название проекта

set(SOURCE_EXE main.cpp)			# Установка переменной со списком исходников

add_executable(main ${SOURCE_EXE})	# Создает исполняемый файл с именем main
