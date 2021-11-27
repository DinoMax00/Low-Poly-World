#version 330 core
out vec4 FragColor;  

flat in vec3 ourColor;
flat in vec3 Normal;  
in vec3 FragPos;  
  
//������
uniform vec3 lightPos; 
uniform vec3 lightColor;
uniform vec2 lightBias;

//���淴��
uniform vec3 viewPos;

//simple diffuse lighting
vec3 calculateLighting() {
	vec3 normal = Normal * 2.0 - 1.0;//required just because of the format the normals were stored in (0 - 1)
	float brightness = max(dot(-lightPos, normal), 0.0);
	return (lightColor * lightBias.x) + (brightness * lightColor * lightBias.y);
}

void main()
{
    // //���²���Ϊ��������
    // float ambientStrength = 0.2;
    // vec3 ambient = ambientStrength * lightColor;
  	
    // //���²���Ϊ������
    // vec3 norm = normalize(Normal);
    // vec3 lightDir = normalize(lightPos - FragPos);
    // float diff = max(dot(norm, lightDir), 0.0);
    // vec3 diffuse = diff * lightColor;

    // //���²���Ϊ���淴��
    // float specularStrength = 0.5;
    // vec3 viewDir = normalize(viewPos - FragPos);
    // vec3 reflectDir = reflect(-lightDir, norm); 
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // vec3 specular = specularStrength * spec * lightColor;  
            
    // vec3 result = (ambient + diffuse + specular) * ourColor;
    vec3 result = ourColor * calculateLighting();
    FragColor = vec4(result, 1.0);
}