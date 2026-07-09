#pragma once
#include "neuralnet.h"
#include "drone.h"
#include "Target.h"
#include "assets.h"
#include "configs.h"

typedef struct {
    bool alive;
    double pontuacao;
    Drone drone;
    NeuralNetwork net;
} agent_t;

typedef struct {
    size_t number;
    agent_t agents[N_AGENTS];
} generation_t;

void troca(agent_t v[], int i, int j);
int Partition(agent_t v[], int comeco, int fim);
void QuickSort(agent_t v[], int min, int max);
void ResetAgents(agent_t v[]);
double AvaliarPontuacao(vector_t posicao_drone, vector_t posicao_target);
void ControlarDrones(agent_t v[], Target &target);

void Evolve(generation_t& gen_atual, generation_t& next_gen);