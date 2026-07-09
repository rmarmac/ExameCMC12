#include <iostream>
#include "neuralnet.h"
#include "fileop.h"
#include "drone.h"
#include "TimeManager.h"
#include "Target.h"
#include <cstring>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <random>
#include "configs.h"
#include "Agents.h"

Assets assets;
generation_t gen_atual;
generation_t next_gen;
Target targets[NUMBER_OF_TARGETS];

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distrib(0.0, 1.0);
    TimeManager time_manager;
    
    Target target(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    double tempo_decorrido = 0;
    double delta_t = 0;

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "TEST");
    window.setFramerateLimit(0);
    bool treinando = true;

    size_t entidades_vivas = N_AGENTS;

    ResetAgents(gen_atual.agents);
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            window.setFramerateLimit(0);
            treinando = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.setFramerateLimit(240);
            treinando = false;
        }
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------
        // Display na tela
        window.clear(sf::Color(31, 64, 62));
        target.Draw(&window, assets);
    
        
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------
        // Update da física

        if (treinando)
            delta_t = 0.004; 
        else
            delta_t = time_manager.Update();


        if (not treinando) {
            sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
            target.SetPosition(mouse_position.x, mouse_position.y);
        }


            
        ControlarDrones(gen_atual.agents, target);
        vector_t target_position = target.GetPosition();
        for (size_t i = 0; i < N_AGENTS; i++) {
            if (gen_atual.agents[i].alive) {
                gen_atual.agents[i].drone.Update(delta_t);
                gen_atual.agents[i].pontuacao += delta_t * AvaliarPontuacao(gen_atual.agents[i].drone, target_position);
                vector_t position = gen_atual.agents[i].drone.GetPosition();
                if (position.x < 0 || position.x > WINDOW_WIDTH || position.y < 0 || position.y > WINDOW_HEIGHT) {
                    gen_atual.agents[i].alive = false;
                    gen_atual.agents[i].pontuacao -= TEMPO_TREINAMENTO;
                    --entidades_vivas;
                }
                double delta_x = position.x - target_position.x;
                double delta_y = position.y - target_position.y;
                if (delta_x * delta_x + delta_y *  delta_y < DIST_ATINGIR_ALVO * DIST_ATINGIR_ALVO) {
                    target.atingido = true;
                    gen_atual.agents[i].pontuacao += TEMPO_TREINAMENTO / 2;
                }
                gen_atual.agents[i].drone.Draw(&window, assets, true);
            }
        }
        if (tempo_decorrido > TEMPO_TREINAMENTO && treinando) {
            tempo_decorrido = 0;
            entidades_vivas = 0;
        }
        if (not entidades_vivas) {
            target.SetPosition((float)distrib(gen) * WINDOW_WIDTH, (float)distrib(gen) * WINDOW_HEIGHT);
            entidades_vivas = N_AGENTS;
            if (treinando) {
                QuickSort(gen_atual.agents, 0, N_AGENTS - 1);
                Evolve(gen_atual, next_gen);
                for (size_t i = 0; i < N_AGENTS; i++)
                    next_gen.agents[i].net.TransferData(gen_atual.agents[i].net);
            }
            ResetAgents(gen_atual.agents);
            tempo_decorrido = 0;
        }

        if(target.atingido && treinando)
            target.SetPosition((float)distrib(gen) * WINDOW_WIDTH, (float)distrib(gen) * WINDOW_HEIGHT);

        //for (agent_t& agent : gen_atual.agents)
            //agent.drone.Draw(&window, assets, true);
        
            
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------
        tempo_decorrido += delta_t;
        window.display();
    }
	return 0;
}