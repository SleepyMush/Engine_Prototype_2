#version 460 core
layout (location = 0)in vec3 position;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

void main()
{
	gl_Position = Projection * View * Model * vec4(position.x, position.y, position.z, 1.0);
}
