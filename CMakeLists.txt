cmake_minimum_required(VERSION 2.8) # Проверка версии CMake.

project(CRYPTO)				# Название проекта

set(SOURCE main.cpp Headers/BMP/BMP.cpp Headers/Compression/compressor.cpp Headers/FileOP/FileOP.cpp Headers/KeyFileEncryption/KeyFileEncryption.cpp Headers/PNG/PNG.cpp Headers/encryption.cpp)			# Установка переменной со списком исходников
set(HEADERS Headers/Encryption/BMP/BMP_includes.h Headers/Encryption/BMP/BMP.h Headers/Compression/compression_includes.h Headers/Compression/compressor.h Headers/FileOP/FileOP_includes.h Headers/FileOP/FileOP.h Headers/KeyFileEncryption/KeyFileEncription_Includes.h Headers/KeyFileEncryption/KeyFileEncription.h Headers/PNG/PNG_Includes.h Headers/PNG/PNG.h)
add_executable(main ${HEADERS} ${SOURCE})	# Создает исполняемый файл с именем main
