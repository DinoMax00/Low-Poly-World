#version 330 core

layout (location = 0) in vec3 aPos;   // λ�ñ���������λ��ֵΪ 0 
layout (location = 1) in vec3 aNormal; // ���ձ���������λ��ֵΪ 1
layout (location = 2) in vec3 aColor; // ��ɫ����������λ��ֵΪ 2

//flatΪ�ؼ����ͣ�ʹ�õ�ֵ�����ֵ��������ɫ�ͷ�������Ҫ��flat
flat out vec3 ourColor; // ��Ƭ����ɫ�����һ����ɫ
out vec3 FragPos;
flat out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    FragPos=vec3(model * vec4(aPos, 1.0));
    Normal=aNormal;
    gl_Position =  projection * view  *vec4(FragPos, 1.0);
    ourColor = aColor;
}