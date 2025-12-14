#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;
uniform vec3 viewPos;

out vec3 normalV;
out vec3 fragPosV;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    fragPosV = vec3(model * vec4(aPos, 1.0));
    
    mat4 normalMatrix = transpose(inverse(model));
    normalV = normalize(vec3(normalMatrix * vec4(aNormal, 0.0)));
   
    ///normalV = aNormal; //importante tranformar las normales a coordenadas del mundo
}
