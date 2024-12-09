#version 330 core
layout (location = 0) in vec3 iPosition;
layout (location = 1) in vec3 iNormal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(iPosition, 1.0);
	FragPos = vec3(model * vec4(iPosition, 1.0));
	// 消除不等比缩放对法向量的影响
	Normal = mat3(transpose(inverse(model))) * iNormal;
}