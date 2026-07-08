#pragma once
#include <math.h>
/* - Para criar uma funcao de ativacao, crie uma classe com o nome da funcao desejada, adicione o
  nome na lista de funcoes disponiveis em neuralnet.h e, por fim, defina, dentro da secao "public",
  a funcao em si (function) e sua derivada (a derivada deve depender do resultado da propria funcao
  como no exemplo de Tanh). Defina os resultados de cada em funcoes_fwdp_e_custo.cpp
   - Para criar uma funcao de calculo de erro, essa deve possuir o mesmo formato de parametros das
  demais funcoes do arquivo, considerando a ordem em que aparecem: outputs, desired_outputs, neuron_-
  derivatives e size. Defina seu funcionamento em funcoes_fwdp_e_custo.cpp
*/
//-------------------------------------------------------------------------------------------------------
class Leaky_ReLu {
public:
	float function(const float x);
	float derivative(const float x);
};
//-------------------------------------------------------------------------------------------------------
class ReLu {
public:
	float function(const float x);
	float derivative(const float x);
};
//-------------------------------------------------------------------------------------------------------
class Tanh {
public:
	float function(const float x);
	float derivative(const float tanhx_times_two);
};
//-------------------------------------------------------------------------------------------------------
class Linear {
public:
	float function(const float x);
	float derivative(const float x);
};
//-------------------------------------------------------------------------------------------------------
void Quadratic_error(const float outputs[], const float desired_outputs[], float neuron_derivatives[], const int size);
