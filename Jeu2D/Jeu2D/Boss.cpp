#pragma once
#include "Boss.h"
#include "Timer.h"
#include <cmath>
#include "Shoot.h";

void CreateBoss(World* world,int beginPos) {
	sf::CircleShape head = CreateCircleShape(sf::CircleShape(80), sf::Color::Black, sf::Vector2f(beginPos, 200),3,sf::Color::Magenta,world);
	
	sf::RectangleShape leftarm_01 = CreateRectangleShape(sf::RectangleShape(sf::Vector2f(120, 20)), sf::Color::Black, sf::Vector2f(beginPos,200.f), 3, sf::Color::Magenta, false, world);
	sf::RectangleShape leftarm_02 = CreateRectangleShape(sf::RectangleShape(sf::Vector2f(120, 15)), sf::Color::Black, sf::Vector2f(beginPos - 85, 390.f), 3, sf::Color::Magenta, false, world); //leftarm_01.setOrigin(sf::Vector2f(head.getPosition().x + head.getRadius(),head.getPosition().y + head.getRadius()));

	sf::RectangleShape rightarm_01 = CreateRectangleShape(sf::RectangleShape(sf::Vector2f(120, 20)), sf::Color::Black, sf::Vector2f(beginPos + 160, 315.f), 3, sf::Color::Magenta, false, world);
	sf::RectangleShape rightarm_02 = CreateRectangleShape(sf::RectangleShape(sf::Vector2f(120, 15)), sf::Color::Black, sf::Vector2f(beginPos + 250, 410.f), 3, sf::Color::Magenta, false, world);

	head.setOrigin(40, 40);

	leftarm_01.setOrigin(-head.getRadius() ,leftarm_01.getSize().y / 2.f); // Je centre son origine au centre du rectangle
	leftarm_01.rotate(130);
	leftarm_01.setPosition(head.getPosition().x + head.getOrigin().x, head.getPosition().y + head.getOrigin().y);

	leftarm_02.rotate(-40);
	leftarm_02.setPosition(leftarm_01.getSize().x - 20,13);

	rightarm_01.setOrigin(-head.getRadius(), rightarm_01.getSize().y / 2.f); // Je centre son origine au centre du rectangle
	rightarm_01.rotate(45);
	rightarm_01.setPosition(head.getPosition().x + head.getOrigin().x, head.getPosition().y + head.getOrigin().y);

	rightarm_02.rotate(45);
	rightarm_02.setPosition(rightarm_01.getSize().x,0);

	Boss* boss = new Boss(head, true, 7.5f, leftarm_01.getRotation(), rightarm_01.getRotation(), false, true, 6, 6, CreateCircleShape(sf::CircleShape(65), sf::Color::Black, sf::Vector2f(beginPos - 25, 195), 3, sf::Color::Blue, world),CreateRectangleShape(sf::RectangleShape(sf::Vector2f(30, 45)), sf::Color::Black, sf::Vector2f(beginPos + 25, 165.f), 3, sf::Color::Magenta, false, world), nullptr, nullptr, { CreateRectangleShape(sf::RectangleShape(sf::Vector2f(300, 25)), sf::Color::Black, sf::Vector2f(830.f, 410.f), 3, sf::Color::Red, false, world),CreateRectangleShape(sf::RectangleShape(sf::Vector2f(80, 25)), sf::Color::Red, sf::Vector2f(830.f, 410.f), 0, sf::Color::Magenta, false, world) },NONE);
	DrawWeaknessArea(head, boss);

	boss->leftArm.push_back(leftarm_01);
	boss->leftArm.push_back(leftarm_02);
	boss->rightArm.push_back(rightarm_01);
	boss->rightArm.push_back(rightarm_02);

	world->boss = boss;
}

void DrawWeaknessArea(sf::CircleShape head,Boss* boss) {
	bool back = true;
	for (int i = 11;i < 20;i++) {
		sf::Vertex vertex(-head.getOrigin() + head.getPosition() + head.getPoint(i));
		vertex.color = sf::Color::Yellow;
		boss->weaknessArea.push_back(vertex);
		if (i != 11 && i != 19) {
			if (back)
				i--;

			back = !back;
		}
	}
}

void UpdateBoss(World* world,Boss* boss,Player& player,std::list<Bullet>& bullets,float dt) {
	if (boss) {
		if (boss->canRotate) {
			float angle = dt * 3.141592f * 2.f * boss->speed;
			switch (boss->rotateLeftArm) {
			case true:
				RotateArms(boss, angle, 163.f, boss->originalLeftAngle, true, boss->leftArm);
				break;

			case false:
				RotateArms(boss, angle, 20.f, boss->originalRightAngle, false, boss->rightArm);
				break;
			}
			
			if (boss->wait) {
				if (!boss->rotateTimer) 
					boss->rotateTimer = InitTimer(1.5f, dt);		
				else {
					if (Wait(*boss->rotateTimer)) {
						boss->wait = false;
						boss->rotateTimer = nullptr;
						boss->speed = 10;
					}
				}
			}

			if ((int)boss->canon.getPosition().y == 120.f) {
				if (!world->boss->canonTimer) { // Appeler qu'une fois ? 
					world->boss->canonTimer = InitTimer(2, dt);
				}
				else {
					if (Wait(*world->boss->canonTimer)) {
						Shoot(sf::Vector2f(boss->canon.getPosition().x,boss->canon.getPosition().y), player.body.getPosition() - boss->canon.getPosition(), bullets,BOSS, dt); // Voir si elles peuvent traverser le corps du boss ou pas
						world->boss->canonTimer = nullptr; 
					}
				}
			}

		}
		else {
			if (boss->state == ANGRY) {
				boss->canon.move(0, -20 * dt);

				if ((int)boss->canon.getPosition().y == 120)
					boss->canRotate = true;
			}
		}

		if (boss->health <= 0)
			DestroyBoss(world);

		if (boss->health <= 3 && boss->state == NONE) {
			boss->state = ANGRY;
			boss->canRotate = false;
		}

	}
}

void RotateArms(Boss* boss,float angle,float beginAngle,float endAngle,bool positive,std::vector<sf::RectangleShape>& arms) {
	//for (sf::RectangleShape& arm : arms) {
	sf::RectangleShape& arm = arms[0];
		if (boss->speed == 7.5f) {
			if (positive ? arm.getRotation() <= beginAngle : arm.getRotation() >= beginAngle)
				arm.rotate(positive ? angle : -angle);
			else
				boss->wait = true;
		}
		else {
			if (positive ? arm.getRotation() >= endAngle : arm.getRotation() <= endAngle)
				arm.rotate(positive ? -angle : angle);
			else {
				boss->rotateLeftArm = !boss->rotateLeftArm;
				boss->speed = 7.5f;
			}
		}

	//}
}

void RefreshBoss(Boss* boss,sf::RenderWindow& window,sf::View& view) {
	if (boss) {
		window.draw(boss->rightArm[0]);
		sf::RenderStates rightState;
		rightState.transform = boss->rightArm[0].getTransform();
		window.draw(boss->rightArm[1],rightState);

		window.draw(boss->leftArm[0]);
		sf::RenderStates leftState;
		leftState.transform = boss->leftArm[0].getTransform();
		window.draw(boss->leftArm[1], leftState);

		window.draw(boss->canon);
		window.draw(boss->head);

		sf::Vertex lines[16];
		std::copy(boss->weaknessArea.begin(), boss->weaknessArea.end(), lines);
		window.draw(lines, boss->weaknessArea.size(), sf::Lines);

		if (boss->head.getPosition().x >= view.getCenter().x - view.getSize().x / 2.f && boss->head.getPosition().x <= view.getCenter().x + view.getSize().x / 2.f) {
			boss->bar.background.setPosition(sf::Vector2f(view.getCenter().x - boss->bar.background.getSize().x / 2.f, 50.f));
			boss->bar.health.setSize(sf::Vector2f((boss->bar.background.getSize().x / boss->maxHealth) * boss->health, boss->bar.health.getSize().y));
			boss->bar.health.setPosition(sf::Vector2f(view.getCenter().x - boss->bar.background.getSize().x / 2.f, 50.f));

			window.draw(boss->bar.background);
			window.draw(boss->bar.health);
		}
	}
}

void DestroyBoss(World* world) {
	delete world->boss->canonTimer;
	delete world->boss->rotateTimer;
	world->boss = nullptr; 
}
