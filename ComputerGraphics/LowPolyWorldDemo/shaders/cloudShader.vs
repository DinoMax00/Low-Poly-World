#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
  
out vec3 FragPos;
flat out vec3 Normal;
flat out vec3 ourColor; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    ourColor = aColor;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}  