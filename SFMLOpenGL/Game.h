#ifndef GAME_H
#define GAME_H

#include <string>
#include <sstream>

#include <iostream>
#include <GL/glew.h>
#include <GL/wglew.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <time.h>


#include <SFML/OpenGL.hpp>

#include <Debug.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <GameObject.h>

using namespace std;
using namespace sf;
using namespace glm;

class Game
{
public:
	Game();
	Game(sf::ContextSettings settings);
	~Game();
	void run();
private:

	GameObject* game_object[NUM_OF_CUBES];
	GameObject* player;
	RenderWindow window;

	RectangleShape background;
	RectangleShape end;
	RectangleShape title;
	RectangleShape tutorial;
	RectangleShape button;
	RectangleShape pauseMenu;

	Font mainFont;
	Font newGameFont;
	Clock clock;
	Time time;

	int playerScore = 0;
	int highScore = 0;

	std::string builtFS[3];

	bool animate = false;
	vec3 rotationAnimation = vec3(0.0f);
	vec3 rotationOffset = vec3(0.0f, -3.3f, 0.0f);
	vec3 translateAnimation = vec3(0.0f);

	float translationTracker = 0.0f;

	float rotation = 0.0f;
	bool isRunning = false;

	void initialize();
	void update();
	void render();
	void findShaderVariables();
	void unload();

	void translateCubes();
	bool shipCubeCollisionCheck();
};

#endif  // ! GAME_H