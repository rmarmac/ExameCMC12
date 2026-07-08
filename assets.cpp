#include "assets.h"
#include "drone.h"

Assets::Assets() {
	base_drone_assets.drone_body = sf::RectangleShape(sf::Vector2f(LENGTH * visual_scale_factor_drone, HEIGHT * visual_scale_factor_drone));
	base_drone_assets.drone_body.setFillColor(sf::Color::White);
	base_drone_assets.drone_body.setOrigin(LENGTH * visual_scale_factor_drone / 2, HEIGHT * visual_scale_factor_drone / 2);

	base_drone_assets.visual_f_left = sf::RectangleShape(sf::Vector2f(visual_scale_factor_forces, HEIGHT * visual_scale_factor_drone / 4));
	base_drone_assets.visual_f_left.setFillColor(sf::Color::Green);
	base_drone_assets.visual_f_left.setOrigin(0, (HEIGHT * visual_scale_factor_drone / 4) / 2);
	base_drone_assets.visual_f_right = sf::RectangleShape(sf::Vector2f(visual_scale_factor_forces, HEIGHT * visual_scale_factor_drone / 4));
	base_drone_assets.visual_f_right.setFillColor(sf::Color::Magenta);
	base_drone_assets.visual_f_right.setOrigin(0, (HEIGHT * visual_scale_factor_drone / 4) / 2);

	double radius = 10;
	target = sf::CircleShape(radius);
	target.setOrigin(radius, radius);
	target.setFillColor(sf::Color::Red);
}