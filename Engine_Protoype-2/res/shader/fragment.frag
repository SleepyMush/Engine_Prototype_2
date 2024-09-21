#version 460 core
out vec4 FragColor;

struct Light {
    vec3 color;
    vec3 position;
    float radius;
};
uniform Light light[1];

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
in mat4 mView;

uniform vec3 viewPos; 
uniform vec3 objectColor;
uniform sampler2D textureSampler;

vec3 CalcPointLighting(Light light,vec3 viewDir);
float saturate(float x);

void main()
{ 
    vec3 viewDir = normalize(viewPos- FragPos);

    vec3 result = vec3(0.0);
    for (int i = 0; i < 1; i++) {
    result += CalcPointLighting(light[i], viewDir);
    }

   FragColor = vec4(result , 1.0) * texture(textureSampler, TexCoord);
}

float saturate(float x) {
    return clamp(x, 0.0, 1.0);
}

vec3 CalcPointLighting(Light light, vec3 viewDir)
{
    // ambient
    float ambientStrength = 1.1;
    vec3 ambient = ambientStrength * light.color;

    // diffuse 
    vec3 nLightPos = vec3(mView * vec4(light.position, 1.0));
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(nLightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.color;

    // specular
    float specularStrength = 0.5;
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * light.color; 
    
    float dist = length(light.position - FragPos);
    float attenuation = saturate(1.0 - dist / light.radius);

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}