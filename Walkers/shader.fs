
#version 330 core
out vec4 FragColor;

in vec3 VertexColor;

// assignment 3
//in vec4 VertexPositions;

void main()
{
    FragColor = vec4(VertexColor, 1.0f);

    // assignment 3
    //FragColor = VertexPositions;
}