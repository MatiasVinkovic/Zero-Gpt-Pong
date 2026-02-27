#include <iostream>
#include <typeinfo>
#include <SFML/Graphics.hpp>
#include "../headers/Ball.h"
#include "../headers/Collision.h"
#include "../headers/Rectangle.h"
#include "../headers/Score.h"
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
	main_ball.setMoveCoeffX(-1);
	main_ball.setMoveCoeffY(-1);

	main_ball.createBall();

	bool win = false;

	// Création du gestionnaire de collision du jeu
	Score scoreManager;
	Collision collHand(r_gauche, main_ball, r_droite, scoreManager);

	// --- AVANT LA BOUCLE WHILE ---
	sf::Font font;
	if (!font.loadFromFile("/Library/Fonts/Arial Unicode.ttf")) {
		// Gestion d'erreur
	}

	// Initialisation du score
	sf::Text textScore;
	textScore.setFont(font);
	textScore.setCharacterSize(40); // Plus gros pour le score
	textScore.setFillColor(sf::Color::White);

	// Initialisation du boost
	sf::Text boostTouche;
	boostTouche.setFont(font);
	boostTouche.setCharacterSize(20);
	boostTouche.setFillColor(sf::Color::Yellow); // Couleur différente pour distinguer
	



    
    while (window.isOpen()){
		
		
		if (scoreManager.getScoreLeft() >= 3 || scoreManager.getScoreRight() >= 3) {
        	win = true;
    	}
		
		if(!win){
			//Démarrer le mouvement de la balle
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

			//gérer les collisions de la balle dans la partie
			
			collHand.handleLeftRectColl();
			collHand.handleRightRectColl();
			collHand.handleGlobalCollision(WindowWidth, WindowHeight);
			
			

			
			// 1. Mise à jour du texte du score
			textScore.setString(std::to_string(scoreManager.getScoreLeft()) + " - " + std::to_string(scoreManager.getScoreRight()));
			
			// 2. Centrage dynamique du score
			sf::FloatRect textRect = textScore.getLocalBounds();
			textScore.setOrigin(textRect.left + textRect.width/2.0f, 0); // On met l'origine au centre horizontal
			textScore.setPosition(sf::Vector2f(WindowWidth/2.0f, 10.0f));

			// 3. Mise à jour du boost
			boostTouche.setString("BOOST : " + collHand.getBoostTouche());
			sf::FloatRect boostRect = boostTouche.getLocalBounds();
			boostTouche.setOrigin(boostRect.left + boostRect.width/2.0f, 0);
			boostTouche.setPosition(sf::Vector2f(WindowWidth/2.0f, WindowHeight - 50.0f)); // En bas de l'écran


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
			window.draw(textScore);
			window.draw(boostTouche);

			window.display();
		}
		else{
			window.clear(sf::Color::Black); // On efface le dernier état du jeu

			textScore.setString("WIN !");
			textScore.setCharacterSize(100);
			textScore.setFillColor(sf::Color::Green);
			
			// Centrage
			sf::FloatRect textRect = textScore.getLocalBounds();
			textScore.setOrigin(textRect.width / 2.f, textRect.height / 2.f);
			textScore.setPosition(WindowWidth / 2.f, WindowHeight / 2.f);
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

			window.draw(textScore);
			window.display(); // <--- TRÈS IMPORTANT : Sans ça, rien ne s'affiche
		}

    }
 
    return EXIT_SUCCESS;
}
