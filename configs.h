#pragma once

//-------------------------------------------------------------------------------------------------------
/// Arquivo de definição de parâmetros globais do código

#define PI 3.14159265358979
#define GRAVITY 9.81
#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define MAX_DELTA_T 0.006
#define N_AGENTS 100
#define PRESERVE 20
#define MUTATION_THRESHOLD 0.005f

#define TEMPO_TREINAMENTO 45
#define NUMBER_OF_TARGETS 5
#define TAMANHO_ALVO 0.04

/// Configurações do Drone
#define LENGTH 0.2
#define HEIGHT 0.02
#define	J_DRONE 0.04
#define MASS 0.5

constexpr double MAX_FORCE = (MASS * GRAVITY);
constexpr double MAX_TORQUE = (MASS* GRAVITY / LENGTH / 4);
//#define MAX_FORCE 1
//#define MAX_TORQUE 1

// scale define quantos pixels representam 1 metro
#define SCALE 400