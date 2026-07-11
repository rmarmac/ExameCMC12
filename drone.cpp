#include "drone.h"
#include "assets.h"
#include <numbers>

constexpr double upper_limit = 1.8 * MASS * GRAVITY;
constexpr double lower_limit = 0.2 * MASS * GRAVITY;

double LimitForce(double force) {
	double adj_force = force > upper_limit ? upper_limit : force;
	return adj_force < lower_limit ? lower_limit : adj_force;
}
//-------------------------------------------------------------------------------------------------------
/// Construtor base do drone
Drone::Drone() : force_left(0), force_right(0), theta(0), omega(0) {
	velocity.x = 0;
	velocity.y = 0;
	position.x = 0;
	position.y = 0;
}
//-------------------------------------------------------------------------------------------------------
/// Construtor de drone com posição pré-definida
Drone::Drone(double x, double y) : force_left(0), force_right(0), theta(0), omega(0) {
	velocity.x = 0;
	velocity.y = 0;
	position.x = x;
	position.y = y;
}
//-------------------------------------------------------------------------------------------------------
/// Get de posição
vector_t Drone::GetPosition() const{
	return position;
}
//-------------------------------------------------------------------------------------------------------
/// Get de velocidade
vector_t Drone::GetVelocity() const {
	return velocity;
}
//-------------------------------------------------------------------------------------------------------
/// Get de theta
double Drone::GetTheta() const {
	return theta;
}
//-------------------------------------------------------------------------------------------------------
/// Get de omega
double Drone::GetOmega() const {
	return omega;
}
//-------------------------------------------------------------------------------------------------------
/// Recebe uma tela e uma variável assets para se desenhar na tela (que não pode ser passada por referência, pois
/// cada drone aplicará uma escala diferente ao asset padrão), e recebe uma variável booleana que define se
/// desenhará as forças aplicadas ou não
void Drone::Draw(sf::RenderWindow* window, Assets assets, bool draw_forces = true) {
	drone_assets_t drone_assets = assets.base_drone_assets;
	double visual_length = LENGTH * SCALE;

	drone_assets.drone_body.setPosition(WINDOW_WIDTH / 2 + position.x * SCALE, WINDOW_HEIGHT / 2 + position.y * SCALE);
	drone_assets.drone_body.setRotation(theta * 180 / PI);
	(*window).draw(drone_assets.drone_body);
	if (draw_forces) {
		drone_assets.visual_f_left.scale(sf::Vector2f(force_left, 1));
		drone_assets.visual_f_left.setPosition(WINDOW_WIDTH / 2 + position.x * SCALE - visual_length / 2 * cos(theta),
											   WINDOW_HEIGHT / 2 + position.y * SCALE - visual_length / 2 * sin(theta));
		drone_assets.visual_f_left.setRotation(theta * 180 / PI - 90);
		drone_assets.visual_f_right.scale(sf::Vector2f(force_right, 1));
		drone_assets.visual_f_right.setPosition(WINDOW_WIDTH / 2 + position.x * SCALE + visual_length / 2 * cos(theta),
												WINDOW_HEIGHT / 2 + position.y * SCALE + visual_length / 2 * sin(theta));
		drone_assets.visual_f_right.setRotation(theta * 180 / PI - 90);

		(*window).draw(drone_assets.visual_f_left);
		(*window).draw(drone_assets.visual_f_right);
	}
}
//-------------------------------------------------------------------------------------------------------
/// Aplica as forças
void Drone::ApplyForces(double f_left, double f_right) {
	force_left = LimitForce(f_left);
	force_right = LimitForce(f_right);
}
//-------------------------------------------------------------------------------------------------------
/// Dadas as forças e um delta_t, atualiza conforme a física do drone
void Drone::Update(double delta_t) {
	double total_force = force_left + force_right;
	velocity.x += total_force * sin(theta) * delta_t / MASS;
	velocity.y += (-total_force * cos(theta) / MASS + GRAVITY) * delta_t;
	position.x += velocity.x * delta_t;
	position.y += velocity.y * delta_t;

	double torque = (force_left - force_right) * LENGTH / 2;
	omega += torque * delta_t / J_DRONE;
	theta += omega * delta_t;
}
//-------------------------------------------------------------------------------------------------------
/// Reseta o drone para uma posição x, y
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