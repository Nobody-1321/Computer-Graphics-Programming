#version 450
layout(location=0) in vec3 position;
uniform mat4 mv_matrix;

void main(void) {
    gl_Position = mv_matrix * vec4(position, 1.0);
}