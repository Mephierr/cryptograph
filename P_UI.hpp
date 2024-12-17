#ifndef P_UI_HPP
#define P_UI_HPP
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>

class Button {
public:
    // Конструктор
    Button(const std::string& text, float x, float y);

    // Метод для отрисовки кнопки
    void draw(sf::RenderWindow& window);

    // Метод для проверки, наведена ли мышь на кнопку
    bool isHovered(sf::Vector2f mousePos);

    // Метод для обработки нажатия кнопки
    void onClick();

private:
    sf::RectangleShape buttonShape; // Форма кнопки
    sf::Text buttonText;            // Текст кнопки
    sf::Font font;                  // Шрифт кнопки
};

class UI {
public:
    // Конструктор
    UI();

    // Деструктор
    ~UI();

    // Метод для отрисовки интерфейса
    void draw(sf::RenderWindow& window);

    // Метод для обработки событий
    void handleEvent(const sf::Event& event);

private:
    Button* button; // Указатель на кнопку
};

#endif 