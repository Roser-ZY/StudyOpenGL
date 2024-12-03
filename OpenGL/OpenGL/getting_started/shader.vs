#version 330 core
layout (location = 0) in vec3 aPos;             // λ�ñ���������λ��ֵΪ0
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;                              // ΪƬ����ɫ��ָ��һ����ɫ���
out vec2 texCoord;

uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColor = aColor;
    texCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
}