#pragma once

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "funcoes_fwdp_e_custo.h"
#include "utils.h"




///  NEURAL NETWORK PARAMETERS  -------------------------------------------------------------------------

constexpr int neural_net_array[] = { 4,5,4,3,2 }; // Define quantos neurons havera por camada
// funcoes disponiveis para usar: Tanh, Leaky_ReLu, Linear, ReLu
#define       FUNCTION_FWDP     Tanh   // Define qual funcao sera usada para a operacao forward_pass
#define       FUNCTION_OUTPUT   Tanh             // Define qual funcao sera usada para a ativacao na saida
// funcoes disponiveis para usar: Quadratic_error
#define       ERROR_FUNCTION    Quadratic_error      // Define qual funcar sera usada para calcular o erro
constexpr float learning_rate = 0.03f;                       // Define a taxa de aprendizado da rede neural
constexpr float inertia = 0.1f;            // Define o quanto a rede neural utilizara o gradiente anterior
constexpr float gamma = 1.0f;             // Define a taxa de desconto póstero para reinforcement learning

///------------------------------------------------------------------------------------------------------
constexpr int neural_net_size = sizeof(neural_net_array) / sizeof(int);
//-------------------------------------------------------------------------------------------------------
constexpr int output_size = neural_net_array[neural_net_size - 1];
constexpr int input_size = neural_net_array[0];
//-------------------------------------------------------------------------------------------------------
constexpr int weights_array_size = calc_weights_array_size(neural_net_array, neural_net_size);
constexpr int neurons_array_size = calc_neurons_array_size(neural_net_array, neural_net_size);
//-------------------------------------------------------------------------------------------------------
class NeuralNetwork {
public:
	NeuralNetwork();
	NeuralNetwork(const char* name_of_file);
	NeuralNetwork(NeuralNetwork& father, NeuralNetwork& mother);
	void Fwdp_and_Back_propagate(const float inputs[], const float desired_outputs[]);
	void Train_Batch(const float inputs[][input_size], const float desired_outputs[][output_size], const int batch_size);
	void Forward_Pass(const float inputs[]);
	void Back_Propagate(const float desired_outputs[]);
	void Print_weights() const;
	void Print_neurons() const;
	void Print_outputs() const;
	void Save_To_File(const char* name_of_file) const;
	void TransferData(NeuralNetwork& other_net) const;
	float* outputs;
	float weights[weights_array_size];
	int   index_max_output;
private:
	int   n_offsets[neural_net_size];
	int   w_offsets[neural_net_size - 1];
	float neurons[neurons_array_size];
	float derivatives_neurons[neurons_array_size] = { 0 };
	float derivatives_weights[weights_array_size] = { 0 };
	void (*error_function)(const float[], const float[], float[], const int) = ERROR_FUNCTION;
	FUNCTION_FWDP function_fwdp;
	FUNCTION_OUTPUT function_output;
};