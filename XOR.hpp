#pragma once
#ifndef XOR

#include <string>

/**
 * @brief Шифрует или дешифрует файл с использованием XOR-шифрования.
 * 
 * @param inputFile Путь к входному файлу, который нужно зашифровать или расшифровать.
 * @param outputFile Путь к выходному файлу, в который будет записан результат.
 * @param key Символ, используемый в качестве ключа для шифрования и дешифрования.
 */
void xorCipher(const std::string &inputFile, const std::string &outputFile, char key);

#endif