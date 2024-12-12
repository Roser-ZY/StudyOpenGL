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
	FragPos = vec3(model * vec4(iPosition, 1.0));
	// �������ȱ����ŶԷ�������Ӱ��
	Normal = mat3(transpose(inverse(model))) * iNormal;
	gl_Position = projection * view * vec4(FragPos, 1.0);
}