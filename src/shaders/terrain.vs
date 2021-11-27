#version 330 core

layout (location = 0) in vec3 aPos;   // 位置变量的属性位置值为 0 
layout (location = 1) in vec3 aNormal; // 光照变量的属性位置值为 1
layout (location = 2) in vec3 aColor; // 颜色变量的属性位置值为 2

//flat为关键类型，使用单值代替插值，其中颜色和法向量都要用flat
flat out vec3 ourColor; // 向片段着色器输出一个颜色
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