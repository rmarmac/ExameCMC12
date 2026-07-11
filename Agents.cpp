#pragma once
#include <random>
#include <math.h>
#include "Agents.h"

constexpr int tournament_selection_number = 4;
const double max_distance = sqrt(WINDOW_WIDTH * WINDOW_WIDTH + WINDOW_HEIGHT * WINDOW_HEIGHT) / SCALE;

//-------------------------------------------------------------------------------------------------------
/// Função auxiliar para trocar dois elementos de posição no vetor
void troca(agent_t v[], int i, int j)
{
    agent_t aux;
    const size_t size = sizeof(agent_t);
    memcpy(&aux, v + i, size);
    memcpy(v + i, v + j, size);
    memcpy(v + j, &aux, size);
}
//-------------------------------------------------------------------------------------------------------
/// Função auxiliar para realizar a partição do Quicksort
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
//-------------------------------------------------------------------------------------------------------
/// Ordenando os elementos por pontuação / custo
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
//-------------------------------------------------------------------------------------------------------
/// Ordenando os elementos por pontuação / custo
void ResetAgents(agent_t v[]) {
    for (size_t i = 0 ; i < N_AGENTS ; i++) {
        v[i].alive = true;
        v[i].drone.Reset(0, 0);
        v[i].pontuacao = 0;
        v[i].nivel_target = 0;
    }
}
//-------------------------------------------------------------------------------------------------------
/// Atribui uma pontuação com base nas informações do drone e da posição de um target
double AvaliarPontuacao(Drone drone, vector_t posicao_target) {
    vector_t posicao_drone = drone.GetPosition();
    //vector_t velocidade = drone.GetVelocity();
    double delta_x = posicao_target.x - posicao_drone.x;
    double delta_y = posicao_target.y - posicao_drone.y;
    double distance = sqrt(delta_x * delta_x + delta_y * delta_y);
    
    return 2 * max_distance / (max_distance + distance);
    //double dot_product = delta_x * velocidade.x + delta_y * velocidade.y;
    //return dot_product + 2 * max_distance / (max_distance + distance);
}
//-------------------------------------------------------------------------------------------------------
/// Controla todos os drones com base na saida das redes neurais de cada agente
void ControlarDrones(agent_t v[], Target targets[], double delta_t, size_t& entidades_vivas) {
    float inputs[input_size];
    vector_t posicoes_targets[NUMBER_OF_TARGETS];
    for (size_t i = 0; i < NUMBER_OF_TARGETS; i++)
        posicoes_targets[i] = targets[i].GetPosition();
    for (size_t i = 0; i < N_AGENTS; i++)
        if (v[i].alive) {
            vector_t target_position = posicoes_targets[v[i].nivel_target];
            vector_t posicao_drone = v[i].drone.GetPosition();
            vector_t velocidade_drone = v[i].drone.GetVelocity();
            inputs[0] = (float)(target_position.x - posicao_drone.x) * SCALE / WINDOW_WIDTH;
            inputs[1] = (float)(target_position.y - posicao_drone.y) * SCALE / WINDOW_HEIGHT;
            //inputs[0] = (float)(posicao_drone.x) / WINDOW_WIDTH;
            //inputs[1] = (float)(posicao_target.y) / WINDOW_HEIGHT;
            inputs[2] = (float)v[i].drone.GetOmega() / PI;
            inputs[3] = (float)v[i].drone.GetTheta() / PI;
            inputs[4] = velocidade_drone.x;
            inputs[5] = velocidade_drone.y;

            v[i].net.Forward_Pass(inputs);
            double torque = MAX_TORQUE * v[i].net.outputs[0];
            double F = MASS * GRAVITY + MAX_FORCE * v[i].net.outputs[1];
            v[i].drone.ApplyForces(F/2 + torque / LENGTH, F / 2 - torque / LENGTH);

            v[i].drone.Update(delta_t);
            v[i].pontuacao += delta_t * AvaliarPontuacao(v[i].drone, target_position);

            if (abs(posicao_drone.x * SCALE) > WINDOW_WIDTH / 2 || abs(posicao_drone.y * SCALE) > WINDOW_HEIGHT / 2) {
                v[i].alive = false;
                v[i].pontuacao -= TEMPO_TREINAMENTO / 2;
                --entidades_vivas;
            }
            double delta_x = posicao_drone.x - target_position.x;
            double delta_y = posicao_drone.y - target_position.y;
            if (delta_x * delta_x + delta_y * delta_y < TAMANHO_ALVO * TAMANHO_ALVO) {
                v[i].pontuacao += 3 * TEMPO_TREINAMENTO;
                v[i].nivel_target = (v[i].nivel_target + 1) % NUMBER_OF_TARGETS;
            }
        }
}
//-------------------------------------------------------------------------------------------------------
/// Faz os agentes seguirem a posição do mouse, para testar o comportamento dos drones
void SeguirMouse(agent_t v[], Target& mouse_target, double delta_t, size_t& entidades_vivas) {
    float inputs[input_size];
    vector_t target_position = mouse_target.GetPosition();
    for (size_t i = 0; i < N_AGENTS; i++)
        if (v[i].alive) {
            vector_t posicao_drone = v[i].drone.GetPosition();
            vector_t velocidade_drone = v[i].drone.GetVelocity();
            inputs[0] = (float)(target_position.x - posicao_drone.x) * SCALE / WINDOW_WIDTH;
            inputs[1] = (float)(target_position.y - posicao_drone.y) * SCALE / WINDOW_HEIGHT;
            //inputs[0] = (float)(posicao_drone.x) / WINDOW_WIDTH;
            //inputs[1] = (float)(posicao_target.y) / WINDOW_HEIGHT;
            inputs[2] = (float)v[i].drone.GetOmega() / PI;
            inputs[3] = (float)v[i].drone.GetTheta() / PI;
            inputs[4] = velocidade_drone.x;
            inputs[5] = velocidade_drone.y;

            v[i].net.Forward_Pass(inputs);
            double torque = MAX_TORQUE * v[i].net.outputs[0];
            double F = MASS * GRAVITY + MAX_FORCE * v[i].net.outputs[1];
            v[i].drone.ApplyForces(F / 2 + torque / LENGTH, F / 2 - torque / LENGTH);

            v[i].drone.Update(delta_t);
            v[i].pontuacao += delta_t * AvaliarPontuacao(v[i].drone, target_position);

            if (abs(posicao_drone.x * SCALE) > WINDOW_WIDTH / 2 || abs(posicao_drone.y * SCALE) > WINDOW_HEIGHT / 2) {
                v[i].alive = false;
                v[i].pontuacao -= TEMPO_TREINAMENTO;
                --entidades_vivas;
            }

        }
}
//-------------------------------------------------------------------------------------------------------
/// Evolução do algoritmo genético
//void Evolve(generation_t& gen_atual, generation_t& next_gen) {
//    std::random_device rd;
//    std::mt19937 gen(rd());
//    std::uniform_real_distribution<float> distrib(0.0, (float)(N_AGENTS*N_AGENTS));
//    float random_number = 0;
//    int selecao1 = 0, selecao2 = 0;
//
//    for (int i = 0; i < N_AGENTS - PRESERVE; i++) {
//        random_number = distrib(gen);
//        for (int j = 1; j <= N_AGENTS; j++)
//            if (random_number < j * j) {
//                selecao1 = j - 1;
//                break;
//            }
//        random_number = distrib(gen);
//        for (int j = 1; j <= N_AGENTS; j++)
//            if (random_number < j * j) {
//                selecao2 = j - 1;
//                break;
//            }
//        
//
//        NeuralNetwork aux(gen_atual.agents[selecao1].net, gen_atual.agents[selecao2].net);
//        aux.TransferData(next_gen.agents[i].net);
//    }
//    for (size_t i = 0; i < N_AGENTS - PRESERVE; i++)
//        next_gen.agents[i].net.TransferData(gen_atual.agents[i].net);
//}
void Evolve(generation_t& gen_atual, generation_t& next_gen) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(0, N_AGENTS - 1);
    int random_number = 0;
    int selecao1 = 0, selecao2 = 0;

    for (int i = 0; i < N_AGENTS - PRESERVE; i++) {
        selecao1 = distrib(gen);
        for (int j = 0; j < tournament_selection_number - 1; j++) {
            random_number = distrib(gen);
            if (gen_atual.agents[random_number].pontuacao > gen_atual.agents[selecao1].pontuacao)
                selecao1 = random_number;
        }
            
        selecao2 = distrib(gen);
        for (int j = 0; j < tournament_selection_number - 1; j++) {
            random_number = distrib(gen);
            if (gen_atual.agents[random_number].pontuacao > gen_atual.agents[selecao2].pontuacao)
                selecao2 = random_number;
        }

        NeuralNetwork aux(gen_atual.agents[selecao1].net, gen_atual.agents[selecao2].net);
        aux.TransferData(next_gen.agents[i].net);
    }
    for (size_t i = 0; i < N_AGENTS - PRESERVE; i++)
        next_gen.agents[i].net.TransferData(gen_atual.agents[i].net);
}
//-------------------------------------------------------------------------------------------------------
void TreinoA(agent_t& agent, Target& test_target, double tempo_decorrido, double delta_t) {
    test_target.SetPosition(0, -1);
    float inputs[input_size];
    vector_t target_position = test_target.GetPosition();
    
    vector_t posicao_drone = agent.drone.GetPosition();
    vector_t velocidade_drone = agent.drone.GetVelocity();
    inputs[0] = (float)(target_position.x - posicao_drone.x) * SCALE / WINDOW_WIDTH;
    inputs[1] = (float)(target_position.y - posicao_drone.y) * SCALE / WINDOW_HEIGHT;
    //inputs[0] = (float)(posicao_drone.x) / WINDOW_WIDTH;
    //inputs[1] = (float)(posicao_target.y) / WINDOW_HEIGHT;
    inputs[2] = (float)agent.drone.GetOmega() / PI;
    inputs[3] = (float)agent.drone.GetTheta() / PI;
    inputs[4] = velocidade_drone.x;
    inputs[5] = velocidade_drone.y;

    agent.net.Forward_Pass(inputs);
    double torque = MAX_TORQUE * agent.net.outputs[0];
    double F = MASS * GRAVITY + MAX_FORCE * agent.net.outputs[1];
    agent.drone.ApplyForces(F / 2 + torque / LENGTH, F / 2 - torque / LENGTH);

    agent.drone.Update(delta_t);
}
//-------------------------------------------------------------------------------------------------------
void TreinoB(agent_t& agent, Target& test_target, double tempo_decorrido, double delta_t) {
    if(tempo_decorrido > 1)
        test_target.SetPosition(1, -1);
    else
        test_target.SetPosition(0, -1);
    float inputs[input_size];
    vector_t target_position = test_target.GetPosition();

    vector_t posicao_drone = agent.drone.GetPosition();
    vector_t velocidade_drone = agent.drone.GetVelocity();
    inputs[0] = (float)(target_position.x - posicao_drone.x) * SCALE / WINDOW_WIDTH;
    inputs[1] = (float)(target_position.y - posicao_drone.y) * SCALE / WINDOW_HEIGHT;
    //inputs[0] = (float)(posicao_drone.x) / WINDOW_WIDTH;
    //inputs[1] = (float)(posicao_target.y) / WINDOW_HEIGHT;
    inputs[2] = (float)agent.drone.GetOmega() / PI;
    inputs[3] = (float)agent.drone.GetTheta() / PI;
    inputs[4] = velocidade_drone.x;
    inputs[5] = velocidade_drone.y;

    agent.net.Forward_Pass(inputs);
    double torque = MAX_TORQUE * agent.net.outputs[0];
    double F = MASS * GRAVITY + MAX_FORCE * agent.net.outputs[1];
    agent.drone.ApplyForces(F / 2 + torque / LENGTH, F / 2 - torque / LENGTH);

    agent.drone.Update(delta_t);
}