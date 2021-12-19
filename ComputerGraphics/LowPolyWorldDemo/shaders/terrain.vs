#version 330

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec4 in_normal;
layout(location = 2) in vec4 in_colour;

flat out vec3 pass_colour;

uniform vec3 lightDirection;
uniform vec3 lightColour;
uniform vec2 lightBias;

uniform mat4 projectionViewMatrix;

uniform vec4 plane;





//simple diffuse lighting
vec3 calculateLighting()
{
	vec3 normal = normalize(in_normal.xyz);
    vec3 ld = normalize(-lightDirection);
	float brightness = max(dot(ld, normal), 0.0);
	return (lightColour * lightBias.x) + (brightness * lightColour * lightBias.y);
}


out vec3 FragPos;
out vec3 Normal;
out vec3 lDir;
flat out vec3 amb;
flat out vec3 dif;


void main(void){

	vec4 worldPosition = vec4(3.0f * in_position, 1.0);
	gl_ClipDistance[0] = dot(worldPosition, plane);
	gl_Position = projectionViewMatrix * worldPosition;
	
	vec3 lighting = calculateLighting();

	FragPos = vec3(mat4(1.0f) * vec4(3.0f * in_position, 1.0));
	Normal = transpose(inverse(mat3(mat4(1.0f)))) * in_normal.xyz;
	lDir = lightDirection * 100.0f;

	pass_colour = in_colour.rgb * lighting;


    vec3 normalx = normalize(in_normal.xyz);
    vec3 ldx = normalize(-lightDirection);
	float brt = max(dot(ldx, normalx), 0.0);
    amb = (lightColour * lightBias.x) * in_colour.rgb;
    dif = (brt * lightColour * lightBias.y) * in_colour.rgb;
	//return (lightColour * lightBias.x) + (brightness * lightColour * lightBias.y);
}