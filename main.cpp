#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cctype>
#include <cstring> // Добавлено для использования memset
#include <openssl/aes.h>
#include <openssl/rand.h>

void xorEncryptDecrypt(const std::string& inputFile, const std::string& outputFile, char key) {
    std::ifstream inFile(inputFile, std::ios::binary);
    std::ofstream outFile(outputFile, std::ios::binary);

    if (!inFile.is_open() || !outFile.is_open()) {
        throw std::runtime_error("Ошибка при открытии файла.");
    }

    char buffer;
    while (inFile.get(buffer)) {
        buffer ^= key; // Применяем XOR
        outFile.put(buffer);
    }

    inFile.close();
    outFile.close();
}

void caesarEncryptDecrypt(const std::string& inputFile, const std::string& outputFile, int shift) {
    std::ifstream inFile(inputFile);
    std::ofstream outFile(outputFile);

    if (!inFile.is_open() || !outFile.is_open()) {
        throw std::runtime_error("Ошибка при открытии файла.");
    }

    char buffer;
    while (inFile.get(buffer)) {
        if (std::isalpha(buffer)) {
            char base = std::islower(buffer) ? 'a' : 'A';
            buffer = static_cast<char>((buffer - base + shift + 26) % 26 + base); // Цезарь
        }
        outFile.put(buffer);
    }

    inFile.close();
    outFile.close();
}

// Функции для AES
void aesEncrypt(const std::string& inputFile, const std::string& outputFile, const unsigned char* key) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw std::runtime_error("Ошибка при создании контекста шифрования.");

    std::ifstream inFile(inputFile, std::ios::binary);
    std::ofstream outFile(outputFile, std::ios::binary);
    if (!inFile.is_open() || !outFile.is_open()) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Ошибка при открытии файла.");
    }

    if (EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), nullptr, key, nullptr) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Ошибка при инициализации шифрования.");
    }

    unsigned char buffer[16];
    unsigned char outBuffer[16];
    int outLen;

    while (inFile.read(reinterpret_cast<char*>(buffer), sizeof(buffer))) {
        if (EVP_EncryptUpdate(ctx, outBuffer, &outLen, buffer, sizeof(buffer)) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Ошибка при шифровании.");
        }
        outFile.write(reinterpret_cast<char*>(outBuffer), outLen);
    }

    // Обработка последнего блока
    size_t bytesRead = inFile.gcount();
    if (bytesRead > 0) {
        if (EVP_EncryptUpdate(ctx, outBuffer, &outLen, buffer, bytesRead) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Ошибка при шифровании.");
        }
        outFile.write(reinterpret_cast<char*>(outBuffer), outLen);
    }

    // Завершение шифрования
    if (EVP_EncryptFinal_ex(ctx, outBuffer, &outLen) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Ошибка при завершении шифрования.");
    }
    outFile.write(reinterpret_cast<char*>(outBuffer), outLen);

    EVP_CIPHER_CTX_free(ctx);
    inFile.close();
    outFile.close();
}

void aesDecrypt(const std::string& inputFile, const std::string& outputFile, const unsigned char* key) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) throw std::runtime_error("Ошибка при создании контекста дешифрования.");

    std::ifstream inFile(inputFile, std::ios::binary);
    std::ofstream outFile(outputFile, std::ios::binary);
    if (!inFile.is_open() || !outFile.is_open()) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Ошибка при открытии файла.");
    }

    if (EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), nullptr, key, nullptr) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Ошибка при инициализации дешифрования.");
    }

    unsigned char buffer[16];
    unsigned char outBuffer[16];
    int outLen;

    while (inFile.read(reinterpret_cast<char*>(buffer), sizeof(buffer))) {
        if (EVP_DecryptUpdate(ctx, outBuffer, &outLen, buffer, sizeof(buffer)) != 1) 
        {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Ошибка при дешифровании.");
        }
        outFile.write(reinterpret_cast<char*>(outBuffer), outLen);
    }

    // Обработка последнего блока
    size_t bytesRead = inFile.gcount();
    if (bytesRead > 0) {
        if (EVP_DecryptUpdate(ctx, outBuffer, &outLen, buffer, bytesRead) != 1) {
                        EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Ошибка при дешифровании.");
        }
        outFile.write(reinterpret_cast<char*>(outBuffer), outLen);
    }

    // Завершение дешифрования
    if (EVP_DecryptFinal_ex(ctx, outBuffer, &outLen) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw std::runtime_error("Ошибка при завершении дешифрования.");
    }
    outFile.write(reinterpret_cast<char*>(outBuffer), outLen);

    EVP_CIPHER_CTX_free(ctx);
    inFile.close();
    outFile.close();
}


// Основная функция
int main() {
    std::string inputFile, outputFile;
    int method;

    std::cout << "Выберите метод шифрования:\n1. XOR\n2. Caesar Cipher\n3. AES\n";
    std::cin >> method;

    std::cout << "Введите имя файла для шифрования/дешифрования: ";
    std::cin >> inputFile;
    std::cout << "Введите имя выходного файла: ";
    std::cin >> outputFile;

    if (method == 1) {
        char key;
        std::cout << "Введите ключ для XOR (один символ): ";
        std::cin >> key;
        xorEncryptDecrypt(inputFile, outputFile, key);
    } else if (method == 2) {
        int shift;
        std::cout << "Введите сдвиг для Caesar Cipher: ";
        std::cin >> shift;
        caesarEncryptDecrypt(inputFile, outputFile, shift);
    } else if (method == 3) {
        unsigned char key[16]; // Ключ для AES (128 бит)
        std::cout << "Введите 16 символов в качестве ключа для AES: ";
        std::cin >> reinterpret_cast<char*>(key);
        
        int action;
        std::cout << "1. Шифровать\n2. Дешифровать\n";
        std::cin >> action;

        if (action == 1) {
            aesEncrypt(inputFile, outputFile, key);
        } else if (action == 2) {
            aesDecrypt(inputFile, outputFile, key);
        } else {
            std::cerr << "Неверное действие." << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Неверный метод." << std::endl;
        return 1;
    }

    std::cout << "Файл успешно обработан." << std::endl;

    return 0;
}
