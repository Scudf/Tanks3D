#version 330 core

uniform mat4 u_matModel;
uniform mat4 u_matView;
uniform mat4 u_matPersp;

layout (location = 0) in vec3 a_position;

void main()
{	
	gl_Position = u_matPersp * u_matView * u_matModel * vec4(a_position, 1.0);
}