#pragma once
#include <SFML/Graphics.hpp>
#include "LevelGenerator.h"
#include "PlayerStr.h"
#include "ShootStr.h"
#include "GameState.h"
#include <list>

const float originalGroundY = 475.f;
const float speed = 400.f;
const float jumpForce = -4.f;
const sf::Vector2f gravity(0.f, 9.8f);
 
Player newPlayer();

void UpdatePlayer(Player&, float, sf::Vector2f&,sf::View& view,World*, std::list<Bullet>&, GameState&);
void MovePlayer(Player&, float);
void JumpPlayer(Player&,float,sf::Vector2f&,World*);

bool isGrounded(Player&,World*);
bool isOnFloor(Player&);

bool CanStopJump(Player&);
bool CanWallJump(Player&);