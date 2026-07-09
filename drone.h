#pragma once
#include "assets.h"
#include <math.h>
#include <SFML/Graphics.hpp>
#include "vector.h"

#define LENGTH 0.2
#define HEIGHT 0.02
#define	J 0.04
#define MASS 0.5


// força máxima do drone é 1.8mg
class Drone {
public:
	Drone();
	Drone(double x, double y);
	vector_t GetPosition() const;
	vector_t GetVelocity() const;
	double GetTheta() const;
	double GetOmega() const;
	void Draw(sf::RenderWindow* window, Assets assets, bool draw_forces);
	void ApplyForces(double force_left, double force_right);
	void Update(double delta_t);
	void Reset(double x, double y);
private:
	double force_left;
	double force_right;
	double theta;
	double omega;
	vector_t position;
	vector_t velocity;
};