#pragma once

// Calcula o espaco necessario para a matriz de neurons
constexpr int calc_neurons_array_size(const int neural_net_array[], const int neural_net_size) {
	int sum = 0;
	for (int i = 0; i < neural_net_size; i++)
		sum += neural_net_array[i];
	const int total_sum = sum + neural_net_size - 1;
	return total_sum;
}
//-------------------------------------------------------------------------------------------------------
// Calcula o espaco necessario para a matriz de weights (ou conexoes) 
constexpr int calc_weights_array_size(const int neural_net_array[], const int neural_net_size) {
	int sum = 0;
	for (int i = 0; i < neural_net_size - 1; i++)
		sum += (neural_net_array[i] + 1) * neural_net_array[i + 1];
	const int total_sum = sum;
	return total_sum;
}
//-------------------------------------------------------------------------------------------------------
