#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

class Obj{
	private:
		sf::CircleShape shape;
		sf::Vector2f a, prevPos, curPos;
	public:
		Obj (float x, float y, float r, sf::Color color): shape(r), prevPos(x,y), curPos(x,y){
			shape.setPosition(x,y);
			shape.setFillColor(color);
		}

		void updatePos(float dt){
			//std::cout << shape.getPosition().x << " , " << shape.getPosition().y << std::endl;
			sf::Vector2f v = curPos - prevPos;
			prevPos = curPos;
			curPos = shape.getPosition() + v * dt + a * dt * dt;
			shape.setPosition(curPos);
		}

		void accelerate(sf::Vector2f vec) {
			a += vec;
		}

		const sf::CircleShape getShape() const {
			return shape;
		}
};

int main() {
	Obj shape1(400, 100, 10, sf::Color::Cyan);
	sf::Vector2f gravity(0, 2);
	float dt = 0.005;

	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
		}

		shape1.accelerate(gravity);
		shape1.updatePos(dt);

		window.clear();
		window.draw(shape1.getShape());
		window.display();
	}
    return 0;
}

