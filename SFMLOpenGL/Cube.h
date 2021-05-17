#ifndef CUBE_H
#define CUBE_H

#include <gl/glew.h>

//Cube Vertices
/*
		  (-1.0f, 1.0f, -1.0f)          (1.0f, 1.0f, -1.0f)
		          [7]                          [6]
		          #-----------------------------#
		         /|                            /|
		        / |                           / |
	  (-1.0f, 1.0f, 1.0f)           (1.0f, 1.0f, 1.0f)
		  [3] /                         [2] /
		     #-----------------------------#    |
		     |    |                        |    |
		     |    |                        |    |
		     |   [4]                       |   [5]
		  (-1.0f, -1.0f, -1.0f)         (1.0f, -1.0f, -1.0f)
		     |    #-----------------------------#
		     |   /                         |   /
		     |  /                          |  /
		     | /                           | /
		     |/                            |/
		     #-----------------------------#
		    [0]                           [1]
	(-1.0f, -1.0f, 1.0f)         (1.0f, -1.0f, 1.0f)
*/
namespace CubeShape
{
	const int VERTICES = 24;	// Total Number of Vertices
	const int INDICES = 12;	// Total Number of Indexes
	const int UVS = 48;	// Total Number of UVs
	const int COLORS = 24;	// Total Number of Colors

	static const GLfloat vertices[] =
	{
		// Front Face
		-0.50f, -0.50f,  0.50f,	// [0]	// ( 0)
		 0.50f, -0.50f,  0.50f,	// [1]	// ( 1)
		 0.50f,  0.50f,  0.50f,	// [2]	// ( 2)
		-0.50f,  0.50f,  0.50f,	// [3]	// ( 3)

		// Top Face
		-0.50f,  0.50f,  0.50f,	// [3]	// ( 4) //12
		 0.50f,  0.50f,  0.50f,	// [2]	// ( 5) //15
		 0.50f,  0.50f, -0.50f,	// [6]	// ( 6) //18
		-0.50f,  0.50f, -0.50f,	// [7]	// ( 7) //21

		// Back Face
		 0.50f, -0.50f, -0.50f,	// [5]	// ( 8)
		-0.50f, -0.50f, -0.50f, // [4]	// ( 9)
		-0.50f,  0.50f, -0.50f,	// [7]	// (10)
		 0.50f,  0.50f, -0.50f,	// [6]	// (11)

		// Bottom Face
		-0.50f, -0.50f, -0.50f, // [4]	// (12) 36
		 0.50f, -0.50f, -0.50f, // [5]	// (13) 39
		 0.50f, -0.50f,  0.50f, // [1]	// (14) 42
		-0.50f, -0.50f,  0.50f, // [0]	// (15) 45

		// Left Face
		-0.50f, -0.50f, -0.50f, // [4]	// (16)
		-0.50f, -0.50f,  0.50f, // [0]	// (17)
		-0.50f,  0.50f,  0.50f, // [3]	// (18)
		-0.50f,  0.50f, -0.50f, // [7]	// (19)

		// Right Face
		 0.50f, -0.50f,  0.50f, // [1]	// (20)
		 0.50f, -0.50f, -0.50f, // [5]	// (21)
		 0.50f,  0.50f, -0.50f, // [6]	// (22)
		 0.50f,  0.50f,  0.50f, // [2]	// (23)
	};

	static const GLfloat colors[] = {

		// Front Face
		0.0f, 0.0f, 0.0f, 0.0f, // [0]	// ( 0)
		0.0f, 0.0f, 0.0f, 0.0f, // [1]	// ( 1)
		0.0f, 0.0f, 0.0f, 0.0f, // [2]	// ( 2)
		0.0f, 0.0f, 0.0f, 0.0f, // [3]	// ( 3)

		// Top Face
		0.0f, 0.0f, 0.0f, 0.0f, // [3]	// ( 4)
		0.0f, 0.0f, 0.0f, 0.0f, // [2]	// ( 5)
		0.0f, 0.0f, 0.0f, 0.0f, // [6]	// ( 6)
		0.0f, 0.0f, 0.0f, 0.0f, // [7]	// ( 7)

		// Back Face
		0.0f, 0.0f, 0.0f, 0.0f, // [5]	// ( 8)
		0.0f, 0.0f, 0.0f, 0.0f, // [4]	// ( 9)
		0.0f, 0.0f, 0.0f, 0.0f, // [7]	// (10)
		0.0f, 0.0f, 0.0f, 0.0f, // [6]	// (11)

		// Bottom Face
		0.0f, 0.0f, 0.0f, 0.0f, // [4]	// (12)
		0.0f, 0.0f, 0.0f, 0.0f, // [5]	// (13)
		0.0f, 0.0f, 0.0f, 0.0f, // [1]	// (14)
		0.0f, 0.0f, 0.0f, 0.0f, // [0]	// (15)

		// Left Face
		0.0f, 0.0f, 0.0f, 0.0f, // [4]	// (16)
		0.0f, 0.0f, 0.0f, 0.0f, // [0]	// (17)
		0.0f, 0.0f, 0.0f, 0.0f, // [3]	// (18)
		0.0f, 0.0f, 0.0f, 0.0f, // [7]	// (19)

		// Right Face
		0.0f, 0.0f, 0.0f, 0.0f, // [1]	// (20)
		0.0f, 0.0f, 0.0f, 0.0f, // [5]	// (21)
		0.0f, 0.0f, 0.0f, 0.0f, // [6]	// (22)
		0.0f, 0.0f, 0.0f, 0.0f, // [2]	// (23)
	};

	static GLfloat uvs[2 * 4 * 6] = {
		// Front Face (other faces populated in initialisation)
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0
	};

	static const GLuint indices[] =
	{
		// Front Face
		0, 1, 2,
		2, 3, 0,

		// Top Face
		4, 5, 6,
		6, 7, 4,

		// Back Face
		8, 9, 10,
		10, 11, 8,

		// Bottom Face
		12, 13, 14,
		14, 15, 12,

		// Left Face
		16, 17, 18,
		18, 19, 16,

		// Right Face
		20, 21, 22,
		22, 23, 20
	};

}
	class Cube
	{
	protected:
		GLfloat vertex[ARRAY_SIZE(CubeShape::vertices)];
		GLfloat color[ARRAY_SIZE(CubeShape::colors)];
		GLfloat uv[ARRAY_SIZE(CubeShape::uvs)];
		GLfloat index[ARRAY_SIZE(CubeShape::indices)];
	};
#endif

