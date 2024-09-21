#version 460 core
layout (location = 0)in vec3 position;
layout (location = 1)in vec2 texturecoordinates;
layout (location = 2)in vec3 aNormal;

out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;
out mat4 mView;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main()
{
    FragPos = vec3(Model * vec4(position, 1.0));
    Normal = normalize(mat3(transpose(inverse(Model))) * aNormal);
    TexCoord = texturecoordinates;
    mView = View;
    gl_Position =  Projection * View * vec4(FragPos, 1.0);
}