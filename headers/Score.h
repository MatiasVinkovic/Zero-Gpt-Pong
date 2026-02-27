#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <typeinfo>
#include <iostream>

#ifndef PONG_SCORE_H
#define PONG_SCORE_H



class Score {
    private:
        int scoreLeft, scoreRight;


    public:

        //constructeur
        Score();

        //ajouter des points
        void addScoreLeft();
        void addScoreRight();

        int getScoreLeft();
        int getScoreRight();

        //gérer la partie





};





#endif 