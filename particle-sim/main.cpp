#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/PrimitiveType.hpp"
#include "SFML/Graphics/Shape.hpp"
#include "SFML/Graphics/Vertex.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Sleep.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <chrono>
#include <string>
#include <thread>
#include <vector>
#include <iostream>
#include <mutex>

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
				shape.setPosition(curPos);
				other.curPos += correction;
				other.shape.setPosition(other.curPos);
			}
		}

		void accelerate(sf::Vector2f vec) {
			a += vec;
		}

		const sf::CircleShape getShape() const {
			return shape;
		}
};

std::mutex objVecMutex;

void addObjectThread(std::vector<Obj>& objVec){
	int totalObject = 100;	
	float r = 5,
				xIncrement = 0.1,
				x = 1,
				yIncrement = 0.1,
				y = 0;
	int colors [15] {205,180,219,255,200,221,255, 175, 204,189, 224, 254,162, 210, 255};
	for (int i=0; i < totalObject; i++){
		objVecMutex.lock();	
		objVec.push_back(Obj (200,200, r, sf::Color(colors[i%5*3], colors[i%5*3+1], colors[i%5*3+2]), x, y));
		objVecMutex.unlock();
		x+=xIncrement;
		if (x > 1.5) xIncrement = -0.1;
		else if (x < 0.5) xIncrement = 0.1;
		if (y > 0.5) yIncrement = -0.1;
		else if (y < -0.5) yIncrement = 0.1;
		r+=5;
		if (r == 20) r = 5;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

int main() {
	std::vector<Obj> objVec;

	sf::Vector2f gravity(0, 9.81);
	float dt = 0.005;
	
	sf::CircleShape container(300);

	sf::Font font;
	font.loadFromFile("/System/Library/Fonts/Supplemental/Arial.ttf");
	sf::Text fpsText;
	fpsText.setFont(font);
	fpsText.setCharacterSize(20);
	fpsText.setFillColor(sf::Color(208, 244, 222));
	fpsText.setPosition(0,0);
	sf::Clock clock;
	float fps = 0;


	sf::RenderWindow window(sf::VideoMode(600, 600), "SFML window");
	
	std::thread thread(addObjectThread, std::ref(objVec));
	
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
		}

		window.clear();

		window.draw(container);	
		
		objVecMutex.lock();
		for (int i = 0; i < objVec.size(); i++) objVec[i].accelerate(gravity);
		for (int k = 9; k--;){
			for (int i = 0; i < objVec.size(); i++) objVec[i].updatePos(dt);
			for (int i = 0; i < objVec.size(); i++) objVec[i].stayInBoundary(container);
			for (int i = 0; i < objVec.size(); i++) 
				for (int j= i + 1; j < objVec.size(); j++) 
					objVec[i].detectCollision(objVec[j]);	
		}
		for (int i=0; i<objVec.size();i++) window.draw(objVec[i].getShape());
		objVecMutex.unlock();

		fps = 1.f / clock.restart().asSeconds();
		fpsText.setString("FPS: " + std::to_string(static_cast<int>(fps)));
		window.draw(fpsText);

		window.display();

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
	thread.join();
	return 0;
}

