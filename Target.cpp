#include "Target.h"

//-------------------------------------------------------------------------------------------------------
/// Construtor Padrão
Target::Target() {
	position.x = 0;
	position.y = 0;
}
//-------------------------------------------------------------------------------------------------------
/// Construtor com posição pré-definida
Target::Target(double x, double y) {
	SetPosition(x, y);
}
//-------------------------------------------------------------------------------------------------------
/// Set de posição
void Target::SetPosition(double x, double y) {
	position.x = x;
	position.y = y;
}
//-------------------------------------------------------------------------------------------------------
/// Recebe uma tela e uma variavel assets para se desenhar.
void Target::Draw(sf::RenderWindow* window, Assets assets) const {
	assets.target.setPosition(WINDOW_WIDTH / 2 + position.x * SCALE, WINDOW_HEIGHT / 2 + position.y * SCALE);
	(*window).draw(assets.target);
}
//-------------------------------------------------------------------------------------------------------
/// Get de posição
vector_t Target::GetPosition() const {
	return position;
}
//-------------------------------------------------------------------------------------------------------
/// Reseta os targets de forma aleatoria
void ResetarTargets(Target targets[]) {
	const double fraction_of_screen = 0.7;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distrib(-fraction_of_screen / 2, fraction_of_screen / 2);
	for (size_t i = 0; i < NUMBER_OF_TARGETS; i++)
		targets[i].SetPosition((float)distrib(gen) * WINDOW_WIDTH / SCALE,
							   (float)distrib(gen) * WINDOW_HEIGHT / SCALE);
}