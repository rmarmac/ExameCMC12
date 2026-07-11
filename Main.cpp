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
// Target mouse_target;
Target test_target(0, -1);

int main() {
#ifdef _WIN32
    SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED);
#endif
    TimeManager time_manager;
    double tempo_decorrido = 0;
    double delta_t = 0.004;

    FILE* arq_drone;
    fopen_s(&arq_drone, "C:/ITA/Prof/CMC12/EXAME/dronepos.txt", "w");
    FILE* arq_geracao;
    fopen_s(&arq_geracao, "C:/ITA/Prof/CMC12/EXAME/generation.txt", "w");

    if (arq_drone == NULL || arq_geracao == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }

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
            delta_t = 0.004;
            treinando = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.setFramerateLimit(240);
            treinando = false;
            ResetAgents(gen_atual.agents);
            tempo_decorrido = 0;
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
            ControlarDrones(gen_atual.agents, targets, delta_t, entidades_vivas);
            for (size_t i = 0; i < N_AGENTS; i++)
                if (gen_atual.agents[i].alive)
                    gen_atual.agents[i].drone.Draw(&window, assets, true);
        }
        else {
            /*sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
            mouse_target.SetPosition((float)(mouse_position.x - WINDOW_WIDTH / 2) / SCALE, (float)(mouse_position.y - WINDOW_HEIGHT / 2) / SCALE);
            mouse_target.Draw(&window, assets);
            delta_t = time_manager.Update();
            SeguirMouse(gen_atual.agents, mouse_target, delta_t, entidades_vivas);*/
            TreinoB(gen_atual.agents[N_AGENTS - 1], test_target, tempo_decorrido, delta_t);
            gen_atual.agents[N_AGENTS - 1].drone.Draw(&window, assets, true);
            vector_t posicao = gen_atual.agents[N_AGENTS - 1].drone.GetPosition();
            test_target.Draw(&window, assets);
            fprintf_s(arq_drone, "%lf %lf %lf\n", posicao.x, posicao.y, tempo_decorrido);
        }

        
 
        if (tempo_decorrido > TEMPO_TREINAMENTO && treinando) {
            tempo_decorrido = 0;
            entidades_vivas = 0;
        }
        if (not entidades_vivas) {
            entidades_vivas = N_AGENTS;
            double max_pontuacao = 0;
            if (treinando) {
                QuickSort(gen_atual.agents, 0, N_AGENTS - 1);
                max_pontuacao = gen_atual.agents[N_AGENTS - 1].pontuacao;
                printf("geracao %zu: %lf\n", gen_atual.number, max_pontuacao);
                Evolve(gen_atual, next_gen);
                
            }
            ResetarTargets(targets);
            ResetAgents(gen_atual.agents);
            
            text.setString(std::to_string(++gen_atual.number));
            fprintf_s(arq_geracao, "%lf %zu \n", max_pontuacao, gen_atual.number);
            tempo_decorrido = 0;
        }

        for (agent_t& agent : gen_atual.agents)
            agent.drone.Draw(&window, assets, true);
        
            
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------
        tempo_decorrido += delta_t;
        if(not treinando)
            window.display();
    }
#ifdef _WIN32
    SetThreadExecutionState(ES_CONTINUOUS);
#endif
	return 0;
}