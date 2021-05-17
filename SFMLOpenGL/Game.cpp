#include <Game.h>
#include <Cube.h>
#include <Easing.h>

// Helper to convert Number to String for HUD
template <typename T>
string toString(T number)
{
	ostringstream oss;
	oss << number;
	return oss.str();
}

GLuint	vsid,		// Vertex Shader ID
		fsid,		// Fragment Shader ID
		fsid2,
		fsid3,
		progID,		// Program ID
		prog2ID,
		prog3ID,
		vao = 0,	// Vertex Array ID
		vbo,		// Vertex Buffer ID
		vib,		// Vertex Index Buffer
		to[1];		// Texture ID
GLint	positionID,	// Position ID
		colorID,	// Color ID
		textureID,	// Texture ID
		uvID,		// UV ID
		mvpID,		// Model View Projection ID
		x_offsetID, // X offset ID
		y_offsetID,	// Y offset ID
		z_offsetID;	// Z offset ID

GLenum	error;		// OpenGL Error Code


//Please see .//Assets//Textures// for more textures
const string filename = ".//Assets//Textures//gameCube.tga";

int width;						// Width of texture
int height;						// Height of texture
int comp_count;					// Component of texture

unsigned char* img_data;		// image data

mat4 mvp, projection, 
		view, cubeModel, shipModel;			// Model View Projection

					// Game font

float x_offset, y_offset, z_offset; // offset on screen (Vertex Shader)

Game::Game() : 
	window(VideoMode(800, 600), 
	"Introduction to OpenGL Texturing")
{
}

Game::Game(sf::ContextSettings settings) : 
	window(VideoMode(800, 600), 
	"Introduction to OpenGL Texturing", 
	sf::Style::Default, 
	settings)
{
	srand(std::time(NULL));

	int xPos;
	int zPos;
	cubeColour cubeTint;

	for (int i = 0; i < NUM_OF_CUBES; i++)
	{
		xPos = (rand() % 16) - 8;
		zPos = (rand() % 10) + 10;
		cubeTint = cubeColour((rand() % 3) + 1);

		game_object[i] = new GameObject(Type::Cube);

		game_object[i]->respawn(0.0);
	}

	player = new GameObject(Type::Ship);
	player->setPosition(vec3(-0.05f, -3.3f, 3.0f));
}

Game::~Game()
{
}


void Game::run()
{
	initialize();

	Event event;

	float start_value = 0.0f;
	float end_value = 1.0f;

	sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	sf::Clock clock;

	clock.restart();

	while (isRunning) {

#if (DEBUG >= 2)
		DEBUG_MSG("Game running...");
#endif

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				isRunning = false;
			}
		}

		if (ScreenController::screen == State::Gameplay) {
			if (event.type == Event::KeyReleased)
			{
				rotationAnimation = glm::vec3(0, 0, 0); //Rotate Y
				background.setRotation(0.0f);
				translateAnimation = glm::vec3(0, 0, 0);
			}


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				if (rotationAnimation.z > 0)
				{
					rotationAnimation.z = 0;
					background.setRotation(0.0f);
				}
				// Set Model Rotation
				if (!animate && abs(rotationAnimation.z) < 2.0f)
				{
					animate = true;
					rotationAnimation = glm::vec3(0, 0, -0.5 + rotationAnimation.z); //Rotate Y
				}

				translateAnimation = glm::vec3(0.03f, translateAnimation.y, translateAnimation.z);
			}

			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				if (rotationAnimation.z < 0)
				{
					rotationAnimation.z = 0;
					background.setRotation(0.0f);
				}
				// Set Model Rotation
				if (!animate && abs(rotationAnimation.z) < 2.0f)
				{
					animate = true;
					rotationAnimation = glm::vec3(0, 0, 0.5 + rotationAnimation.z); //Rotate Y
				}

				translateAnimation = glm::vec3(-0.03f, translateAnimation.y, translateAnimation.z);
			}

			//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			//{
			//	translateAnimation = glm::vec3(translateAnimation.x, translateAnimation.y, 0.05f);
			//}

			//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			//{
			//	translateAnimation = glm::vec3(translateAnimation.x, translateAnimation.y, -0.05f);
			//}

			if (animate)
			{
				background.setRotation(-rotationAnimation.z);
				animate = false;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
			{
				if (ScreenController::screen == State::Pause)
				{
					ScreenController::screen = State::Gameplay;
				}

				else if (ScreenController::screen == State::Gameplay)
				{
					ScreenController::screen = State::Pause;
				}
			}

			translateCubes();
		}

		timeSinceLastUpdate += clock.restart();

		if (timeSinceLastUpdate > timePerFrame)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				sf::Vector2f mouseCoords = sf::Vector2f(sf::Mouse::getPosition(window));
				if (ScreenController::screen == State::StartScreen && button.getGlobalBounds().contains(mouseCoords))
				{
					ScreenController::screen = State::Gameplay;
				}

				else if (ScreenController::screen == State::Pause && pauseMenu.getGlobalBounds().contains(mouseCoords))
				{
					ScreenController::screen = State::Gameplay;
				}
			}

			update();
			render();
		}
	}

#if (DEBUG >= 2)
	DEBUG_MSG("Calling Cleanup...");
#endif
	unload();
	
}


void Game::initialize()
{
	background.setSize(sf::Vector2f(1000, 400));
	background.setOrigin(sf::Vector2f(500, 200));
	background.setPosition(400, 550);
	background.setFillColor(sf::Color(144,140,140));

	end.setSize(sf::Vector2f(800, 600));
	end.setFillColor(sf::Color(0, 0, 0,0));

	title.setSize(sf::Vector2f(400, 100));
	title.setOrigin(200, 50);
	title.setPosition(400, 200);
	title.setFillColor(sf::Color(144, 140, 140, 100));

	tutorial.setSize(sf::Vector2f(400, 50));
	tutorial.setOrigin(200, 37.5);
	tutorial.setPosition(400, 300);
	tutorial.setFillColor(sf::Color(144, 140, 140, 100));

	button.setSize(sf::Vector2f(100, 25));
	button.setOrigin(50, 12.5);
	button.setPosition(280, 220);
	button.setFillColor(sf::Color(250, 250, 250));
	button.setOutlineColor(sf::Color::Black);
	button.setOutlineThickness(1);

	pauseMenu.setSize(sf::Vector2f(250, 100));
	pauseMenu.setOrigin(125, 50);
	pauseMenu.setPosition(400, 250);
	pauseMenu.setFillColor(sf::Color(100, 100, 100, 100));

	isRunning = true;
	GLint isCompiled = 0;
	GLint isLinked = 0;

	if (!(!glewInit())) { DEBUG_MSG("glewInit() failed"); }

	// Copy UV's to all faces
	for (int i = 1; i < 6; i++)
	{
		memcpy(&CubeShape::uvs[i * 4 * 2], &CubeShape::uvs[0], 2 * 4 * sizeof(GLfloat));
	}

	for (int i = 1; i < 3; i++)
	{
		memcpy(&ShipShape::uvs[i * 3 * 2], &ShipShape::uvs[0], 2 * 3 * sizeof(GLfloat));
	}

	DEBUG_MSG(glGetString(GL_VENDOR));
	DEBUG_MSG(glGetString(GL_RENDERER));
	DEBUG_MSG(glGetString(GL_VERSION));

	// Vertex Array Buffer
	glGenBuffers(1, &vbo);		// Generate Vertex Buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//glBufferData(GL_ARRAY_BUFFER, ((3 * ShipShape::VERTICES) + (4 * ShipShape::COLORS) + (2 * ShipShape::UVS)) * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, ((3 * CubeShape::VERTICES) + (4 * CubeShape::COLORS) + (2 * CubeShape::UVS)) * sizeof(GLfloat), NULL, GL_STATIC_DRAW);

	glGenBuffers(1, &vib); //Generate Vertex Index Buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vib);


	int countINDICES = player->getIndexCount();
	// Indices to be drawn


	// NOTE: uniforms values must be used within Shader so that they 
	// can be retreived
	const char* vs_src =
		"#version 400\n\r"
		""
		"in vec3 sv_position;"
		"in vec4 sv_color;"
		"in vec2 sv_uv;"
		""
		"out vec4 color;"
		"out vec2 uv;"
		""
		"uniform mat4 sv_mvp;"
		"uniform float sv_x_offset;"
		"uniform float sv_y_offset;"
		"uniform float sv_z_offset;"
		""
		"void main() {"
		"	color = sv_color;"
		"	uv = sv_uv;"
		//"	gl_Position = vec4(sv_position, 1);"
		"	gl_Position = sv_mvp * vec4(sv_position.x + sv_x_offset, sv_position.y + sv_y_offset, sv_position.z + sv_z_offset, 1 );"
		"}"; //Vertex Shader Src

	DEBUG_MSG("Setting Up Vertex Shader");

	vsid = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsid, 1, (const GLchar**)&vs_src, NULL);
	glCompileShader(vsid);

	// Check is Shader Compiled
	glGetShaderiv(vsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Vertex Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Vertex Shader Compilation Error");
	}

	const char* fsPart1 =
		"#version 400\n\r"
		""
		"uniform sampler2D f_texture;"
		""
		"in vec4 color;"
		"in vec2 uv;"
		""
		"out vec4 fColor;"
		""
		"void main() {";

	const char* colourDecision[] =
	{ "	fColor = color+ texture2D(f_texture, uv);",
	"	fColor = color+ texture2D(f_texture, uv) + vec4(-0.003, 0.18,-0.03,0);", 
	"	fColor = color+ texture2D(f_texture, uv) + vec4(-0.025, -0.4,-0.036,0);" };
	const char* fsPart2 = 
		""
		"}"; //Fragment Shader Src

	

	for (int j = 0; j < 3; j++)
	{
		builtFS[j] = fsPart1;
		builtFS[j] += colourDecision[j];
		builtFS[j] += fsPart2;


	}
	const char* fs_src[3] = { builtFS[0].c_str(),builtFS[1].c_str(), builtFS[2].c_str() };

	DEBUG_MSG("Setting Up Fragment Shader");

	fsid = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsid, 1, (const GLchar**)&fs_src[0], NULL);
	glCompileShader(fsid);

	fsid2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsid2, 1, (const GLchar**)&fs_src[1], NULL);
	glCompileShader(fsid2);

	fsid3 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsid3, 1, (const GLchar**)&fs_src[2], NULL);
	glCompileShader(fsid3);

	// Check is Shader Compiled
	glGetShaderiv(fsid, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Fragment Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Fragment Shader Compilation Error");
	}

	glGetShaderiv(fsid2, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Fragment Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Fragment Shader Compilation Error");
	}

	glGetShaderiv(fsid3, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_TRUE) {
		DEBUG_MSG("Fragment Shader Compiled");
		isCompiled = GL_FALSE;
	}
	else
	{
		DEBUG_MSG("ERROR: Fragment Shader Compilation Error");
	}

	DEBUG_MSG("Setting Up and Linking Shader");
	progID = glCreateProgram();
	glAttachShader(progID, vsid);
	glAttachShader(progID, fsid);
	glLinkProgram(progID);

	prog2ID = glCreateProgram();
	glAttachShader(prog2ID, vsid);
	glAttachShader(prog2ID, fsid2);
	glLinkProgram(prog2ID);

	prog3ID = glCreateProgram();
	glAttachShader(prog3ID, vsid);
	glAttachShader(prog3ID, fsid3);
	glLinkProgram(prog3ID);

	// Check is Shader Linked
	glGetProgramiv(progID, GL_LINK_STATUS, &isLinked);

	if (isLinked == 1) {
		DEBUG_MSG("Shader 1 Linked");
	}
	else
	{
		DEBUG_MSG("ERROR: Shader 1 Link Error");
	}

	glGetProgramiv(prog2ID, GL_LINK_STATUS, &isLinked);

	if (isLinked == 1) {
		DEBUG_MSG("Shader 2 Linked");
	}
	else
	{
		DEBUG_MSG("ERROR: Shader 2 Link Error");
	}

	glGetProgramiv(prog3ID, GL_LINK_STATUS, &isLinked);

	if (isLinked == 1) {
		DEBUG_MSG("Shader 3 Linked");
	}
	else
	{
		DEBUG_MSG("ERROR: Shader 3 Link Error");
	}

	// Set image data
	// https://github.com/nothings/stb/blob/master/stb_image.h
	img_data = stbi_load(filename.c_str(), &width, &height, &comp_count, 4);

	if (img_data == NULL)
	{
		DEBUG_MSG("ERROR: Texture not loaded");
	}

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &to[0]);
	glBindTexture(GL_TEXTURE_2D, to[0]);

	// Wrap around
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	// Filtering
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexParameter.xml
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Bind to OpenGL
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glTexImage2D.xml
	glTexImage2D(
		GL_TEXTURE_2D,			// 2D Texture Image
		0,						// Mipmapping Level 
		GL_RGBA,				// GL_RED, GL_GREEN, GL_BLUE, GL_ALPHA, GL_RGB, GL_BGR, GL_RGBA 
		width,					// Width
		height,					// Height
		0,						// Border
		GL_RGBA,				// Bitmap
		GL_UNSIGNED_BYTE,		// Specifies Data type of image data
		img_data				// Image Data
		);

	// Projection Matrix 
	projection = perspective(
		45.0f,					// Field of View 45 degrees
		4.0f / 3.0f,			// Aspect ratio
		5.0f,					// Display Range Min : 0.1f unit
		100.0f					// Display Range Max : 100.0f unit
		);

	// Camera Matrix
	view = lookAt(
		vec3(0.0f, 0.0f, 10.0f),	// Camera (x,y,z), in World Space
		vec3(0.0f, 0.0f, 0.0f),		// Camera looking at origin
		vec3(0.0f, 1.0f, 0.0f)		// 0.0f, 1.0f, 0.0f Look Down and 0.0f, -1.0f, 0.0f Look Up
		);

	// Model matrix
	cubeModel = mat4(
		1.0f					// Identity Matrix
		);

	shipModel = mat4(
		2.0f
	);

	// Enable Depth Test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	// Load Font
	if (!mainFont.loadFromFile("Assets/Fonts/times.ttf"))
	{
		std::cout << "Failed to load main font";
	}

	if (!newGameFont.loadFromFile("Assets/Fonts/micross.ttf"))
	{
		std::cout << "Failed to load new game font";
	}
}

void Game::update()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Updating...");
#endif

	for (int i = 0; i < NUM_OF_CUBES; i++)
	{
		game_object[i]->update(sf::FloatRect(0,0,window.getSize().x, window.getSize().y), cubeModel[3].x);
	}

	if (ScreenController::screen == State::Gameplay)
	{
		playerScore += 2;
		player->update(game_object, cubeModel[3].x);
	}

	if (ScreenController::screen == State::Dying)
	{
		if (end.getFillColor().a < 254)
		{
			end.setFillColor(sf::Color(0, 0, 0, end.getFillColor().a + 1));
		}
		
		else
		{
			ScreenController::screen = State::StartScreen;

			if (playerScore > highScore)
			{
				highScore = playerScore;
			}

			playerScore = 0;
		}
	}

	if (ScreenController::screen == State::StartScreen)
	{
		int test = end.getFillColor().a;
		test--;
		if (end.getFillColor().a > 0)
		{
			end.setFillColor(sf::Color(0, 0, 0, test));
		}

		rotationAnimation = glm::vec3(0, 0, 0); //Rotate Y
		background.setRotation(0.0f);
		translateAnimation = glm::vec3(0, 0, 0);
	}
}

void Game::render()
{

#if (DEBUG >= 2)
	DEBUG_MSG("Render Loop...");
#endif

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Save current OpenGL render states
	// https://www.sfml-dev.org/documentation/2.0/classsf_1_1RenderTarget.php#a8d1998464ccc54e789aaf990242b47f7
	window.pushGLStates();
	window.clear(sf::Color::White);

	string hud;
	Text text;

	text.setOrigin(0, 0);
	hud = (toString(playerScore));

	text = Text(hud, mainFont);

	text.setFillColor(sf::Color::Black);
	window.draw(text);

	window.draw(background);
	window.draw(end);

	if (ScreenController::screen == State::Pause)
	{
		window.draw(pauseMenu);

		hud = string("PAUSED");
		text = Text(hud, mainFont);
		text.setCharacterSize(35);
		text.setFillColor(sf::Color::White);
		text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
		text.setPosition(400.f, 220.f);
		window.draw(text);

		hud = string("Click here or Press P to continue");
		text = Text(hud, mainFont);
		text.setCharacterSize(17);
		text.setFillColor(sf::Color::White);
		text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
		text.setPosition(400.f, 270.f);
		window.draw(text);
	}

	if (ScreenController::screen == State::StartScreen)
	{
		window.draw(title);
		window.draw(tutorial);
		window.draw(button);

		hud = string("CUBEFIELD");
		text = Text(hud, mainFont);
		text.setCharacterSize(40);
		text.setFillColor(sf::Color::White);
		text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
		text.setPosition(400.f, 170.f);
		window.draw(text);

		hud = string("New Game");
		text = Text(hud, newGameFont);
		text.setCharacterSize(15);
		text.setFillColor(sf::Color::Black);
		text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
		text.setPosition(280.f, 215.f);
		window.draw(text);

		hud = string("Use left and right arrow keys to dodge cubes");
		text = Text(hud, mainFont);
		text.setCharacterSize(20);
		text.setFillColor(sf::Color::White);
		text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
		text.setPosition(400.f, 275.f);
		window.draw(text);

		hud = string("Press P to pause");
		text = Text(hud, mainFont);
		text.setCharacterSize(20);
		text.setFillColor(sf::Color::White);
		text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
		text.setPosition(400.f, 290.f);
		window.draw(text);

		hud = string("Top Score: ") + std::to_string(highScore);
		text = Text(hud, mainFont);
		text.setCharacterSize(20);
		text.setFillColor(sf::Color::White);
		text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
		text.setPosition(500.f, 215.f);
		window.draw(text);
	}

	window.popGLStates();

	// Rebind Buffers and then set SubData
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vib);

	// Use Progam on GPU
	glUseProgram(progID);

	findShaderVariables();

	// Find variables within the shader
	// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glGetAttribLocation.xml

	// Set Active Texture .... 32 GL_TEXTURE0 .... GL_TEXTURE31
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(textureID, 0); // 0 .... 31

	// To alter Camera modify view & projection
	mvp = projection * view * cubeModel;
	// Send transformation to shader mvp uniform [0][0] is start of array
	glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * CubeShape::INDICES * sizeof(GLuint), CubeShape::indices, GL_STATIC_DRAW);

	// Set pointers for each parameter (with appropriate starting positions)
// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, (VOID*)(3 * CubeShape::VERTICES * sizeof(GLfloat)));
	glVertexAttribPointer(uvID, 2, GL_FLOAT, GL_FALSE, 0, (VOID*)(((3 * CubeShape::VERTICES) + (4 * CubeShape::COLORS)) * sizeof(GLfloat)));

	// Enable Arrays
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);
	glEnableVertexAttribArray(uvID);

	// VBO Data....vertices, colors and UV's appended
	// Add the Vertices for all your GameOjects, Colors and UVS

	glBufferSubData(GL_ARRAY_BUFFER, 0 * CubeShape::VERTICES * sizeof(GLfloat), 3 * CubeShape::VERTICES * sizeof(GLfloat), game_object[0]->getVertex());
	glBufferSubData(GL_ARRAY_BUFFER, 3 * CubeShape::VERTICES * sizeof(GLfloat), 4 * CubeShape::COLORS * sizeof(GLfloat), CubeShape::colors);
	glBufferSubData(GL_ARRAY_BUFFER, ((3 * CubeShape::VERTICES) + (4 * CubeShape::COLORS)) * sizeof(GLfloat), 2 * CubeShape::UVS * sizeof(GLfloat), CubeShape::uvs);

	// Draw Element Arrays

	if (ScreenController::screen != State::Dying)
	{
		for (int i = 0; i < NUM_OF_CUBES; i++)
		{
			switch (game_object[i]->getTint())
			{
			case cubeColour::Yellow:
				glUseProgram(prog2ID);
				break;
			case cubeColour::Orange:
				glUseProgram(progID);
				break;
			case cubeColour::Red:
				glUseProgram(prog3ID);
				break;
			default:
				break;
			}

			glActiveTexture(GL_TEXTURE0);
			glUniform1i(textureID, 0); // 0 .... 31

			// To alter Camera modify view & projection
			mvp = projection * view * cubeModel;
			// Send transformation to shader mvp uniform [0][0] is start of array
			glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * CubeShape::INDICES * sizeof(GLuint), CubeShape::indices, GL_STATIC_DRAW);


			glUniform1f(x_offsetID, game_object[i]->getPosition().x);
			glUniform1f(y_offsetID, game_object[i]->getPosition().y);
			glUniform1f(z_offsetID, game_object[i]->getPosition().z);

			glDrawElements(GL_TRIANGLES, 3 * CubeShape::INDICES, GL_UNSIGNED_INT, NULL);
		}
	}

	glUseProgram(progID);

	// Update Model View Projection
	// For mutiple objects (cubes) create multiple models
	// To alter Camera modify view & projection
	if (ScreenController::screen == State::Gameplay || ScreenController::screen == State::Pause)
	{
		mvp = projection * view * shipModel;

		// Send transformation to shader mvp uniform [0][0] is start of array
		glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * ShipShape::INDICES * sizeof(GLuint), ShipShape::indices, GL_STATIC_DRAW);

		// Set pointers for each parameter (with appropriate starting positions)
		// https://www.khronos.org/opengles/sdk/docs/man/xhtml/glVertexAttribPointer.xml
		glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, (VOID*)(3 * ShipShape::VERTICES * sizeof(GLfloat)));
		glVertexAttribPointer(uvID, 2, GL_FLOAT, GL_FALSE, 0, (VOID*)(((3 * ShipShape::VERTICES) + (4 * ShipShape::COLORS)) * sizeof(GLfloat)));

		// VBO Data....vertices, colors and UV's appended
		// Add the Vertices for all your GameOjects, Colors and UVS

		glBufferSubData(GL_ARRAY_BUFFER, 0 * ShipShape::VERTICES * sizeof(GLfloat), 3 * ShipShape::VERTICES * sizeof(GLfloat), player->getVertex());
		glBufferSubData(GL_ARRAY_BUFFER, 3 * ShipShape::VERTICES * sizeof(GLfloat), 4 * ShipShape::COLORS * sizeof(GLfloat), ShipShape::colors);
		glBufferSubData(GL_ARRAY_BUFFER, ((3 * ShipShape::VERTICES) + (4 * ShipShape::COLORS)) * sizeof(GLfloat), 2 * ShipShape::UVS * sizeof(GLfloat), ShipShape::uvs);

		glUniform1f(x_offsetID, player->getPosition().x);
		glUniform1f(y_offsetID, player->getPosition().y);
		glUniform1f(z_offsetID, player->getPosition().z);

		glDrawElements(GL_TRIANGLES, 3 * ShipShape::INDICES, GL_UNSIGNED_INT, NULL);
	}

	window.display();
	// Disable Arrays
	glDisableVertexAttribArray(positionID);
	glDisableVertexAttribArray(colorID);
	glDisableVertexAttribArray(uvID);

	// Unbind Buffers with 0 (Resets OpenGL States...important step)
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Reset the Shader Program to Use.
	glUseProgram(0);

	// Check for OpenGL Error code
	error = glGetError();
	if (error != GL_NO_ERROR) {
		DEBUG_MSG(error);
	}
}

void Game::findShaderVariables()
{
	positionID = glGetAttribLocation(progID, "sv_position");
	if (positionID < 0) { DEBUG_MSG("positionID not found"); }

	colorID = glGetAttribLocation(progID, "sv_color");
	if (colorID < 0) { DEBUG_MSG("colorID not found"); }

	uvID = glGetAttribLocation(progID, "sv_uv");
	if (uvID < 0) { DEBUG_MSG("uvID not found"); }

	textureID = glGetUniformLocation(progID, "f_texture");
	if (textureID < 0) { DEBUG_MSG("textureID not found"); }

	mvpID = glGetUniformLocation(progID, "sv_mvp");
	if (mvpID < 0) { DEBUG_MSG("mvpID not found"); }

	x_offsetID = glGetUniformLocation(progID, "sv_x_offset");
	if (x_offsetID < 0) { DEBUG_MSG("x_offsetID not found"); }

	y_offsetID = glGetUniformLocation(progID, "sv_y_offset");
	if (y_offsetID < 0) { DEBUG_MSG("y_offsetID not found"); }

	z_offsetID = glGetUniformLocation(progID, "sv_z_offset");
	if (z_offsetID < 0) { DEBUG_MSG("z_offsetID not found"); };
}

void Game::unload()
{
#if (DEBUG >= 2)
	DEBUG_MSG("Cleaning up...");
#endif
	glDetachShader(progID, vsid);	// Shader could be used with more than one progID
	glDetachShader(progID, fsid);	// ..

	glDetachShader(prog2ID, vsid);	// Shader could be used with more than one progID
	glDetachShader(prog2ID, fsid2);	// ..

	glDetachShader(prog3ID, vsid);	// Shader could be used with more than one progID
	glDetachShader(prog3ID, fsid3);	// ..

	glDeleteShader(vsid);			// Delete Vertex Shader
	glDeleteShader(fsid);			// Delete Fragment Shader
	glDeleteShader(fsid2);
	glDeleteShader(fsid3);

	glDeleteProgram(progID);		// Delete Shader
	glDeleteProgram(prog2ID);
	glDeleteProgram(prog3ID);

	glDeleteBuffers(1, &vbo);		// Delete Vertex Buffer
	glDeleteBuffers(1, &vib);		// Delete Vertex Index Buffer
	stbi_image_free(img_data);		// Free image stbi_image_free(..)
}

void Game::translateCubes()
{
	cubeModel = translate(cubeModel, translateAnimation);
}

bool Game::shipCubeCollisionCheck()
{
	return false;
}

