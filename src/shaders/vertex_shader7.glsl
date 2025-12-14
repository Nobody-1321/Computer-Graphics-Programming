#version 410 core
layout (location = 0) in vec3 aPos;

uniform vec4 objectColor;
uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;
uniform vec3 viewPos;
out vec4 ourColorV;

uniform vec3 lightPos; 
uniform vec4 lightColor;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    ourColorV = objectColor;
}
