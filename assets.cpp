#include "assets.h"
#include "drone.h"
//-------------------------------------------------------------------------------------------------------
/// Construtor padrão de Assets
Assets::Assets() {
	base_drone_assets.drone_body = sf::RectangleShape(sf::Vector2f(LENGTH * SCALE, HEIGHT * SCALE));
	base_drone_assets.drone_body.setFillColor(sf::Color::White);
	base_drone_assets.drone_body.setOrigin(LENGTH * SCALE / 2, HEIGHT * SCALE / 2);

	base_drone_assets.visual_f_left = sf::RectangleShape(sf::Vector2f(visual_scale_factor_forces, HEIGHT * SCALE / 4));
	base_drone_assets.visual_f_left.setFillColor(sf::Color::Green);
	base_drone_assets.visual_f_left.setOrigin(0, (HEIGHT * SCALE / 4) / 2);
	base_drone_assets.visual_f_right = sf::RectangleShape(sf::Vector2f(visual_scale_factor_forces, HEIGHT * SCALE / 4));
	base_drone_assets.visual_f_right.setFillColor(sf::Color::Magenta);
	base_drone_assets.visual_f_right.setOrigin(0, (HEIGHT * SCALE / 4) / 2);

	double radius = TAMANHO_ALVO * SCALE;
	target = sf::CircleShape(radius);
	target.setOrigin(radius, radius);
	target.setFillColor(sf::Color::Red);
}