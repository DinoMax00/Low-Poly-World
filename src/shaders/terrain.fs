#version 330 core
out vec4 FragColor;  

flat in vec3 ourColor;
flat in vec3 Normal;  
in vec3 FragPos;  
  
//Âþ·´Éä
uniform vec3 lightDirection; 
uniform vec3 lightColor;
uniform vec2 lightBias;

//¾µÃæ·´Éä
uniform vec3 viewPos;

//simple diffuse lighting
vec3 calculateLighting() {
	vec3 normal = Normal * 2.0 - 1.0;//required just because of the format the normals were stored in (0 - 1)
	float brightness = max(dot(-lightDirection, normal), 0.0);
	return (lightColor * lightBias.x) + (brightness * lightColor * lightBias.y);
}

void main()
{
    vec3 result = ourColor * calculateLighting();
    FragColor = vec4(result, 1.0);
}