#version 410 core

out vec4 FragColor;
in vec4 ourColorV;
in vec3 lightColorV;

void main()
{
    float ambientStrength = 0.4;
    vec3 ambient = ambientStrength * lightColorV;
    vec3 result = ambient * vec3(ourColorV);
    FragColor = vec4(result, 1.0);

}
