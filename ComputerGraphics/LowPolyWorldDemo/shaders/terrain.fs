#version 330

flat in vec3 pass_colour;

out vec4 out_colour;

uniform bool useShadow;
uniform sampler2D shadowMap;
uniform mat4 lightSpaceMatrix;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Calculate bias (based on depth map resolution and slope)
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // Check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

in vec3 FragPos;
in vec3 Normal;
in vec3 lDir;
flat in vec3 amb;
flat in vec3 dif;
void main(void){

	if (useShadow)
	{
		vec4 FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);


		vec3 normal = normalize(Normal);
		vec3 lightDir = normalize(-lDir - FragPos);

		float shadow = ShadowCalculation(FragPosLightSpace, normal, lightDir);                      
		shadow = min(shadow, 0.75);
		out_colour = vec4(amb + (1.0f - shadow) * dif, 1.0);
	}
	else
	{
		out_colour = vec4(pass_colour, 1.0);
	}

}