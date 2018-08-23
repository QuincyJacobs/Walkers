
#version 330 core
out vec4 FragColor;

in vec3 VertexColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

// assignment 3
//in vec4 VertexPositions;

void main()
{
    // with colors
    // FragColor = vec4(VertexColor, 1.0f);

    vec4 texture2vec = texture(texture2, vec2(TexCoord.x+1.0f, TexCoord.y));

    // with textures
    FragColor = mix(texture(texture1, TexCoord), vec4(texture2vec.x, texture2vec.y, texture2vec.z, texture2vec.w), 0.2); //texture(texture2, TexCoord); 

    // assignment 3
    //FragColor = VertexPositions;
}