#include <iostream>
#include "neuralnet.h"
#include "fileop.h"
#include "drone.h"
#include "TimeManager.h"
#include "Target.h"
#include <SFML/Graphics.hpp>
#include <stdlib.h>

#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 900
#define N_AGENTS 10

typedef struct entidade_t {
    bool alive;
    Drone drone;
} entidade_t;

typedef struct agent_t {
    NeuralNetwork net;
    double pontuacao;
} agent_t;

//NeuralNetwork net("Rede1.txt");
Assets assets;
agent_t agents[N_AGENTS];
entidade_t entidades[N_AGENTS];

Drone drone(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);


int main() {
    TimeManager time_manager;
    Target target(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    double delta_t = 0;

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "TEST");
    window.setFramerateLimit(240);
    bool executando = false;
    // ----------------------------------------------------------------------------------------------------------------------------------------------------------------
    // ----------------------------------------------------------------------------------------------------------------------------------------------------------------
    // Loop principal
    while (window.isOpen())
    {
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------
        // Checagem de eventos
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            executando = true;
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------
        // Lógica do jogo
        








        // ------------------------------------------------------------------------------------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------
        // Display na tela
        window.clear(sf::Color(31, 64, 62));
        target.Draw(&window, assets);
        drone.Draw(&window, assets);
        
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------
        // Update da física
        delta_t = time_manager.Update();


        if (executando) {
            drone.ApplyForces(2.4, 2.6);
            drone.Update(delta_t);
            vector_t position = drone.GetPosition();
            if (position.x < 0 || position.x > WINDOW_WIDTH || position.y < 0 || position.y > WINDOW_HEIGHT)
                drone.Reset(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        }


            
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------
        
        window.display();
    }
	return 0;
}