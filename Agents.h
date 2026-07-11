#pragma once
#include "neuralnet.h"
#include "drone.h"
#include "Target.h"
#include "assets.h"
#include "configs.h"
//-------------------------------------------------------------------------------------------------------
/// Estrutura de um agente, possuindo sua própria rede neural, um drone para controlar, e informações adicionais
/// para saber qual seu alvo, se está vivo e sua pontuação.
typedef struct {
    Drone drone;
    NeuralNetwork net;
    double pontuacao;
    size_t nivel_target;
    bool alive;
} agent_t;
//-------------------------------------------------------------------------------------------------------
/// Estrutura de uma geração do algoritmo genético
typedef struct {
    size_t number;
    agent_t agents[N_AGENTS];
} generation_t;

void troca(agent_t v[], int i, int j);
int Partition(agent_t v[], int comeco, int fim);
void QuickSort(agent_t v[], int min, int max);
void ResetAgents(agent_t v[]);
double AvaliarPontuacao(Drone drone, vector_t posicao_target);
void ControlarDrones(agent_t v[], Target targets[], double delta_t, size_t& entidades_vivas);
void SeguirMouse(agent_t v[], Target& mouse_target, double delta_t, size_t& entidades_vivas);

void Evolve(generation_t& gen_atual, generation_t& next_gen);

void TreinoA(agent_t& agent, Target& test_target, double tempo_decorrido, double delta_t);
void TreinoB(agent_t& agent, Target& test_target, double tempo_decorrido, double delta_t);