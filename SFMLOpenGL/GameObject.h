#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#if !defined(ARRAY_SIZE)
#define ARRAY_SIZE(x) (sizeof((x)) / sizeof((x)[0]))
#endif

#include <SFML/Graphics.hpp>

#include <stdio.h>
#include <string.h>
#include <Cube.h>
#include <Ship.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ScreenController.h"

#include <tinyc2.h>

using namespace glm;

static const int NUM_OF_CUBES = 40;

enum class Type
{
	Cube,
	Ship
};

enum class cubeColour
{
	Null,
	Yellow,
	Orange,
	Red
};

class GameObject : public Cube, public Ship
{
private:
	// Cube Elements

	cubeColour tint = cubeColour::Null;
	Type currentShape;
	vec3 position;

	void checkCollisions(GameObject** t_obstacles, float xOffset);
public:
	GameObject(Type shapeType);
	~GameObject();

	vec3 getPosition();
	void setPosition(vec3 position);
	void move(vec3 addition);

	void update(sf::FloatRect t_window, float xOffset);
	void update(GameObject** t_obstacles, float xOffset);

	bool offScreen(sf::FloatRect t_window);
	void respawn(float xOffset);

	// Returns the first element of the array
	GLfloat* getVertex();
	// 3 Vertices
	int getVertexCount();

	// Returns the first element of the array
	GLfloat* getColor();
	cubeColour getTint();
	// 3 Colors RGB
	int getColorCount();

	// Returns the first element of the array
	GLfloat* getUV();
	// 3 Colors RGB
	int getUVCount();

	// Returns the first element of the array
	GLfloat* getIndex();
	// 3 Colors RGB
	int getIndexCount();
};

#endif // !GAME_OBJECT_H

