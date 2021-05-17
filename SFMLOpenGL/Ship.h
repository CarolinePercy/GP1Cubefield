#ifndef SHIP_H
#define SHIP_H

#include <gl/glew.h>

//Ship Vertices
/*
								 
								[1]
						(0.0f, 0.0f, -1.0f)
								 #
								/ \
							   / | \
							  /  |  \
							 /	 |	 \
							/	 |	  \
						   /	 |	   \
						  /		 |	    \
						 /		 |		 \
						/		 |	      \
					   /		 |		   \
					  /			 |		    \
					 /		   _/#\_		 \
					/		 _/	[3] \_		  \
				   /	(0.0f, 0.0f, 0.5f)     \
				  /		 _/			    \_	    \
				 /	   _/				  \_	 \
				/	 _/					    \_	  \
			   /   _/						  \_   \
			  /  _/							    \_  \
			 / _/								  \_ \
			/_/									    \_\
		   #-------------------------------------------#
		  [2]										  [0]
 (-1.0f, -1.0f, 1.0f)					      (1.0f, -1.0f, 1.0f)

*/
namespace ShipShape
{
	const int VERTICES = 9;	// Total Number of Vertices
	const int INDICES = 3;	// Total Number of Indexes
	const int UVS = 18;	// Total Number of UVs
	const int COLORS = 9;	// Total Number of Colors

	const float size1 = 0.25f;
	const float size2 = 0.12f;
	const float size3 = 0.15f;
	const float size4 = 0.15f;

	static const GLfloat vertices[] =
	{
		// Front Face
		-size1, -size4,  size1,	// [2]	// ( 0)
		 size1, -size4,  size1,	// [0]	// ( 1)
		 0.00f,  0.00f,  size2, // [3]	// ( 2)

		// Left Face
		-size1, -size4,  size1,	// [2]	// ( 3)
		 0.00f,  0.00f,  size2,	// [3]	// ( 4)
		 0.00f,  0.00f, -size3,	// [1]	// ( 5)

		 // Right Face
		 0.00f,  0.00f, -size3,	// [1]	// ( 6)
		 0.00f,  0.00f,  size2,	// [3]	// ( 7)
		 size1, -size4,  size1,	// [0]	// ( 8)
	     
	};

	static const GLfloat colors[] = {

		// Front Face
		-0.781f, -0.41f, 0.15f, 1.0f, // [2]	// (0)
		-0.781f, -0.41f, 0.15f, 1.0f, // [0]	// (1)
		-0.781f, -0.41f, 0.15f, 1.0f, // [3]	// (2)

		// Left Face
		0.6f, 0.6f, 0.6f, 1.0f, // [2]	// (3)
		0.6f, 0.6f, 0.6f, 1.0f, // [3]	// (4)
		0.6f, 0.6f, 0.6f, 1.0f, // [1]	// (5)

		// Right Face
		-0.58f, -0.2f, 0.35f, 1.0f, // [1]	// (6)
		-0.58f, -0.2f, 0.35f, 1.0f, // [3]	// (7)
		-0.58f, -0.2f, 0.35f, 1.0f, // [0]	// (8)
	};

	static GLfloat uvs[2 * 3 * 3] = {
		// Front Face (other faces populated in initialisation)
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0//,
		//0.0, 1.0
	};

	static const GLuint indices[] =
	{
		// Front Face
		0, 1, 2,

		// Left Face
		3, 4, 5,

		// Right Face
		6, 7, 8
	};

}
	class Ship
	{
	protected:
		GLfloat vertex[ARRAY_SIZE(ShipShape::vertices)];
		GLfloat color[ARRAY_SIZE(ShipShape::colors)];
		GLfloat uv[ARRAY_SIZE(ShipShape::uvs)];
		GLfloat index[ARRAY_SIZE(ShipShape::indices)];
	};
#endif
