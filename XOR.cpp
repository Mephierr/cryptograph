#include "XOR.hpp"
#include <iostream>
#include <fstream>
#include <string>

// Функция для шифрования и дешифрования файла с использованием XOR
void xorCipher(const std::string &inputFile, const std::string &outputFile, char key) {
    std::ifstream inFile(inputFile, std::ios::binary);
    std::ofstream outFile(outputFile, std::ios::binary);
    
    if (!inFile.is_open() || !outFile.is_open()) {
        std::cerr << "Ошибка при открытии файла!" << std::endl;
        return;
    }

    char buffer;
    while (inFile.get(buffer)) {
        // Шифруем/дешифруем байт с помощью XOR
        buffer ^= key;
        outFile.put(buffer);
    }

    inFile.close();
    outFile.close();
}

/*int main() {
    std::string inputFile, outputFile;
    char key;

    // Ввод имени входного файла, выходного файла и ключа
    std::cout << "Введите имя входного файла: ";
    std::getline(std::cin, inputFile);
    std::cout << "Введите имя выходного файла: ";
    std::getline(std::cin, outputFile);
    std::cout << "Введите ключ (один символ): ";
    std::cin >> key;

    // Шифрование файла
    xorCipher(inputFile, outputFile, key);
    std::cout << "Файл зашифрован в " << outputFile << std::endl;

    // Для демонстрации обратного процесса (дешифрования), можно использовать тот же файл и ключ
    std::string decryptedOutputFile = "decrypted_" + outputFile;
    xorCipher(outputFile, decryptedOutputFile, key);
    std::cout << "Файл расшифрован в " << decryptedOutputFile << std::endl;

    return 0;
}*/
