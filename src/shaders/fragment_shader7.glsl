#version 410 core

uniform vec4 lightColor;

out vec4 FragColor;
in vec4 ourColorV;

void main()
{
    FragColor = vec4(ourColorV);
}
