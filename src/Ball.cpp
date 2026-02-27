//
// Created by Matias Vinkovic on 19/02/2026.
//

#include "../headers/Ball.h"
#include <iostream>
#include <SFML/Graphics.hpp>


//constructeur par défaut
Ball::Ball(float new_x, float new_y, float new_radius) {
    this->x = new_x;
    this->y = new_y;
    this->radius = new_radius;
}

void Ball::createBall() {
    this->shape.setRadius(this->radius);
    this->shape.setPosition(this->x, this->y);
    this->shape.setOutlineColor(sf::Color::White);
}

// Ajout du type 'void'
void Ball::setPosition(float new_x, float new_y) {
    this->x = new_x;
    this->y = new_y;
}

sf::CircleShape Ball::getShape() {
    return this->shape;
}

float Ball::getPositionX() {
    return x;
}

float Ball::getPositionY() {
    return y;
}

void Ball::startBallMovement(int new_moveCoeffX, int new_moveCoeffY) {
    //y+=1*0.02;
    y = y +(new_moveCoeffY*0.05);
    x = x +(new_moveCoeffX*0.05);
    this->shape.setPosition(this->x,y);

    //mettre à jour les coefficients directeurs de la balle
    this->moveCoeffX = new_moveCoeffX;
    this->moveCoeffY = new_moveCoeffY;
}

void Ball::stopBall() {
    this->shape.setPosition(this->x, this->y);
}

//getter pour les coefficients de mouvement de la balle
int Ball::getMoveCoeffX() {
  return moveCoeffX;
}

int Ball::getMoveCoeffY() {
  return moveCoeffY;
}

void Ball::setMoveCoeffX(int newValue) {
    moveCoeffX = newValue;
}

void Ball::setMoveCoeffY(int newValue) {
    moveCoeffY = newValue;
}
