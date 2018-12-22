#version 330 core

struct Material
{
	sampler2D diffuseTex;
	sampler2D specularTex;
	float shininess;
	float saturation;
	float brightness;
};

struct Light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 u_color;
uniform vec3 u_viewPosition;
uniform Material u_material;
uniform Light u_light;

in vec3 v_normal;  
in vec3 v_fragmentPosition;  
in vec2 v_texCoord;

out vec4 out_color;

void main()
{
	// Ambient
	vec3 ambient = u_light.ambient * vec3(texture(u_material.diffuseTex, v_texCoord));
	
    // Diffuse 
    vec3 norm = v_normal;
    vec3 lightDir = normalize(-u_light.direction);
    float diff = clamp(dot(norm, lightDir), 0.0, 1.0);
    vec3 diffuse = u_light.diffuse * diff * vec3(texture(u_material.diffuseTex, v_texCoord));
	
    // Specular
    vec3 viewDir = normalize(u_viewPosition - v_fragmentPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);
    vec3 specular = u_light.specular * spec * vec3(texture(u_material.specularTex, v_texCoord));
        
    vec3 result = (ambient + diffuse + specular)* u_color;
	
	const vec3 kLumCoeff = vec3(0.2125, 0.7154, 0.0721);
	vec3 intensity = vec3(dot(result.rgb, kLumCoeff));
	vec3 intensityRGB =  vec3(mix(intensity, result.rgb, u_material.saturation));
	result = intensityRGB * u_material.brightness;
	
    out_color = vec4(result, 1.0);
} 
