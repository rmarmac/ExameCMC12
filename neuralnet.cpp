#include "neuralnet.h"
#include "configs.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <random>

#define w_position(matrix,line,column) (w_offsets[matrix] + line * (neural_net_array[matrix] + 1) + column) 
#define n_position(layer,line) (n_offsets[layer] + line)
//-------------------------------------------------------------------------------------------------------
/// Construtor padrao da rede neural
NeuralNetwork::NeuralNetwork() {
	// Setup para variáveis aleatórias
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distrib(-2.0, 2.0);
	int i;
	index_max_output = 0;
	for (i = 0; i < weights_array_size; i++)
		weights[i] = (float)distrib(gen);
	w_offsets[0] = n_offsets[0] = 0;
	for (i = 0; i < neural_net_size - 2; i++) {
		w_offsets[i + 1] = (neural_net_array[i] + 1) * neural_net_array[i + 1] + w_offsets[i];
		n_offsets[i + 1] = neural_net_array[i] + n_offsets[i] + 1;
		// Neurons relativos ao bias tem valor constante e igual a 1.0f
		neurons[n_offsets[i + 1] - 1] = 1.0f;
	}
	n_offsets[neural_net_size - 1] = neural_net_array[neural_net_size - 2] + n_offsets[neural_net_size - 2] + 1;
	neurons[n_offsets[neural_net_size - 1] - 1] = 1.0f;
	outputs = &(neurons[n_position(neural_net_size - 1, 0)]);
}
//-------------------------------------------------------------------------------------------------------
/// Construtor da rede neural a partir de um arquivo cujo nome é dado por "name of file"
NeuralNetwork::NeuralNetwork(const char* name_of_file) {
	FILE* arquivo;
	if(fopen_s(&arquivo, name_of_file, "r")) exit(1);
	int i;
	index_max_output = 0;
	for (i = 0; i < weights_array_size; i++)
		if(!fscanf_s(arquivo, "%f", &(weights[i]))) exit(2);
	fclose(arquivo);
	w_offsets[0] = n_offsets[0] = 0;
	for (i = 0; i < neural_net_size - 2; i++) {
		w_offsets[i + 1] = (neural_net_array[i] + 1) * neural_net_array[i + 1] + w_offsets[i];
		n_offsets[i + 1] = neural_net_array[i] + n_offsets[i] + 1;
		// Neurons relativos ao bias tem valor constante e igual a 1.0f
		neurons[n_offsets[i + 1] - 1] = 1.0f;
	}
	n_offsets[neural_net_size - 1] = neural_net_array[neural_net_size - 2] + n_offsets[neural_net_size - 2] + 1;
	neurons[n_offsets[neural_net_size - 1] - 1] = 1.0f;
	outputs = &(neurons[n_position(neural_net_size - 1, 0)]);
}
//-------------------------------------------------------------------------------------------------------
/// Combinação para algoritmo genético
NeuralNetwork::NeuralNetwork(NeuralNetwork& father, NeuralNetwork& mother) {
	int i;
	// Setup para variáveis aleatórias
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distrib(0.0, 1.0);
	index_max_output = 0;
	const float threshold = MUTATION_THRESHOLD;
	for (i = 0; i < (weights_array_size >> 1); i++)
	{
		if ((float)distrib(gen) < threshold)
			weights[i] = (float)distrib(gen) * 4.f - 2.f;
		else
			weights[i] = father.weights[i];
	}
	for (i = (weights_array_size >> 1); i < weights_array_size; i++)
	{
		if ((float)distrib(gen) < threshold)
			weights[i] = (float)distrib(gen) * 4.f - 2.f;
		else
			weights[i] = mother.weights[i];
	}
	w_offsets[0] = n_offsets[0] = 0;
	for (i = 0; i < neural_net_size - 2; i++) {
		w_offsets[i + 1] = (neural_net_array[i] + 1) * neural_net_array[i + 1] + w_offsets[i];
		n_offsets[i + 1] = neural_net_array[i] + n_offsets[i] + 1;
		// Neurons relativos ao bias tem valor constante e igual a 1.0f
		neurons[n_offsets[i + 1] - 1] = 1.0f;
	}
	n_offsets[neural_net_size - 1] = neural_net_array[neural_net_size - 2] + n_offsets[neural_net_size - 2] + 1;
	neurons[n_offsets[neural_net_size - 1] - 1] = 1.0f;
	outputs = &(neurons[n_position(neural_net_size - 1, 0)]);
}
//-------------------------------------------------------------------------------------------------------
/// Imprime na tela os atuais weights (pesos das conexoes) da rede neural
void NeuralNetwork::Print_weights() const {
	for (int i = 0; i < neural_net_size - 1; i++) {
		for (int j = 0; j < neural_net_array[i + 1]; j++) {
			for (int k = 0; k < neural_net_array[i] + 1; k++)
				std::cout << weights[w_position(i, j, k)] << " ";
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}
}
//-------------------------------------------------------------------------------------------------------
/// Salva a rede neural para um arquivo cujo nome será dado pela variável "name of file"
void NeuralNetwork::Save_To_File(const char* name_of_file) const {
	FILE* arquivo;
	if(fopen_s(&arquivo, name_of_file, "w")) exit(1);
	for (int i = 0; i < neural_net_size - 1; i++) {
		for (int j = 0; j < neural_net_array[i + 1]; j++) {
			for (int k = 0; k < neural_net_array[i] + 1; k++)
				fprintf_s(arquivo, "%f ", weights[w_position(i, j, k)]);
		}
	}
	fclose(arquivo);
}
//-------------------------------------------------------------------------------------------------------
/// Transfere os dados de si para outra rede neural
void NeuralNetwork::TransferData(NeuralNetwork& other_net) const {
	memcpy(other_net.weights, weights, weights_array_size * sizeof(float));
}
//-------------------------------------------------------------------------------------------------------
/// Imprime na tela as atuais saidas produzidas por cada neuron
void NeuralNetwork::Print_neurons() const {
	for (int i = 0; i < neural_net_size; i++) {
		for (int j = 0; j < neural_net_array[i] + (i != neural_net_size - 1); j++)
			std::cout << neurons[n_position(i, j)] << " ";
		std::cout << std::endl;
	}
}
//-------------------------------------------------------------------------------------------------------
/// Imprime as ultimas saidas da rede neural (exige que, antes, tenha sido feito um forward_pass)
void NeuralNetwork::Print_outputs() const {
	for (int i = 0; i < output_size; i++)
		std::cout << outputs[i] << " ";
	std::cout << std::endl;
}
//-------------------------------------------------------------------------------------------------------
/// Recebe inputs e os passa pela rede neural para produzir um resultado na saida
void NeuralNetwork::Forward_Pass(const float inputs[]) {
	int i;
	for (i = 0; i < input_size; i++)
		neurons[i] = inputs[i];
	// Para cada camada interna, propaga para frente os valores recebidos usando a função definida para function_fwdp
	for (i = 1; i < neural_net_size - 1; i++)
		for (int j = 0; j < neural_net_array[i]; j++) {
			neurons[n_position(i, j)] = 0;
			for (int k = 0; k < neural_net_array[i - 1] + 1; k++)
				neurons[n_position(i, j)] += neurons[n_position(i - 1, k)] * weights[w_position(i - 1, j, k)];
			neurons[n_position(i, j)] = function_fwdp.function(neurons[n_position(i, j)]);
		}
	float max_output;
	// A camada de saída possui uma função não linear própria, ao gosto do usuário, e por isso é tratada separadamente.
	for (int j = 0; j < output_size; j++) {
		outputs[j] = 0;
		for (int k = 0; k < neural_net_array[neural_net_size - 2] + 1; k++)
			outputs[j] += neurons[n_position(neural_net_size - 2, k)] *
			weights[w_position(neural_net_size - 2, j, k)];
		outputs[j] = function_output.function(outputs[j]);
		if (j == 0) {
			max_output = outputs[0];
			index_max_output = 0;
		} else if (outputs[j] > max_output) {
			max_output = outputs[j];
			index_max_output = j;
		}
	}
}
//-------------------------------------------------------------------------------------------------------
/// Recebe inputs e os passa pela rede neural para produzir um resultado na saida E modifica os pesos por propagacao reversa
void NeuralNetwork::Fwdp_and_Back_propagate(const float inputs[], const float desired_outputs[]) {
	int i;
	for (i = 0; i < input_size; i++)
		neurons[i] = inputs[i];
	for (i = 1; i < neural_net_size - 1; i++)
		for (int j = 0; j < neural_net_array[i]; j++) {
			neurons[n_position(i, j)] = 0;
			for (int k = 0; k < neural_net_array[i - 1] + 1; k++)
				neurons[n_position(i, j)] += neurons[n_position(i - 1, k)] * weights[w_position(i - 1, j, k)];
			neurons[n_position(i, j)] = function_fwdp.function(neurons[n_position(i, j)]);
		}
	float max_output;
	for (int j = 0; j < output_size; j++) {
		outputs[j] = 0;
		for (int k = 0; k < neural_net_array[neural_net_size - 2] + 1; k++)
			outputs[j] += neurons[n_position(neural_net_size - 2, k)] *
			weights[w_position(neural_net_size - 2, j, k)];
		outputs[j] = function_output.function(outputs[j]);
		if (j == 0) {
			max_output = outputs[0];
			index_max_output = 0;
		}
		else if (outputs[j] > max_output) {
			max_output = outputs[j];
			index_max_output = j;
		}
	}

	error_function(
		outputs,
		desired_outputs,
		&(derivatives_neurons[n_position(neural_net_size - 1, 0)]),
		output_size);

	for (i = 0; i < neural_net_array[neural_net_size - 2]; i++)
		derivatives_neurons[n_position(neural_net_size - 2, i)] = 0.0f;
	for (i = 0; i < output_size; i++) {
		float aux = derivatives_neurons[n_position(neural_net_size - 1, i)] *
			function_output.derivative(outputs[i]);
		for (int j = 0; j < neural_net_array[neural_net_size - 2] + 1; j++) {
			derivatives_weights[w_position(neural_net_size - 2, i, j)] = aux * neurons[n_position(neural_net_size - 2, j)] +
				inertia * derivatives_weights[w_position(neural_net_size - 2, i, j)];
			derivatives_neurons[n_position(neural_net_size - 2, j)] += aux * weights[w_position(neural_net_size - 2, i, j)];
			// Update dos valores dos pesos da rede neural (ultima camada)
			weights[w_position(neural_net_size - 2, i, j)] -= learning_rate * derivatives_weights[w_position(neural_net_size - 2, i, j)];
		}
	}
	for (int layer = neural_net_size - 3; layer > -1; layer--) {
		for (i = 0; i < neural_net_array[layer]; i++)
			derivatives_neurons[n_position(layer, i)] = 0.0f;
		for (i = 0; i < neural_net_array[layer + 1]; i++) {
			float aux = derivatives_neurons[n_position(layer + 1, i)] *
				function_fwdp.derivative(neurons[n_position(layer + 1, i)]);
			for (int j = 0; j < neural_net_array[layer] + 1; j++) {
				derivatives_weights[w_position(layer, i, j)] = aux * neurons[n_position(layer, j)] +
					inertia * derivatives_weights[w_position(layer, i, j)];
				derivatives_neurons[n_position(layer, j)] += aux * weights[w_position(layer, i, j)];
				// Update dos valores dos pesos da rede neural
				weights[w_position(layer, i, j)] -= learning_rate * derivatives_weights[w_position(layer, i, j)];
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------------
/// Realiza apenas a propagacao reversa
void NeuralNetwork::Back_Propagate(const float desired_outputs[]) {
	int i;
	error_function(
		outputs,
		desired_outputs,
		&(derivatives_neurons[n_position(neural_net_size - 1, 0)]),
		output_size);

	for (i = 0; i < neural_net_array[neural_net_size - 2]; i++)
		derivatives_neurons[n_position(neural_net_size - 2, i)] = 0.0f;
	for (i = 0; i < output_size; i++) {
		float aux = derivatives_neurons[n_position(neural_net_size - 1, i)] *
			function_output.derivative(outputs[i]);
		for (int j = 0; j < neural_net_array[neural_net_size - 2] + 1; j++) {
			derivatives_weights[w_position(neural_net_size - 2, i, j)] = aux * neurons[n_position(neural_net_size - 2, j)] +
				inertia * derivatives_weights[w_position(neural_net_size - 2, i, j)];
			derivatives_neurons[n_position(neural_net_size - 2, j)] += aux * weights[w_position(neural_net_size - 2, i, j)];
			// Update dos valores dos pesos da rede neural (ultima camada)
			weights[w_position(neural_net_size - 2, i, j)] -= learning_rate * derivatives_weights[w_position(neural_net_size - 2, i, j)];
		}
	}
	for (int layer = neural_net_size - 3; layer > -1; layer--) {
		for (i = 0; i < neural_net_array[layer]; i++)
			derivatives_neurons[n_position(layer, i)] = 0.0f;
		for (i = 0; i < neural_net_array[layer + 1]; i++) {
			float aux = derivatives_neurons[n_position(layer + 1, i)] *
				function_fwdp.derivative(neurons[n_position(layer + 1, i)]);
			for (int j = 0; j < neural_net_array[layer] + 1; j++) {
				derivatives_weights[w_position(layer, i, j)] = aux * neurons[n_position(layer, j)] +
					inertia * derivatives_weights[w_position(layer, i, j)];
				derivatives_neurons[n_position(layer, j)] += aux * weights[w_position(layer, i, j)];
				// Update dos valores dos pesos da rede neural
				weights[w_position(layer, i, j)] -= learning_rate * derivatives_weights[w_position(layer, i, j)];
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------------
/// Recebe inputs e os passa pela rede neural para produzir um resultado na saida E modifica os pesos por propagacao reversa para um "batch" de informações
void NeuralNetwork::Train_Batch(const float inputs[][input_size], const float desired_outputs[][output_size], const int batch_size) {
	int i, iteration;
	float inv_batch_size = 1.f / batch_size;
	for (i = 0; i < weights_array_size; i++)
		derivatives_weights[i] = inertia * derivatives_weights[i];

	for (iteration = 0; iteration < batch_size; iteration++) {
		for (i = 0; i < input_size; i++)
			neurons[i] = inputs[iteration][i];
		for (i = 1; i < neural_net_size - 1; i++)
			for (int j = 0; j < neural_net_array[i]; j++) {
				neurons[n_position(i, j)] = 0;
				for (int k = 0; k < neural_net_array[i - 1] + 1; k++)
					neurons[n_position(i, j)] += neurons[n_position(i - 1, k)] * weights[w_position(i - 1, j, k)];
				neurons[n_position(i, j)] = function_fwdp.function(neurons[n_position(i, j)]);
			}
		float max_output;
		for (int j = 0; j < output_size; j++) {
			outputs[j] = 0;
			for (int k = 0; k < neural_net_array[neural_net_size - 2] + 1; k++)
				outputs[j] += neurons[n_position(neural_net_size - 2, k)] *
				weights[w_position(neural_net_size - 2, j, k)];
			outputs[j] = function_output.function(outputs[j]);
			if (j == 0) {
				max_output = outputs[0];
				index_max_output = 0;
			}
			else if (outputs[j] > max_output) {
				max_output = outputs[j];
				index_max_output = j;
			}
		}

		error_function(
			outputs,
			desired_outputs[iteration],
			&(derivatives_neurons[n_position(neural_net_size - 1, 0)]),
			output_size);

		for (i = 0; i < neural_net_array[neural_net_size - 2]; i++)
			derivatives_neurons[n_position(neural_net_size - 2, i)] = 0.0f;
		for (i = 0; i < output_size; i++) {
			float aux = derivatives_neurons[n_position(neural_net_size - 1, i)] *
				function_output.derivative(outputs[i]);
			for (int j = 0; j < neural_net_array[neural_net_size - 2] + 1; j++) {
				derivatives_weights[w_position(neural_net_size - 2, i, j)] += aux * neurons[n_position(neural_net_size - 2, j)];
				derivatives_neurons[n_position(neural_net_size - 2, j)] += aux * weights[w_position(neural_net_size - 2, i, j)];
			}
		}
		for (int layer = neural_net_size - 3; layer > -1; layer--) {
			for (i = 0; i < neural_net_array[layer]; i++)
				derivatives_neurons[n_position(layer, i)] = 0.0f;
			for (i = 0; i < neural_net_array[layer + 1]; i++) {
				float aux = derivatives_neurons[n_position(layer + 1, i)] *
					function_fwdp.derivative(neurons[n_position(layer + 1, i)]);
				for (int j = 0; j < neural_net_array[layer] + 1; j++) {
					derivatives_weights[w_position(layer, i, j)] += aux * neurons[n_position(layer, j)];
					derivatives_neurons[n_position(layer, j)] += aux * weights[w_position(layer, i, j)];
				}
			}
		}
	}
	for (i = 0; i < weights_array_size; i++)
		weights[i] -= learning_rate * inv_batch_size * derivatives_weights[i];
}