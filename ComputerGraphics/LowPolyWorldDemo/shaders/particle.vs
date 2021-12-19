#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
flat out vec3 Normal;
flat out vec3 ourColor; 

uniform vec3 color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    ourColor = color;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}