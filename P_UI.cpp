#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>


class Button {
public:
    Button(const std::string& text, float x, float y) {
        // Настройка шрифта
       
        // Настройка текста кнопки
        buttonText.setFont(font);
        buttonText.setString(text);
        buttonText.setCharacterSize(24);
        buttonText.setFillColor(sf::Color::White);
        buttonText.setPosition(x + 10, y + 5); // Положение текста внутри кнопки

        // Настройка прямоугольника кнопки
        buttonShape.setSize(sf::Vector2f(200, 50));
        buttonShape.setPosition(x, y);
        buttonShape.setFillColor(sf::Color::Blue);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(buttonShape);
        window.draw(buttonText);
    }

    bool isHovered(sf::Vector2f mousePos) {
        return buttonShape.getGlobalBounds().contains(mousePos);
    }

    void onClick() {
        std::cout << "Кнопка нажата!" << std::endl;
    }

private:
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
    sf::Font font;
};

class UI {
public:
    UI() {
        // Создаем кнопку
        button = new Button("Нажми меня", 300, 250);
    }

    ~UI() {
        delete button;
    }

    void draw(sf::RenderWindow& window) {
        button->draw(window);
    }

    void handleEvent(const sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (button->isHovered(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                button->onClick();
            }
        }
    }

private:
    Button* button;
};
