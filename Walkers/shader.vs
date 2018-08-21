
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

// assignment 2
//uniform float HorizontalOffset;

out vec3 VertexColor;
out vec2 TexCoord;

// assignment 3
//out vec4 VertexPositions;

void main()
{
	VertexColor = aColor;
	TexCoord = aTexCoord;
	
	// normal
	gl_Position = vec4(aPos, 1.0);

	// assignment 1
	//gl_Position = vec4(aPos.x, -aPos.y, aPos.z, 1.0);

	// assignment 2
	//gl_Position = vec4(aPos.x + HorizontalOffset.x, aPos.y, aPos.z, 1.0);

	// assignment 3
	//VertexPositions = gl_Position;
}