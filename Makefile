CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 $(shell sdl2-config --cflags)
LDFLAGS  := $(shell sdl2-config --libs) -lSDL2_mixer -lm

SRCS   := src/main.cpp src/Ball.cpp src/Paddle.cpp src/Game.cpp
OBJS   := $(patsubst src/%.cpp, build/%.o, $(SRCS))
TARGET := pong

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -Isrc -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build $(TARGET)
