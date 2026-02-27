//
// Created by Matias Vinkovic on 18/02/2026.
//
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#ifndef PONG_RECTANGLE_H
#define PONG_RECTANGLE_H


class Rectangle {
	private :
		float y;
		float x;
		sf::RectangleShape shape;

	public:
		//constructeur principal
		Rectangle (float x, float y);
		void createRectangle();
		void setPosition(float x, float y);
		void move(int moveCoeff);
		sf::RectangleShape getShape();
		float getPositionY();
		float getPositionX();





};


#endif //PONG_RECTANGLE_H