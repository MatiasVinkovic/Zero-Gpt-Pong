//
// Created by Matias Vinkovic on 24/02/2026.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include <typeinfo>
#include <cstdio>
#include <ctime>


#include "../headers/Rectangle.h"
#include "../headers/Ball.h"
#include "../headers/Score.h"

using namespace std;



#ifndef PONG_COLLISION_H
#define PONG_COLLISION_H


class Collision {
	private:
		Rectangle& rec;
		Rectangle& recRight;
		Ball& main_ball;
		Score& scoreManager;

		//sf::Keyboard::Key toucheBoost = sf::Keyboard::Space;
		sf::Keyboard::Key toucheBoost = sf::Keyboard::Space;
		sf::Keyboard::Key listeTouche[5];
		




	public:
		//constructeur
		Collision(Rectangle& new_rec, Ball& new_main_ball, Rectangle& new_recRight, Score& scoreManager_);

		//gérer les collisions pour les 2 rectangles
		void handleLeftRectColl();
   		void handleRightRectColl();
		void start();
		void handleGlobalCollision(int windowW, int windowH);

		std::string getBoostTouche();
		std::string keyToString(sf::Keyboard::Key key);
		void changeBoost();





};


#endif //PONG_COLLISION_H