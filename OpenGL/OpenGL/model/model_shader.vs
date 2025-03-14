#version 330 core
layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iNormal;
layout (location = 2) in vec2 iTexCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	FragPos = vec3(model * vec4(iPos, 1.0));
	// 消除不等比缩放对法向量的影响
	Normal = mat3(transpose(inverse(model))) * iNormal;
	TexCoords = iTexCoords;
	gl_Position = projection * view * vec4(FragPos, 1.0);
}