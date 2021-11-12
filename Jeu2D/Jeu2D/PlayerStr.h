#pragma once

#include <SFML/Graphics.hpp>
#include "CollisionStr.h"


struct Player
{
	sf::CircleShape body;
	float health;
	bool mooveX;
	sf::Vector2f direction;
	sf::Vector2f lastDirection;
	bool isJumping;
	Collision collision;
};