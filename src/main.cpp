#include <SFML/Graphics.hpp>
#include <iostream>
#include <typeinfo>

#include "../headers/Rectangle.h"
#include "../headers/Ball.h"
#include "../headers/Collision.h"
using namespace std;





int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Titre");
	// récupération de la taille de la fenêtre
	sf::Vector2u size = window.getSize();
	unsigned int WindowWidth = size.x;
	unsigned int WindowHeight = size.y;



	//création du rectangle de gauche
	Rectangle r_gauche(30.0, 10.0); //objet de type Rectangle
	r_gauche.createRectangle();

	//création du rectangle de droite
	Rectangle r_droite(700.0, 10.0); //objet de type Rectangle
	r_droite.createRectangle();

	//création de la balle
	Ball main_ball(400, 400, 13.0);
	main_ball.setMoveCoeffX(-5);
	main_ball.setMoveCoeffY(-5);

	main_ball.createBall();

	bool touched = false;

	// Création du gestionnaire de collision du jeu
	Collision collHand(r_gauche, main_ball, r_droite);



    
    while (window.isOpen()){
		collHand.start();

		// mouvements des 2 rectangles
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) r_gauche.move(-1);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) r_gauche.move(1);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) r_droite.move(-1);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)) r_droite.move(1);



		float YpositionBallArrondie = (std::round(10 * main_ball.getPositionY() ) / 10);
		float XpositionBallArrondie = (std::round(10 * main_ball.getPositionX() ) / 10);
		float Y_rec_bottom = r_gauche.getPositionY()+200;
		float Y_rec_top = r_gauche.getPositionY();

		//if( YpositionBallArrondie < Y_rec_bottom && YpositionBallArrondie > Y_rec_top && XpositionBallArrondie == 30.0){
				//cout<<"touched !"<<endl;
				//touched = true;

		//gérer la collision à gauches
		collHand.handleLeftRectColl();
		collHand.handleRightRectColl();
		collHand.handleGlobalCollision(WindowWidth, WindowHeight);









		// 1. Créer l'objet Font et charger le fichier
		sf::Font font;
		if (!font.loadFromFile("/Library/Fonts/Arial Unicode.ttf")) {
    		// Gérer l'erreur si le fichier n'est pas trouvé
		}

		// 2. Créer l'objet Text (vide)
		sf::Text text;

		// 3. Lui donner la police (OBLIGATOIRE avant d'afficher)
		text.setFont(font);

		// 4. Configurer le reste
		// On crée une chaîne de caractères propre
		//std::string debugCoord = "X : " + std::to_string(main_ball.getPositionX())
                       //+ "  Y : " + std::to_string(main_ball.getPositionY());

		std::string debugCoord = "Y balle : " + std::to_string(YpositionBallArrondie) + "  Y carré top : " + std::to_string( r_gauche.getPositionY()) +  "  Y carré bottom : " + std::to_string( r_gauche.getPositionY()+200 );

		//std::string debugCoord = "(" + std::to_string(main_ball.getMoveCoeffX()) + ";" + std::to_string(main_ball.getMoveCoeffY()) + ")" ;

		// On l'applique à l'objet text
		text.setString(debugCoord);
		text.setCharacterSize(24);


    sf::Event event;
        while (window.pollEvent(event)){


            switch(event.type){
                case sf::Event::Closed:
                    window.close();
                    break;

                
                default:
                    break;
            }
            

        }

        window.clear();

        window.draw(r_gauche.getShape());
        window.draw(r_droite.getShape());
        window.draw(main_ball.getShape());
        window.draw(text);



        window.display();
    }
 
    return EXIT_SUCCESS;
}
