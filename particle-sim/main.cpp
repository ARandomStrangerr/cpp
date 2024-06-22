#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/PrimitiveType.hpp"
#include "SFML/Graphics/Shape.hpp"
#include "SFML/Graphics/Vertex.hpp"
#include "SFML/System/Sleep.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <thread>

class Obj{
	private:
		sf::CircleShape shape;
		sf::Vector2f a, prevPos, curPos;
	public:
		Obj (float x, float y, float r, sf::Color color, float initX, float initY) : shape(r), prevPos(x,y), curPos(x+initX,y+initY){
			shape.setPosition(prevPos);
			shape.setFillColor(color);
		}

		void updatePos(float dt){
			//std::cout << shape.getPosition().x << " , " << shape.getPosition().y << std::endl;
			sf::Vector2f v = curPos - prevPos;
			prevPos = curPos;
			curPos = curPos + v + a * dt * dt;
			shape.setPosition(curPos);
		}

		void stayInBoundary(sf::CircleShape& boundary){
			// obtain the position vector center of the boundary
			sf::Vector2f centerBoundary = boundary.getPosition() + sf::Vector2f(boundary.getRadius(), boundary.getRadius());
			// obtain the position vecotr center of this shape
			sf::Vector2f centerThisShape = shape.getPosition() + sf::Vector2f(shape.getRadius(), shape.getRadius());
			// if the displacement is greater than the radius, then it is outside of the circle
			sf::Vector2f displacement = centerThisShape - centerBoundary;
			float distance = sqrt(pow(displacement.x, 2) + pow(displacement.y, 2));
			if (distance + shape.getRadius() >= boundary.getRadius()) {
				curPos = centerBoundary + (displacement / distance) * (boundary.getRadius() - shape.getRadius()) - sf::Vector2f(shape.getRadius(), shape.getRadius());
				shape.setPosition(curPos);
			}
		}

		void detectCollision(Obj& other){
			sf::Vector2f centerThisShape = shape.getPosition()+sf::Vector2f(shape.getRadius(), shape.getRadius());
			sf::Vector2f centerOtherShape = other.shape.getPosition()+sf::Vector2f(other.shape.getRadius(), other.shape.getRadius());
			sf::Vector2f displacement = centerOtherShape - centerThisShape;
			float distance = sqrt(pow(displacement.x,2) + pow (displacement.y,2));
			// if 2 shapes overlap eachother, we move each shape half the overlap distance
			if (distance < shape.getRadius() + other.shape.getRadius()) {
				float overlap = shape.getRadius() + other.shape.getRadius() - distance;
				sf::Vector2f correction = (displacement / distance) * overlap * 0.5f;
				curPos -= correction;
				shape.move(-correction);
				other.curPos += correction;
				other.shape.move(correction);
			}
		}

		void accelerate(sf::Vector2f vec) {
			a += vec;
		}

		const sf::CircleShape getShape() const {
			return shape;
		}
};

int main() {
	Obj shape1(400, 300, 30, sf::Color::Cyan, 0, 100);
	Obj shape2(200,300, 30, sf::Color::Yellow, 0, 0);
	sf::Vector2f gravity(0, 9.81);
	float dt = 0.09;
	
	sf::CircleShape container(300);

	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
		}

		shape1.accelerate(gravity);
		shape1.updatePos(dt);
		shape1.stayInBoundary(container);
		shape1.detectCollision(shape2);

		shape2.accelerate(gravity);
		shape2.updatePos(dt);
		shape2.stayInBoundary(container);
		shape2.detectCollision(shape1);

		window.clear();
		window.draw(container);
		window.draw(shape1.getShape());
		window.draw(shape2.getShape());
		window.display();

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
    return 0;
}

