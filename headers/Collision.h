//
// Created by Matias Vinkovic on 24/02/2026.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include <typeinfo>

#include "../headers/Rectangle.h"
#include "../headers/Ball.h"


#ifndef PONG_COLLISION_H
#define PONG_COLLISION_H


class Collision {
	private:
		Rectangle& rec;
		Rectangle& recRight;
		Ball& main_ball;


	public:
		//constructeur
		Collision(Rectangle& new_rec, Ball& new_main_ball, Rectangle& new_recRight);

		//gérer les collisions pour les 2 rectangles
		void handleLeftRectColl();
   		void handleRightRectColl();


		void start();

		void handleGlobalCollision(int windowW, int windowH);


};


#endif //PONG_COLLISION_H