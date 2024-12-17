#include<iostream>
#include"XOR.hpp"
#include"P_UI.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Пример UI на SFML");
    UI ui;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            ui.handleEvent(event);
        }

        window.clear();
        ui.draw(window);
        window.display();
    }

    return 0;
}