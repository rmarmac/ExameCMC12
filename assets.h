#pragma once
#include <SFML/Graphics.hpp>

const double visual_scale_factor_forces = 30;
const double visual_scale_factor_drone = 500;

typedef struct drone_assets_t {
	sf::RectangleShape drone_body;
	sf::RectangleShape visual_f_left;
	sf::RectangleShape visual_f_right;
} drone_assets_t;

class Assets {
public:
	Assets();
	drone_assets_t base_drone_assets;
	sf::CircleShape target;
};