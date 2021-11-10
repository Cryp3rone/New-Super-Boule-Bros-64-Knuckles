#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "LevelGenerator.h"
#include "EnnemyStr.h"

void CreateEnnemies(World* world);
void CreateCEnnemy(World* world,sf::CircleShape* shape, float speed,sf::Vector2f position,bool canMoove, sf::Vector2f min, sf::Vector2f max);
void CreateREnnemy(World* world, sf::RectangleShape shape, float speed, sf::Vector2f position, bool canMoove, sf::Vector2f min, sf::Vector2f max);
Ennemy CreateEnnemy(World* world, float speed, sf::Vector2f position, bool canMoove, sf::Vector2f min, sf::Vector2f max);

void UpdateEnnemies(World* world,float deltaTime);

void MooveEnnemy(Ennemy& ennemy, float deltaTime);
	
void RefreshEnnemies(World* world, sf::RenderWindow& window);