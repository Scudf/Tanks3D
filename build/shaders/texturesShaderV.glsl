#version 330 core

uniform mat4 u_matModel;
uniform mat4 u_matView;
uniform mat4 u_matPersp;

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_texCoord;

out vec3 v_normal;
out vec3 v_fragmentPosition;
out vec2 v_texCoord;

void main()
{	
	gl_Position = u_matPersp * u_matView * u_matModel * vec4(a_position, 1.0);
	v_fragmentPosition = vec3(u_matModel * vec4(a_position, 1.0));
	v_normal = normalize(mat3(transpose(inverse(u_matModel))) * a_normal);
	v_texCoord = vec2(a_texCoord.x, a_texCoord.y);
}