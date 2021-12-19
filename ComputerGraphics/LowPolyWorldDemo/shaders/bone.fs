#version 330 core

in vec2 tex_cord;
in vec3 v_normal;
in vec3 v_pos;
in vec4 bw;
out vec4 color;

uniform sampler2D diff_texture;
uniform vec3 lightPos;
	
void main()
{
	vec3 lightDir = normalize(lightPos);
	float diff = max(dot(v_normal, lightDir), 0.6);
	vec3 dCol = 0.8 * texture(diff_texture, tex_cord).rgb; 
	color = vec4(dCol, 1);
}
