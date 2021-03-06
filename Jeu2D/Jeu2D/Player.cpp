#pragma once
#include "Player.h"
#include <SFML/Graphics.hpp>
#include "LevelGenerator.h"
#include "Collision.h"
#include <list>
#include "GameState.h"

Player newPlayer()
{
	Player p;

	p.body = sf::CircleShape(20.f);
	p.body.setPosition(600.f, originalGroundY - 20);
	p.body.setFillColor(sf::Color::Black);
	p.body.setOrigin(p.body.getRadius(), p.body.getRadius());
	p.body.setOutlineThickness(3.f);
	p.body.setOutlineColor(sf::Color::Red);

	p.health = 100.f;

	p.isJumping = false;
	p.lastJumpDirection = 0;
	p.lastPosition = sf::Vector2f(0.f, 0.f);
	p.maxAmmo = 10;
	p.ammo = p.maxAmmo;
	p.canJump = true;
	return p;
}

void UpdatePlayer(Player& player, float dt, sf::Vector2f& velocity, sf::View& view,World* world, std::list<Bullet>& bullets,GameState& state) {
	player.velocity = velocity;

	MovePlayer(player, dt);
	JumpPlayer(player,dt,velocity,world);
	OnCollisionDetection(player, world,bullets,state);
	DestroyEnnemies(world);

	if (player.body.getPosition().y >= 600) // Le joueur a dépassé la caméra
		state = LOOSE;

	view.setCenter(sf::Vector2f(player.body.getPosition().x, 300.f));
	player.lastPosition = player.body.getPosition();
}

void MovePlayer(Player& player, float dt) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		player.direction = sf::Vector2f(-1.f, 0.f);

		if (player.direction.x != player.lastDirection.x || (player.direction.x == player.lastDirection.x && player.mooveX)) { // On regarde si le joueur change de direction ou si il est dans la m�me direction et qu'il peut se d�placer
			if (-speed * dt + player.body.getPosition().x >= 5.f) // On place une bordure a 5 pour empecher le joueur d'aller au dela du niveau
				player.body.move(sf::Vector2f(-speed * dt, 0.f));

			if (player.direction.x != player.lastDirection.x) {
				player.mooveX = true;
				player.lastDirection = player.direction;
			}
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		player.direction = sf::Vector2f(1.f, 0.f);

		if (player.direction.x != player.lastDirection.x || (player.direction.x == player.lastDirection.x && player.mooveX)) { // On regarde si le joueur change de direction ou si il est dans la m�me direction et qu'il peut se d�placer
			player.body.move(sf::Vector2f(speed * dt, 0.f));

			if (player.direction.x != player.lastDirection.x) {
				player.mooveX = true;
				player.lastDirection = player.direction;
			}
		}
	}
}

void JumpPlayer(Player& player,float dt, sf::Vector2f& velocity,World* world) {
	if (player.canJump) {
		for (std::pair<sf::Shape*,Collision*> pair : player.collisions) {
			if (pair.second->isOnCollision && player.velocity.x != 0) {
				if (pair.second->rectangleCol) {
					Plateform* plateform = GetPlateformByShape(*pair.second->rectangleCol, world);

					if (plateform) {
						switch (player.lastJumpDirection != 0 && plateform->jumpDirection != player.lastJumpDirection) {
						case true:
							if (player.velocity.x < 0 && plateform->jumpDirection == -1)
								velocity.x = 0;
							else if (player.velocity.x > 0 && plateform->jumpDirection == 1)
								velocity.x = 0;

							break;
						}

						player.lastJumpDirection = plateform->jumpDirection;
					}
				}
			}
		}

		if (isGrounded(player, world)) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				if (velocity.x != 0)
					velocity.x = 0;

				velocity.y = jumpForce;
				player.body.move(velocity);
				player.isJumping = true;
				player.lastDirection = sf::Vector2f(0, 0);
			}


		}
		else {
			player.body.move(velocity);
			velocity += gravity * dt;
		}

		if (isGrounded(player, world) && player.isJumping)
			player.body.setPosition(player.body.getPosition().x, (*world).groundY - player.body.getRadius());

		if (isOnFloor(player)) {
			if (player.isJumping)
				player.isJumping = false;
		}
	}
}

bool isGrounded(Player& p,World* world) {
	return (p.body.getPosition().y + p.body.getRadius() >= (*world).groundY);
}

bool isOnFloor(Player& p) {
	return p.body.getPosition().y + p.body.getRadius() == originalGroundY;
}

bool CanStopJump(Player& player) {
	for (std::pair<sf::Shape*, Collision*> pair : player.collisions) {
		if (player.isJumping && pair.second->isOnCollision && pair.second->plateform && pair.second->plateform->type == WALL_JUMP) 
			return true;	
	}

	return false;
}

bool CanWallJump(Player& player) {
	for (std::pair<sf::Shape*, Collision*> pair : player.collisions) {
		if (!player.canJump && player.isJumping && pair.second->isOnCollision && pair.second->plateform && pair.second->plateform->type == WALL_JUMP) 
			return true;	
	}

	return false;
}
