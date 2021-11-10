#pragma once
#include <SFML/Graphics.hpp>

const float groundY = 475.f;
const float speed = 400.f;
const float jumpForce = -4.f;
const sf::Vector2f gravity(0.f, 9.8f);

struct Player
{
	sf::CircleShape body;
	float health;
};

Player newPlayer();

void MovePlayer(Player&, float, sf::Vector2f&,sf::View& view);

bool isGrounded(Player&);