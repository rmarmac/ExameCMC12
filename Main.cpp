#include <iostream>
#include "neuralnet.h"
#include "fileop.h"
#include "drone.h"
#include "TimeManager.h"
#include "Target.h"
#include <cstring>
#include <SFML/Graphics.hpp>
#include <stdlib.h>

#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 1000
#define N_AGENTS 5

constexpr double min_dist_sqr = 200*200;

typedef struct entidade_t {
    bool alive;
    Drone drone;
} entidade_t;

typedef struct agent_t {
    NeuralNetwork net;
    double pontuacao;
} agent_t;


void troca(agent_t v[], int i, int j)
{
    agent_t aux;
    memcpy(&aux, v + i, sizeof(agent_t));
    memcpy(v + i, v + j, sizeof(agent_t));
    memcpy(v + j, &aux, sizeof(agent_t));
}

int Partition(agent_t v[], int comeco, int fim)
{
    agent_t pivot = v[comeco];
    int l = comeco + 1;
    int r = fim;
    while (1)
    {
        while (l < fim && v[l].pontuacao < pivot.pontuacao)
            ++l;
        while (r >= l && v[r].pontuacao >= pivot.pontuacao)
            --r;
        if (l >= r)
            break;
        troca(v, l, r);
    }
    v[comeco] = v[r];
    v[r] = pivot;
    return r;
}

void QuickSort(agent_t v[], int min, int max)
{
    int p;
    if (min < max)
    {
        p = Partition(v, min, max);
        QuickSort(v, min, p - 1);
        QuickSort(v, p + 1, max);
    }
}



//NeuralNetwork net("Rede1.txt");
Assets assets;
agent_t agents[N_AGENTS];
entidade_t entidades[N_AGENTS];
Target target(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

void ControlarDrones() {
    float inputs[input_size];
    vector_t posicao_target = target.GetPosition();
    for (size_t i = 0; i < N_AGENTS; i++)
        if(entidades[i].alive){
            vector_t posicao_drone = entidades[i].drone.GetPosition();
            inputs[0] = (float)(posicao_target.x - posicao_drone.x);
            inputs[1] = (float)(posicao_target.y - posicao_drone.y);
            inputs[2] = (float)entidades[i].drone.GetOmega();
            inputs[3] = (float)entidades[i].drone.GetTheta();

            agents[i].net.Forward_Pass(inputs);
            entidades[i].drone.ApplyForces(agents[i].net.outputs[0], agents[i].net.outputs[1]);
        }
}

void ResetEntidades() {
    for (entidade_t& entidade : entidades) {
        entidade.drone.Reset(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        entidade.alive = true;
    }
}

void ResetAgents() {
    for (agent_t& agent : agents)
        agent.pontuacao = 0;
}

double AvaliarPontuacao(vector_t posicao_drone, vector_t posicao_target) {
    double delta_x = posicao_target.x - posicao_drone.x;
    double delta_y = posicao_target.y - posicao_drone.y;
    double distance_sqr = delta_x * delta_x + delta_y * delta_y;
    if (distance_sqr < min_dist_sqr)
        distance_sqr = min_dist_sqr;
    return (WINDOW_WIDTH * WINDOW_WIDTH + WINDOW_HEIGHT * WINDOW_HEIGHT) / distance_sqr;
}

int main() {
    TimeManager time_manager;
    
    double delta_t = 0;

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "TEST");
    window.setFramerateLimit(240);
    bool treinando = false;
    size_t entidades_vivas = N_AGENTS;

    ResetEntidades();
    ResetAgents();
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            treinando = true;
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------
        // Lógica do jogo
        








        // ------------------------------------------------------------------------------------------------------------------------------------------------------------
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------
        // Display na tela
        window.clear(sf::Color(31, 64, 62));
        target.Draw(&window, assets);
    
        
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------
        // Update da física
        delta_t = time_manager.Update();


        if (treinando) {
            ControlarDrones();
            for (size_t i = 0; i < N_AGENTS; i++) {
                if (entidades[i].alive) {
                    agents[i].pontuacao += delta_t * AvaliarPontuacao(entidades[i].drone.GetPosition(), target.GetPosition());
                    entidades[i].drone.Update(delta_t);
                    vector_t position = entidades[i].drone.GetPosition();
                    if (position.x < 0 || position.x > WINDOW_WIDTH || position.y < 0 || position.y > WINDOW_HEIGHT) {
                        entidades[i].alive = false;
                        --entidades_vivas;
                    }
                }
            }
        }
        for (entidade_t& entidade : entidades)
            entidade.drone.Draw(&window, assets);
        if (not entidades_vivas) {
            entidades_vivas = N_AGENTS;
            QuickSort(agents, 0, N_AGENTS - 1);
            for (agent_t& agent : agents)
                printf("%lf ", agent.pontuacao);
            ResetAgents();
            ResetEntidades();
            printf("\n");
        }
            
        // ------------------------------------------------------------------------------------------------------------------------------------------------------------
        
        window.display();
    }
	return 0;
}