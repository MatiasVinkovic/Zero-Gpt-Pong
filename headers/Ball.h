//
// Created by Matias Vinkovic on 19/02/2026.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include <typeinfo>

#ifndef PONG_BALL_H
#define PONG_BALL_H


class Ball {

	private:
		sf::CircleShape shape;
		float radius;
		sf::Color color;
		float x;
		float y;

		int moveCoeffX, moveCoeffY;

		// --- IL MANQUAIT ÇA ICI ---
        sf::Clock boostTimer;
        bool isAccelerated = false;


	public:

		//constructeur principal
		Ball (float new_x, float new_y, float new_radius);
		void createBall();
		void setPosition(float x, float y);
		void startBallMovement(int new_moveCoeffX, int new_moveCoeffY);
		void startBallMovementAcce(int new_moveCoeffX, int new_moveCoeffY, bool acceleration);

		
		sf::CircleShape getShape();
		float getPositionX();
		float getPositionY();

		void stopBall();

		int getMoveCoeffX();
		int getMoveCoeffY();

		void setMoveCoeffX(int newValue);
		void setMoveCoeffY(int newValue);





};


#endif //PONG_BALL_H