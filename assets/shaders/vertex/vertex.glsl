#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in mat4 viewMatrix;

uniform mat4 projection;
uniform mat4 view;

varying vec3 v_color;

void main() {
    gl_Position = projection * view * viewMatrix * vec4(position, 1.0);
    v_color = color;
}