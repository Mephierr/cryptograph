#include<iostream>
#include <vector>
#include <string>
#include <limits>


class UI {
public:
    // Отображаем меню
    void displayMenu(const std::vector<std::string>& options) {
        std::cout << "Выберите опцию:" << std::endl;
        for (size_t i = 0; i < options.size(); ++i) {
            std::cout << i + 1 << ". " << options[i] << std::endl;
        }
        std::cout << "0. Выход" << std::endl;
    }

    // Получаем ввод от пользователя
    int getUserInput(int maxOption) {
        int choice;
        std::cout << "Введите номер опции: ";
        std::cin >> choice;

        // Проверка на корректный ввод
        while (std::cin.fail() || choice < 0 || choice > maxOption) {
            std::cin.clear(); // Очистить флаг ошибки
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Игнорировать неверный ввод
            std::cout << "Неверный ввод. Пожалуйста, попробуйте снова: ";
            std::cin >> choice;
        }
        return choice;
    }

    // Вывод сообщения
    void showMessage(const std::string& message) {
        std::cout << message << std::endl;
    }
};


int main()
{
    UI ui;
    std::vector<std::string> options = {"Опция 1", "Опция 2", "Опция 3"};

    while (true) {
        ui.displayMenu(options);
        int choice = ui.getUserInput(options.size());

        if (choice == 0) {
            ui.showMessage("Выход из программы.");
            break;
        }

        ui.showMessage("Вы выбрали: " + options[choice - 1]);
    }

    return 0;
}