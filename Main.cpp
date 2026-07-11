#ifdef _WIN32
#define NOWINBASEINTERLOCK 
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include <iostream>
#include "neuralnet.h"
#include "drone.h"
#include "TimeManager.h"
#include "Target.h"
#include <cstring>
#include <string>
#include <format>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <random>
#include "configs.h"
#include "Agents.h"

Assets assets;
generation_t gen_atual;
generation_t next_gen;
Target targets[NUMBER_OF_TARGETS];
Target mouse_target;

int main() {
#ifdef _WIN32
    SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED);
#endif
    TimeManager time_manager;
    double tempo_decorrido = 0;
    double delta_t = 0;


    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "TEST");
    sf::Font font;
    if (!font.loadFromFile("C:/Users/rmari/AppData/Local/Microsoft/Windows/Fonts/JetBrainsMonoNLNerdFontMono-Regular.ttf")) {
        std::cerr << "Erro ao carregar a fonte!" << std::endl;
        return -1;
    }
    sf::Text text("0", font, 30);
    text.setFillColor(sf::Color::White);
    text.setPosition(50.f, 50.f);


    window.setFramerateLimit(0);
    bool treinando = true;

    size_t entidades_vivas = N_AGENTS;
    ResetarTargets(targets);
    ResetAgents(gen_atual.agents);

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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            window.setFramerateLimit(0);
            treinando = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.setFramerateLimit(240);
            treinando = false;
        }
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------
        // Clear na tela
        window.clear(sf::Color(31, 64, 62));
        window.draw(text);
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------
        // Update da física

        if (treinando) {
            for (Target& target : targets)
                target.Draw(&window, assets);
            delta_t = 0.004;
            ControlarDrones(gen_atual.agents, targets, delta_t, entidades_vivas);
        }
        else {
            sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
            mouse_target.SetPosition((float)(mouse_position.x - WINDOW_WIDTH / 2) / SCALE, (float)(mouse_position.y - WINDOW_HEIGHT / 2) / SCALE);
            mouse_target.Draw(&window, assets);
            delta_t = time_manager.Update();
            SeguirMouse(gen_atual.agents, mouse_target, delta_t, entidades_vivas);
        }

        for (size_t i = 0; i < N_AGENTS; i++)
            if (gen_atual.agents[i].alive)
                gen_atual.agents[i].drone.Draw(&window, assets, true);
 
        if (tempo_decorrido > TEMPO_TREINAMENTO && treinando) {
            tempo_decorrido = 0;
            entidades_vivas = 0;
        }
        if (not entidades_vivas) {
            entidades_vivas = N_AGENTS;
            if (treinando) {
                QuickSort(gen_atual.agents, 0, N_AGENTS - 1);
                Evolve(gen_atual, next_gen);
                for (size_t i = 0; i < N_AGENTS; i++)
                    next_gen.agents[i].net.TransferData(gen_atual.agents[i].net);
            }
            ResetarTargets(targets);
            ResetAgents(gen_atual.agents);
            
            text.setString(std::to_string(++gen_atual.number));
            tempo_decorrido = 0;
        }

        //for (agent_t& agent : gen_atual.agents)
            //agent.drone.Draw(&window, assets, true);
        
            
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------
        tempo_decorrido += delta_t;
        window.display();
    }
#ifdef _WIN32
    SetThreadExecutionState(ES_CONTINUOUS);
#endif
	return 0;
}