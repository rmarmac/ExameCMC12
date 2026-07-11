#pragma once
#include <SFML/Graphics.hpp>

const double visual_scale_factor_forces = 30;

//-------------------------------------------------------------------------------------------------------
/// Estrutura de assets para o drone
typedef struct drone_assets_t {
	sf::RectangleShape drone_body;
	sf::RectangleShape visual_f_left;
	sf::RectangleShape visual_f_right;
} drone_assets_t;
//-------------------------------------------------------------------------------------------------------
/// A classe assets serve como um organizador para os elementos visuais do programa. Ajuda ao não ser necessário
/// criar a mesma instância de elemento visual para cada drone e target, que usam o mesmo modelo base
class Assets {
public:
	Assets();
	drone_assets_t base_drone_assets;
	sf::CircleShape target;
};