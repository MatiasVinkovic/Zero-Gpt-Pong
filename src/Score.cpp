#include "../headers/Score.h"
#include <iostream>

using namespace std;

Score::Score(){
    this->scoreLeft=0;
    this->scoreRight=0;
}

void Score::addScoreLeft(){
    this->scoreLeft++;
}

void Score::addScoreRight(){
    this->scoreRight++;
}

int Score::getScoreLeft(){
    return this->scoreLeft;
}


int Score::getScoreRight(){
    return this->scoreRight;
}