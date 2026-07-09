#pragma once
#include <random>
#include <math.h>
#include "Agents.h"

void troca(agent_t v[], int i, int j)
{
    agent_t aux;
    const size_t size = sizeof(agent_t);
    memcpy(&aux, v + i, size);
    memcpy(v + i, v + j, size);
    memcpy(v + j, &aux, size);
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

void ResetAgents(agent_t v[]) {
    for (size_t i = 0 ; i < N_AGENTS ; i++) {
        v[i].alive = true;
        v[i].drone.Reset(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        v[i].pontuacao = 0;
    }
}
// 
double AvaliarPontuacao(Drone drone, vector_t posicao_target) {
    vector_t posicao_drone = drone.GetPosition();
    //vector_t velocidade = drone.GetVelocity();
    //double modulo_velocidade = sqrt(velocidade.x * velocidade.x + velocidade.y * velocidade.y);
    double delta_x = posicao_target.x - posicao_drone.x;
    double delta_y = posicao_target.y - posicao_drone.y;
    double distance_sqr = delta_x * delta_x + delta_y * delta_y;
    double max_distance = WINDOW_WIDTH * WINDOW_WIDTH + WINDOW_HEIGHT * WINDOW_HEIGHT;
    
    return 2 * max_distance/(max_distance + distance_sqr);
}



void ControlarDrones(agent_t v[], Target& target) {
    float inputs[input_size];
    vector_t posicao_target = target.GetPosition();
    for (size_t i = 0; i < N_AGENTS; i++)
        if (v[i].alive) {
            vector_t posicao_drone = v[i].drone.GetPosition();
            vector_t velocidade_drone = v[i].drone.GetVelocity();
            inputs[0] = (float)(posicao_target.x - posicao_drone.x) / WINDOW_WIDTH;
            inputs[1] = (float)(posicao_target.y - posicao_drone.y) / WINDOW_HEIGHT;
            //inputs[0] = (float)(posicao_drone.x) / WINDOW_WIDTH;
            //inputs[1] = (float)(posicao_target.y) / WINDOW_HEIGHT;
            inputs[2] = (float)v[i].drone.GetOmega() / PI;
            inputs[3] = (float)v[i].drone.GetTheta() / PI;
            inputs[4] = velocidade_drone.x;
            inputs[5] = velocidade_drone.y;

            v[i].net.Forward_Pass(inputs);
            double torque = MAX_TORQUE * v[i].net.outputs[0];
            double F = MAX_FORCE * v[i].net.outputs[1];
            v[i].drone.ApplyForces(F/2 + torque / LENGTH, F / 2 - torque / LENGTH);
        }
}

void Evolve(generation_t& gen_atual, generation_t& next_gen) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distrib(0.0, (float)(N_AGENTS*N_AGENTS));
    float random_number = 0;
    int selecao1 = 0, selecao2 = 0;

    for (int i = 0; i < N_AGENTS - PRESERVE; i++) {
        random_number = distrib(gen);
        for (int j = 1; j <= N_AGENTS; j++)
            if (random_number < j * j) {
                selecao1 = j - 1;
                break;
            }
        random_number = distrib(gen);
        for (int j = 1; j <= N_AGENTS; j++)
            if (random_number < j * j) {
                selecao2 = j - 1;
                break;
            }
        

        NeuralNetwork aux(gen_atual.agents[selecao1].net, gen_atual.agents[selecao2].net);
        aux.TransferData(next_gen.agents[i].net);
    }
    for (int i = N_AGENTS - PRESERVE; i < N_AGENTS; i++)
        gen_atual.agents[i].net.TransferData(next_gen.agents[i].net);
    
}