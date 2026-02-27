# Zero-Gpt-Pong
In this world of AI, what about a challenge to code a Pong game in C++ completely from scratch without any use of AI ?


run it : 
g++ main.cpp Rectangle.cpp Ball.cpp Collision.cpp -std=c++17 \
-I/opt/homebrew/opt/sfml@2/include \
-L/opt/homebrew/opt/sfml@2/lib \
-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio \
-o prog
