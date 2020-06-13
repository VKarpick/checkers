#include <iostream>

#include <SFML/Graphics.hpp>

#include "ai/checkers_trainer.h"
#include "checkers.h"
#include "checkers_sfml.h"
#include "checkerboard.h"



int main(int argc, char** arvg)
{
    //CheckersTrainer checkers_trainer(50, 6, 0.1);
    //CheckersTrainer checkers_trainer(std::vector<double>(128, 2.0), 100, 4);
    //CheckersTrainer checkers_trainer(1, 1);
    //checkers_trainer.train();

    /*Checkers checkers;
    checkers.play();*/

    Checkerboard board;

    sf::RenderWindow window(sf::VideoMode(512, 512), "Checkers");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            window.clear(sf::Color::Black);

            window.draw(board);

            window.display();
        }
    }

    return 0;
}
