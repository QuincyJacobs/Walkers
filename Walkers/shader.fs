
#version 330 core
out vec4 FragColor;

in vec3 VertexColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

// assignment 3
//in vec4 VertexPositions;

void main()
{
    // with colors
    // FragColor = vec4(VertexColor, 1.0f);

    // with textures
    FragColor = texture(ourTexture, TexCoord);

    // assignment 3
    //FragColor = VertexPositions;
}