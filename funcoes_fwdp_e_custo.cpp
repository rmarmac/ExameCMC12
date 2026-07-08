#include "funcoes_fwdp_e_custo.h"
//-------------------------------------------------------------------------------------------------------
float Leaky_ReLu::function(float x) {
	return x * (0.125f + (x > 0) * 0.875f);
}
float Leaky_ReLu::derivative(float x) {
	return 0.125f + 0.875f * (x > 0);
}
//-------------------------------------------------------------------------------------------------------
float ReLu::function(float x) {
	return x * (x > 0);
}
float ReLu::derivative(float x) {
	return 1.f * (x > 0);
}
//-------------------------------------------------------------------------------------------------------
float Tanh::function(float x) {
	return (float)tanh(x);
}
float Tanh::derivative(float tanhx) {
	return (1 - tanhx * tanhx);
}
//-------------------------------------------------------------------------------------------------------
float Linear::function(float x) {
	return x;
}
float Linear::derivative(float x) {
	return 1;
}
//-------------------------------------------------------------------------------------------------------
void Quadratic_error(const float outputs[], const float desired_outputs[], float neuron_derivatives[], const int output_size) {
	for (int i = 0; i < output_size; i++)
		neuron_derivatives[i] = (outputs[i] - desired_outputs[i]);
}
//-------------------------------------------------------------------------------------------------------