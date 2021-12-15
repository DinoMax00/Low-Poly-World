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
vec3 calculateLighting(){
	vec3 normal = in_normal.xyz * 2.0 - 1.0;//required just because of the format the normals were stored in (0 - 1)
	float brightness = max(dot(-lightDirection, normal), 0.0);
	return (lightColour * lightBias.x) + (brightness * lightColour * lightBias.y);
}

void main(void){

	vec4 worldPosition = vec4(in_position, 1.0);
	gl_ClipDistance[0] = dot(worldPosition, plane);
	gl_Position = projectionViewMatrix * worldPosition;
	
	vec3 lighting = calculateLighting();
	pass_colour = in_colour.rgb * lighting;

}