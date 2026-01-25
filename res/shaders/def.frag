// #version 330 core
// out vec4 FragColor;
//
// void main()
// {
//     // Solid grey color
//     FragColor = vec4(0.6, 0.6, 0.6, 1.0);
// }

#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

struct Material {
    sampler2D texture_diffuse1;
};

uniform Material material;

void main()
{
    FragColor = texture(material.texture_diffuse1, TexCoords);
}
