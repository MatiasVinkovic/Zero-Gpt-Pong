# Nom de l'exécutable final
TARGET = prog

# Compilateur
CXX = g++

# Options de compilation (C++17 + Chemins SFML)
CXXFLAGS = -std=c++17 -I/opt/homebrew/opt/sfml@2/include

# Options de lien (Bibliothèques SFML)
LDFLAGS = -L/opt/homebrew/opt/sfml@2/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Liste de tes fichiers sources
SOURCES = main.cpp Rectangle.cpp Ball.cpp Collision.cpp

# Génération automatique des fichiers objets (.o)
OBJECTS = $(SOURCES:.cpp=.o)

# Règle principale : on compile tout
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Comment compiler chaque fichier .cpp en .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Pour nettoyer les fichiers temporaires
clean:
	rm -f $(OBJECTS) $(TARGET)