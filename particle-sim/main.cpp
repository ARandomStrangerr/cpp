#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>

class Obj{
	private:
		sf::CircleShape shape;
		sf::Vector2f a, prevPos, curPos;
	public:
		Obj (float x, float y, float r, sf::Color color): shape(r){
			shape.setPosition(x,y);
			shape.setFillColor(color);
		}

		void updatePos(float dt){
			sf::Vector2f v = curPos - prevPos;
			prevPos = curPos;
			curPos = shape.getPosition() + v * dt + a * dt * dt;
		}

		const sf::CircleShape getShape() const {
			return shape;
		}
};

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

	Obj shape1(300, 300, 10, sf::Color::Cyan);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
		}

		window.clear();
		window.draw(shape1.getShape());
		window.display();
	}
    return 0;
}

