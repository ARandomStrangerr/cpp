#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/VideoMode.hpp"
#include "particle.h"
#include <chrono>
#include <string>
#include <thread>
#include <vector>
#include <iostream>
#include <mutex>

std::mutex objVecMutex;

void addObjectThread(std::vector<Obj>& objVec, int totalObject){
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

void fallingObject() {
	std::vector<Obj> objVec;

	sf::Vector2f gravity(0, 9.81);
	float dt = 0.1;

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

	sf::RenderWindow window(sf::VideoMode(600, 600), "Test falling objects");

	std::thread thread(addObjectThread, std::ref(objVec), 100);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
		}

		window.clear();

		window.draw(container);

		objVecMutex.lock();
		for (int i = 0; i < objVec.size(); i++) objVec[i].applyAcceleration(gravity);
		for (int i = 0; i < objVec.size(); i++) objVec[i].updatePos(0.05);
		for (int k = 9; k--;){
			for (int i = 0; i < objVec.size(); i++) objVec[i].applyBoundary(container);
			for (int i = 0; i < objVec.size(); i++)
				for (int j= i + 1; j < objVec.size(); j++)
					objVec[i].applyCollision(objVec[j]);
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
}

void chain() {
	sf::Vector2f acceleration(0,9.81);
	Chain chain(300,300, 2, 5, 90, 15);
	sf::RenderWindow window(sf::VideoMode(600,600), "Test chain window");
	while (window.isOpen()){
		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed) window.close();
		}
		window.clear();

		chain.applyAcceleration(acceleration);
		chain.updatePos(0.05);
		chain.applyContrain();
		for (int i = 0; i < chain.getChain().size(); i++) window.draw(chain.getChain()[i].getShape());

		window.display();

		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}
}

int main(){
	chain();
	return 0;
}
