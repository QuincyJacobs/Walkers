
#version 330 core
out vec4 FragColor;

in vec3 VertexColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float texMix;

// assignment 3
//in vec4 VertexPositions;

void main()
{
    // colors
    // FragColor = vec4(VertexColor, 1.0f);

    // modify texture for fun
    vec4 texture2vec = texture(texture2, vec2(-TexCoord.x, TexCoord.y));

    // with textures
    FragColor = mix(texture(texture1, TexCoord), vec4(texture2vec.x, texture2vec.y, texture2vec.z, texture2vec.w), texMix); 
}