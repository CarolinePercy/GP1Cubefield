#include <GameObject.h>

void GameObject::checkCollisions(GameObject** t_obstacles, float xOffset)
{
	int result = 0;

	c2AABB t_obstacleCollider;
	c2Poly t_triangle;

	t_triangle.count = 3;

	t_triangle.verts[0] = c2V(this->Ship::vertex[15] + this->position.x, this->Ship::vertex[17] + this->position.z);
	t_triangle.verts[1] = c2V(this->Ship::vertex[9] + this->position.x, this->Ship::vertex[12] + this->position.z);
	t_triangle.verts[2] = c2V(this->Ship::vertex[24] + this->position.x, this->Ship::vertex[26] + this->position.z);


	for (int i = 0; i < NUM_OF_CUBES; i++)
	{
		t_obstacleCollider.min = c2V(t_obstacles[i]->Cube::vertex[21] + t_obstacles[i]->getPosition().x + xOffset,
			t_obstacles[i]->Cube::vertex[23] + t_obstacles[i]->getPosition().z);
		t_obstacleCollider.max = c2V(t_obstacles[i]->Cube::vertex[15] + t_obstacles[i]->getPosition().x + xOffset,
			t_obstacles[i]->Cube::vertex[17] + t_obstacles[i]->getPosition().z);

		result = c2AABBtoPoly(t_obstacleCollider, &t_triangle, NULL);

		if (result != 0)
		{
			ScreenController::screen = State::Dying;
		}
	}
}

GameObject::GameObject(Type shapeType)
{
	currentShape = shapeType;

	switch (currentShape)
	{
	case Type::Cube:
		// Copy the Cube contents into GameObject
		memcpy(this->Cube::vertex, CubeShape::vertices, sizeof(this->Cube::vertex));
		memcpy(this->Cube::color, CubeShape::colors, sizeof(this->Cube::color));
		memcpy(this->Cube::uv, CubeShape::uvs, sizeof(this->Cube::uv));

		// Copy UV's to all faces
		for (int i = 1; i < 6; i++)
			memcpy(&Cube::uv[i * 4 * 2], &Cube::uv[0], 2 * 4 * sizeof(GLfloat));

		memcpy(this->Cube::index, CubeShape::indices, sizeof(this->Cube::index));
		break;
	case Type::Ship:
		// Copy the Ship contents into GameObject
		memcpy(this->Ship::vertex, ShipShape::vertices, sizeof(this->Ship::vertex));
		memcpy(this->Ship::color, ShipShape::colors, sizeof(this->Ship::color));
		memcpy(this->Ship::uv, ShipShape::uvs, sizeof(this->Ship::uv));

		// Copy UV's to all faces
		for (int i = 1; i < 3; i++)
			memcpy(&Ship::uv[i * 3 * 2], &Ship::uv[0], 2 * 3 * sizeof(GLfloat));

		memcpy(this->Ship::index, ShipShape::indices, sizeof(this->Ship::index));
		break;
	default:
		break;
	}

	this->position = vec3();
}

GameObject::~GameObject() 
{
}

vec3 GameObject::getPosition() { return this->position; }
void GameObject::setPosition(vec3 position) { this->position = position; }

void GameObject::move(vec3 addition)
{
	this->position += addition;
}

void GameObject::update(sf::FloatRect t_window, float xOffset)
{
	if (ScreenController::screen == State::Gameplay)
	{
		move(vec3(0, 0, 0.025));
	}

	else if (ScreenController::screen == State::StartScreen)
	{
		move(vec3(0, 0, 0.005));
	}

	if (offScreen(t_window))
	{
		respawn(-xOffset);
	}
}

void GameObject::update(GameObject** t_obstacles, float xOffset)
{
	checkCollisions(t_obstacles, xOffset);
}

bool GameObject::offScreen(sf::FloatRect t_window)
{
	return this->position.z > 8;
}

void GameObject::respawn(float xOffset)
{
	int xPos = (rand() % 50) - 25;
	int zPos = (rand() % 20) + 20;

	this->tint = cubeColour((rand() % 3) + 1);
	this->position = vec3(xPos + xOffset, -3.7f, -zPos);
}

// Returns the first element of the Vertex array
GLfloat* GameObject::getVertex() 
{
	switch (currentShape)
	{
	case Type::Cube:
		return this->Cube::vertex;
		break;
	case Type::Ship:
		return this->Ship::vertex;
		break;
	default:
		break;
	}
}
// 3 Vertices
int GameObject::getVertexCount() 
{
	switch (currentShape)
	{
	case Type::Cube:
		return ARRAY_SIZE(Cube::vertex) / 3;
		break;
	case Type::Ship:
		return ARRAY_SIZE(Ship::vertex) / 3;
		break;
	default:
		break;
	}
}

// Returns the first element of the Color array
GLfloat* GameObject::getColor()
{
	switch (currentShape)
	{
	case Type::Cube:
		return this->Cube::color;
		break;
	case Type::Ship:
		return this->Ship::color;
		break;
	default:
		break;
	}
}
cubeColour GameObject::getTint()
{
	return tint;
}
// 4 Colors RGBA
int GameObject::getColorCount() 
{
	switch (currentShape)
	{
	case Type::Cube:
		return ARRAY_SIZE(Cube::color) / 4;
		break;
	case Type::Ship:
		return ARRAY_SIZE(Ship::color) / 4;
		break;
	default:
		break;
	}
}

// Returns the first element of the UV array
GLfloat* GameObject::getUV() 
{
	switch (currentShape)
	{
	case Type::Cube:
		return this->Cube::uv;
		break;
	case Type::Ship:
		return this->Ship::uv;
		break;
	default:
		break;
	}
}
// 3 Colors RGB
int GameObject::getUVCount() 
{ 
	switch (currentShape)
	{
	case Type::Cube:
		return ARRAY_SIZE(Cube::uv);
		break;
	case Type::Ship:
		return ARRAY_SIZE(Ship::uv);
		break;
	default:
		break;
	}
}

// Returns the first element of the Index array
GLfloat* GameObject::getIndex() 
{
	switch (currentShape)
	{
	case Type::Cube:
		return this->Cube::index;
		break;
	case Type::Ship:
		return this->Ship::index;
		break;
	default:
		break;
	}
}
// 3 Colors RGB
int GameObject::getIndexCount() 
{
	switch (currentShape)
	{
	case Type::Cube:
		return ARRAY_SIZE(Cube::index) / 3;
		break;
	case Type::Ship:
		return ARRAY_SIZE(Ship::index) / 3;
		break;
	default:
		return 0;
		break;
	}
}