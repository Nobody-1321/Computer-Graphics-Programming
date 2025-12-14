#version 410 core

out vec4 FragColor;

uniform vec3 lightPos; 
uniform vec4 lightColor;
uniform vec4 objectColor;
uniform vec3 viewPos;

in vec3 normalV;   // Normal del fragmento
in vec3 fragPosV;    // Posición del fragmento

void main()
{
    // Fuerza de luz ambiental
    float ambientStrength = 0.8;
    vec3 ambient = ambientStrength * lightColor.rgb;
  
    // Cálculo de luz difusa
    vec3 norm = normalize(normalV);  // Usar normalV en lugar de Normal
    vec3 lightDir = normalize(lightPos - fragPosV);  // Usar fragPos en lugar de FragPos
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor.rgb;  // Usar lightColor.rgb

    //specular
    float specularStrength = 0.4;
    vec3 viewDir = normalize(viewPos - fragPosV);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor.rgb;

    // Resultado final
    vec3 result = (ambient + diffuse + specular) * objectColor.rgb;  // Usar objectColor.rgb
    FragColor = vec4(result, 1.0);
}
