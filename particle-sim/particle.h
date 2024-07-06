#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include <vector>
#include <cmath>

class Obj;
class Chain;

class Obj {
	friend class Chain;
	private:
		sf::CircleShape shape;
		sf::Vector2f a,prevPos,curPos;
	public:
		Obj(float, float, float, sf::Color, float, float);
		void updatePos(float);
		void applyBoundary(const sf::CircleShape&);
		void applyCollision(Obj&);
		void applyAcceleration(const sf::Vector2f&);
		const sf::CircleShape& getShape() const;
};

class Chain {
	private:
		std::vector<Obj> objs;
		sf::Vector2f originalPosition;
		float targetDistance;
	public:
		Chain(float, float, int, float, float, float);
		void updatePos(float);
		void applyContrain();
		void applyAcceleration(const sf::Vector2f&);
		const std::vector<Obj> getChain() const;
};
