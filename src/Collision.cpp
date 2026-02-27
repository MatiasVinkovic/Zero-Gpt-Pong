//
// Created by Matias Vinkovic on 24/02/2026.
//

#include "../headers/Collision.h"
#include "../headers/Rectangle.h"
#include "../headers/Ball.h"

using namespace std;


//constructeur par défault : on donne à rec et main_ball
Collision::Collision (Rectangle& new_rec, Ball& new_main_ball, Rectangle& new_recRight) : rec(new_rec), main_ball(new_main_ball), recRight(new_recRight){
}




void Collision::handleLeftRectColl() {
    float YpositionBallArrondie (std::round(10 * main_ball.getPositionY() ) / 10); //y balle
    float XpositionBallArrondie (std::round(10 * main_ball.getPositionX() ) / 10); //x balle
    float Y_rec_bottom (rec.getPositionY()+200);
    float Y_rec_top (rec.getPositionY());

    int coeffX = this->main_ball.getMoveCoeffX();
    int coeffY = this->main_ball.getMoveCoeffY();

    if( YpositionBallArrondie < Y_rec_bottom && YpositionBallArrondie > Y_rec_top && XpositionBallArrondie == 30.0){
        cout<<"touched !"<<endl;
        cout<<this->main_ball.getMoveCoeffX()<<endl;
        cout<<";"<<endl;
        cout<<this->main_ball.getMoveCoeffY()<<endl;

        if (coeffX < 0 && coeffY < 0) {
            //réaliser le nouveau mouvement
            this->main_ball.startBallMovement(-coeffX,coeffY);
            cout<<this->main_ball.getMoveCoeffX()<<endl;
            cout<<""<<endl;
            cout<<this->main_ball.getMoveCoeffY()<<endl;
        }
        else {
            //réaliser le nouveau mouvement
            this->main_ball.startBallMovement(-coeffX,coeffY);
            cout<<this->main_ball.getMoveCoeffX()<<endl;
            cout<<""<<endl;
            cout<<this->main_ball.getMoveCoeffY()<<endl;
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

    if( YpositionBallArrondie < Y_rec_bottom && YpositionBallArrondie > Y_rec_top && XpositionBallArrondie == 700.0){
        cout<<"touched !"<<endl;
        cout<<this->main_ball.getMoveCoeffX()<<endl;
        cout<<";"<<endl;
        cout<<this->main_ball.getMoveCoeffY()<<endl;

        if (coeffX > 0 && coeffY > 0) {
            //réaliser le nouveau mouvement
            this->main_ball.startBallMovement(-coeffX,coeffY);
            cout<<this->main_ball.getMoveCoeffX()<<endl;
            cout<<""<<endl;
            cout<<this->main_ball.getMoveCoeffY()<<endl;
        }
        else {
            //réaliser le nouveau mouvement
            this->main_ball.startBallMovement(-coeffX,coeffY);
            cout<<this->main_ball.getMoveCoeffX()<<endl;
            cout<<""<<endl;
            cout<<this->main_ball.getMoveCoeffY()<<endl;
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
    //côté gauche
    else if( XpositionBallArrondie <= 0){
        if (coeffX < 0 && coeffY > 0) {
            this->main_ball.startBallMovement(-coeffX,coeffY);
        }
        else if (coeffX < 0 && coeffY < 0) {
            this->main_ball.startBallMovement(-coeffX,coeffY);
        }
    }
    //côté droit
    else if( XpositionBallArrondie >= windowW){
        if (coeffX > 0 && coeffY < 0) {
            this->main_ball.startBallMovement(-coeffX,coeffY);
        }
        else if (coeffX > 0 && coeffY > 0) {
            this->main_ball.startBallMovement(-coeffX,coeffY);
        }
    }
}

void Collision::start() {
    this->main_ball.startBallMovement(main_ball.getMoveCoeffX(), main_ball.getMoveCoeffY());
}