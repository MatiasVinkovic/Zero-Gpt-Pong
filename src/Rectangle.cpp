#include "../headers/Rectangle.h"
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;

Rectangle::Rectangle(float init_x, float init_y) {
    x = init_x;
    y = init_y;
    cout << "le rectangle est créé !" << endl;
}

// Ajout du type 'void'
void Rectangle::setPosition(float new_x, float new_y) {
    this->x = new_x;
    this->y = new_y;
}

float Rectangle::getPositionY() {
    return y;

}

float Rectangle::getPositionX() {
    return x;
}

void Rectangle::createRectangle() {
    this->shape.setSize(sf::Vector2f(20, 200));
    this->shape.setPosition(this->x, this->y);
}

sf::RectangleShape Rectangle::getShape() {
    return this->shape;
}

void Rectangle::move(int moveCoeff){
    //*value += moveCoeff*0.05;
    y += moveCoeff*0.5;
    this->shape.setPosition(this->x,y);
}