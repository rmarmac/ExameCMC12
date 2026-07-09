#include "drone.h"
#include "assets.h"
#include <numbers>

#define PI 3.14159265358979
#define GRAVITY 9.81

Drone::Drone() : force_left(0), force_right(0), theta(0), omega(0) {
	velocity.x = 0;
	velocity.y = 0;
	position.x = 0;
	position.y = 0;
}

Drone::Drone(double x, double y) : force_left(0), force_right(0), theta(0), omega(0) {
	velocity.x = 0;
	velocity.y = 0;
	position.x = x;
	position.y = y;
}

vector_t Drone::GetPosition() const{
	return position;
}

vector_t Drone::GetVelocity() const {
	return velocity;
}

double Drone::GetTheta() const {
	return theta;
}

double Drone::GetOmega() const {
	return omega;
}

void Drone::Draw(sf::RenderWindow* window, Assets assets, bool draw_forces = true) {
	drone_assets_t drone_assets = assets.base_drone_assets;
	double visual_length = LENGTH * visual_scale_factor_drone;

	drone_assets.drone_body.setPosition(position.x, position.y);
	drone_assets.drone_body.setRotation(theta * 180 / PI);
	(*window).draw(drone_assets.drone_body);
	if (draw_forces) {
		drone_assets.visual_f_left.scale(sf::Vector2f(force_left, 1));
		drone_assets.visual_f_left.setPosition(position.x - visual_length / 2 * cos(theta), position.y - visual_length / 2 * sin(theta));
		drone_assets.visual_f_left.setRotation(theta * 180 / PI - 90);
		drone_assets.visual_f_right.scale(sf::Vector2f(force_right, 1));
		drone_assets.visual_f_right.setPosition(position.x + visual_length / 2 * cos(theta), position.y + visual_length / 2 * sin(theta));
		drone_assets.visual_f_right.setRotation(theta * 180 / PI - 90);

		(*window).draw(drone_assets.visual_f_left);
		(*window).draw(drone_assets.visual_f_right);
	}
}

void Drone::ApplyForces(double f_left, double f_right) {
	force_left = f_left;
	force_right = f_right;
}

void Drone::Update(double delta_t) {
	double total_force = force_left + force_right;
	velocity.x += total_force * sin(theta) * delta_t / MASS;
	velocity.y += (-total_force * cos(theta) / MASS + GRAVITY) * delta_t;
	position.x += velocity.x;
	position.y += velocity.y;

	double torque = (force_left - force_right) * LENGTH / 2;
	omega += torque * delta_t / J;
	theta += omega * delta_t;
}

void Drone::Reset(double x, double y) {
	force_left = 0;
	force_right = 0;
	theta = 0;
	omega = 0;
	velocity.x = 0;
	velocity.y = 0;
	position.x = x;
	position.y = y;
}