cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.

project(CRYPTO)				# Название проекта

set(SOURCE main.cpp Headers/BMP/BMP.cpp Headers/Compression/compressor.cpp Headers/FileOP/FileOP.cpp Headers/KeyFileEncryption/KeyFileEncryption.cpp Headers/PNG/PNG.cpp Headers/encryption.cpp)			# Установка переменной со списком исходников
set(HEADERS)

add_executable(main ${SOURCE})	# Создает исполняемый файл с именем main
