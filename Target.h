#pragma once
#include "configs.h"
#include "assets.h"
#include "vector.h"
#include <random>


//-------------------------------------------------------------------------------------------------------
/// Classe para um alvo
class Target {
public:
	Target();
	Target(double x, double y);
	void Draw(sf::RenderWindow* window, Assets assets) const;
	void SetPosition(double x, double y);
	vector_t GetPosition() const;
private:
	vector_t position;
};

void ResetarTargets(Target targets[]);