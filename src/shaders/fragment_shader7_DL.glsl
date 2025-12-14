#version 410 core

out vec4 FragColor;

uniform vec3 lightPos; 
uniform vec4 lightColor;
uniform vec4 objectColor;

in vec3 normalV;   // Normal del fragmento
in vec3 fragPosV;    // Posición del fragmento

void main()
{
    // Fuerza de luz ambiental
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * lightColor.rgb;
  
    // Cálculo de luz difusa
    vec3 norm = normalize(normalV);  // Usar normalV en lugar de Normal
    vec3 lightDir = normalize(lightPos - fragPosV);  // Usar fragPos en lugar de FragPos
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor.rgb;  // Usar lightColor.rgb

    // Resultado final
    vec3 result = (ambient + diffuse) * objectColor.rgb;  // Usar objectColor.rgb
    FragColor = vec4(result, 1.0);
}
