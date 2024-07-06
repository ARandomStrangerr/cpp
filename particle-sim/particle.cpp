#include "particle.h"
#include "SFML/Graphics/Color.hpp"
#include "SFML/System/String.hpp"
#include "SFML/System/Vector2.hpp"
#include <cmath>
#include <vector>
#include <iostream>

Obj::Obj (float xInitPos, float yInitPos, float radi, sf::Color color, float vxInit, float vyInit) : shape(radi), prevPos(xInitPos, yInitPos), curPos(xInitPos+vxInit, yInitPos+vyInit) {
	shape.setPosition(curPos);
	shape.setFillColor(color);
}

void Obj::updatePos(float dt){
	sf::Vector2f v = curPos - prevPos;
	prevPos = curPos;
	curPos = curPos + v + a * dt * dt;
	shape.setPosition(curPos);
}

void Obj::applyAcceleration(const sf::Vector2f& acceleration){
	a+=acceleration;
}

void Obj::applyBoundary(const sf::CircleShape& boundary){
	sf::Vector2f centerBoundary = boundary.getPosition() + sf::Vector2f(boundary.getRadius(), boundary.getRadius());
	sf::Vector2f centerThisShape = shape.getPosition() + sf::Vector2f(shape.getRadius(), shape.getRadius());
	sf::Vector2f displacement = centerThisShape - centerBoundary;
	float distance = sqrt(pow(displacement.x,2)+pow(displacement.y,2));
	if (distance + shape.getRadius() >= boundary.getRadius()) {
		curPos = centerBoundary + (displacement/distance) * (boundary.getRadius()-shape.getRadius()) -sf::Vector2f(shape.getRadius(), shape.getRadius());
		shape.setPosition(curPos);
	}
}

void Obj::applyCollision(Obj& other){
	sf::Vector2f centerThisShape = shape.getPosition() + sf::Vector2f(shape.getRadius(), shape.getRadius()),
	centerOtherShape = other.shape.getPosition() + sf::Vector2f(other.shape.getRadius(), other.shape.getRadius()),
	displacement = centerOtherShape - centerThisShape;
	float distance = sqrt(pow(displacement.x,2) + pow(displacement.y,2));
	if (distance < shape.getRadius()+other.shape.getRadius()){
		float overlap = shape.getRadius() + other.shape.getRadius() - distance;
		sf::Vector2f correction = (displacement/distance) * overlap * .5f;
		curPos -= correction;
		shape.setPosition(curPos);
		other.curPos += correction;
		other.shape.setPosition(other.curPos);
	}
}

const sf::CircleShape& Obj::getShape() const{
	return shape;
}

Chain::Chain(float xInitPos, float yInitPos, int numObj, float radiEach, float angle, float distance) : originalPosition(xInitPos, yInitPos), targetDistance(distance) {
	sf::Vector2f displacement = sf::Vector2f(std::cos(angle)*distance, std::sin(angle)*distance);
	for(int i = 0; i < numObj; i++) {
		Obj obj (xInitPos + displacement.x * i, yInitPos + displacement.y * i, radiEach, sf::Color::Cyan, 0, 0);
		objs.push_back(obj);
	}
}

void Chain::updatePos(float dt) {
	for (int i = 1; i < objs.size(); i++) objs[i].updatePos(dt);
}

void Chain::applyContrain(){
	for (int i = 1; i < objs.size() ; i++){
		sf::Vector2f displacementVec = objs[i].curPos - objs[i-1].curPos;
		float norm = sqrt(pow(displacementVec.x,2) + pow(displacementVec.y,2));
		sf::Vector2f unitVec = displacementVec / norm;
		float diff = norm - targetDistance;
		sf::Vector2f correction = 0.5f * diff * unitVec;
		objs[i].curPos -= correction;
		objs[i-1].curPos += correction;
		std::cout<<diff<<std::endl;
	}
}

void Chain::applyAcceleration(const sf::Vector2f& acceleration){
	for(int i = 1; i < objs.size(); i++) objs[i].applyAcceleration(acceleration);
}

const std::vector<Obj> Chain::getChain() const{
	return objs;
}
