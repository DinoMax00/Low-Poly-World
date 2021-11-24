#version 330 core
out vec4 FragColor;  

flat in vec3 ourColor;
flat in vec3 Normal;  
in vec3 FragPos;  
  
//漫反射
uniform vec3 lightPos; 
uniform vec3 lightColor;
//镜面反射
uniform vec3 viewPos;

void main()
{
    //以下部分为环境光照
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
  	
    //以下部分为漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //以下部分为镜面反射
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
            
    vec3 result = (ambient + diffuse + specular) * ourColor;
    FragColor = vec4(result, 1.0);
}