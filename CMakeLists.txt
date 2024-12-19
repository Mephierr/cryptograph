cmake_minimum_required(VERSION 3.5) # Проверка версии CMake.

project(cryptograph)

include_directories(Headers)

add_executable(cryptograph main.cpp Headers/includes.h Headers/Encryption/Compression/compressor.h Headers/Encryption/Compression/compression_includes.h Headers/Encryption/Compression/compressor.cpp Headers/Encryption/encryption_includes.h Headers/Encryption/KeyFileEncryption/KeyFileEncryption_Includes.h Headers/Encryption/KeyFileEncryption/KeyFileEncryption.h Headers/Encryption/KeyFileEncryption/KeyFileEncryption.cpp Headers/Encryption/FileOP/FileOP.h Headers/Encryption/FileOP/FileOP_includes.h Headers/Encryption/FileOP/FileOP.cpp Headers/Encryption/encryption.h Headers/Encryption/BMP/BMP.cpp Headers/Encryption/BMP/BMP_includes.h Headers/Encryption/BMP/BMP.h Headers/Encryption/encryption.cpp Headers/Encryption/PNG/PNG.cpp Headers/Encryption/PNG/PNG.h Headers/Encryption/PNG/PNG_Includes.h)