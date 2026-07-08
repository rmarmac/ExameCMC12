#include "Target.h"

Target::Target() {
	position.x = 0;
	position.y = 0;
}

Target::Target(double x, double y) {
	SetPosition(x, y);
}

void Target::SetPosition(double x, double y) {
	position.x = x;
	position.y = y;
}

void Target::Draw(sf::RenderWindow* window, Assets assets) const {
	assets.target.setPosition(position.x, position.y);
	(*window).draw(assets.target);
}

vector_t Target::GetPosition() const {
	return position;
}