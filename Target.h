#pragma once
#include "assets.h"
#include "vector.h"

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