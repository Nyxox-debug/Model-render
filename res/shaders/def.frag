#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform bool hasTexture;
uniform vec3 fallbackColor;

void main()
{
    if (hasTexture)
        FragColor = texture(texture_diffuse1, TexCoords);
    else
        FragColor = vec4(fallbackColor, 1.0);
}
