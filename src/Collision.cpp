//
// Created by Matias Vinkovic on 24/02/2026.
//

#include "../headers/Collision.h"
#include "../headers/Rectangle.h"
#include "../headers/Ball.h"
#include <random>
#include <SFML/Graphics.hpp>

using namespace std;


//constructeur par défault : on donne à rec et main_ball
Collision::Collision (Rectangle& new_rec, Ball& new_main_ball, Rectangle& new_recRight, Score& scoreManager_) : rec(new_rec), main_ball(new_main_ball), recRight(new_recRight), scoreManager(scoreManager_) {
    this->listeTouche[0] = sf::Keyboard::Space;
    this->listeTouche[1] = sf::Keyboard::N;
    this->listeTouche[2] = sf::Keyboard::Y;
    this->listeTouche[3] = sf::Keyboard::G;
    this->listeTouche[4] = sf::Keyboard::U;
    
}






void Collision::handleLeftRectColl() {
    float YpositionBallArrondie (std::round(10 * main_ball.getPositionY() ) / 10); //y balle
    float XpositionBallArrondie (std::round(10 * main_ball.getPositionX() ) / 10); //x balle
    float Y_rec_bottom (rec.getPositionY()+200);
    float Y_rec_top (rec.getPositionY());

   

    int coeffX = this->main_ball.getMoveCoeffX();
    int coeffY = this->main_ball.getMoveCoeffY();
                    //40 pour avoir la marge de la largeur du rectangle, same pour 680 en dessous
    if( YpositionBallArrondie < Y_rec_bottom && YpositionBallArrondie > Y_rec_top && XpositionBallArrondie == 40.0){
        

        if (coeffX < 0 && coeffY < 0) {
            //réaliser le nouveau mouvement
            cout<<this->toucheBoost<<endl;
            if(sf::Keyboard::isKeyPressed(toucheBoost)){
                this->main_ball.startBallMovementAcce(-coeffX,coeffY, true);
            };
            this->main_ball.startBallMovementAcce(-coeffX,coeffY, false);
            changeBoost();
        }
        else {
            //réaliser le nouveau mouvement
            
            if(sf::Keyboard::isKeyPressed(toucheBoost)){
                this->main_ball.startBallMovementAcce(-coeffX,coeffY, true);
            };
            this->main_ball.startBallMovementAcce(-coeffX,coeffY, false);
            changeBoost();
        }
    }
}

void Collision::handleRightRectColl() {
    float YpositionBallArrondie (std::round(10 * main_ball.getPositionY() ) / 10); //y balle
    float XpositionBallArrondie (std::round(10 * main_ball.getPositionX() ) / 10); //x balle
    float Y_rec_bottom (recRight.getPositionY()+200);
    float Y_rec_top (recRight.getPositionY());

    int coeffX = this->main_ball.getMoveCoeffX();
    int coeffY = this->main_ball.getMoveCoeffY();

    if( YpositionBallArrondie < Y_rec_bottom && YpositionBallArrondie > Y_rec_top && XpositionBallArrondie == 680.0){


        if (coeffX >= 0 && coeffY >= 0) {
            //réaliser le nouveau mouvement
            if(sf::Keyboard::isKeyPressed(toucheBoost)){
                this->main_ball.startBallMovementAcce(-coeffX,coeffY, true);
            };
            this->main_ball.startBallMovementAcce(-coeffX,coeffY, false);
            changeBoost();
        }
        else  {
            //réaliser le nouveau mouvement
            if(sf::Keyboard::isKeyPressed(toucheBoost)){
                this->main_ball.startBallMovementAcce(-coeffX,coeffY, true);
            };
            this->main_ball.startBallMovementAcce(-coeffX,coeffY, false);
            changeBoost();
        }
    }
}

void Collision::handleGlobalCollision(int windowW, int windowH) {
    float YpositionBallArrondie (std::round(10 * main_ball.getPositionY() ) / 10); //y balle
    float XpositionBallArrondie (std::round(10 * main_ball.getPositionX() ) / 10); //x balle

    int coeffX = this->main_ball.getMoveCoeffX();
    int coeffY = this->main_ball.getMoveCoeffY();


    // plafond
    if( YpositionBallArrondie <= 0){
        if (coeffX > 0 && coeffY < 0) {
            this->main_ball.startBallMovement(coeffX,-coeffY);
        }
        else if (coeffX < 0 && coeffY < 0) {
            this->main_ball.startBallMovement(coeffX,-coeffY);
        }
    }
    //sol
    else if( YpositionBallArrondie >= windowH-10){
        if (coeffX > 0 && coeffY > 0) {
            this->main_ball.startBallMovement(coeffX,-coeffY);
        }
        else if (coeffX < 0 && coeffY > 0) {
            this->main_ball.startBallMovement(coeffX,-coeffY);
        }
    }
    //côté gauche --> but pour la partie droite
    else if( XpositionBallArrondie <= 0){
        cout<<"goal for right !"<<endl;
        this->scoreManager.addScoreRight();
        if (coeffX < 0 && coeffY > 0) {
            this->main_ball.startBallMovement(-coeffX,coeffY);
        }
        else if (coeffX < 0 && coeffY < 0) {
            this->main_ball.startBallMovement(-coeffX,coeffY);
        }
    }
    //côté droit --> but pour la partie gauche
    else if( XpositionBallArrondie >= windowW){
        cout<<"goal for left !"<<endl;
        this->scoreManager.addScoreLeft();
        if (coeffX > 0 && coeffY < 0) {
            this->main_ball.startBallMovement(-coeffX,coeffY);
             cout<<"in"<<endl;
        }
        else if (coeffX > 0 && coeffY > 0) {
            this->main_ball.startBallMovement(-coeffX,coeffY);
        }
    }
}

void Collision::start() {
    this->main_ball.startBallMovementAcce(main_ball.getMoveCoeffX(), main_ball.getMoveCoeffY(), false);
}

std::string Collision::keyToString(sf::Keyboard::Key key) {
    switch (key) {
        case sf::Keyboard::Space:  return "Space";
        case sf::Keyboard::Y:      return "Y";
        case sf::Keyboard::N:      return "N";
        case sf::Keyboard::G:      return "G";
        case sf::Keyboard::U:      return "U";


        // Ajoute les touches dont tu as besoin...
        default:      return "Unknown Key";
    }
}

std::string Collision::getBoostTouche(){
    return this->keyToString(toucheBoost);
}

void Collision::changeBoost(){
    std::random_device rd; 
    std::default_random_engine re(rd()); 
    std::uniform_int_distribution<int> distrib{0, 4};
    int resultatRand = distrib(re);
    this->toucheBoost = listeTouche[resultatRand];
}