#version 410 core

uniform vec4 u_color;

varying vec3 v_color;

layout(location = 0) out vec4 color;

void main() {
    float ambientStrength = 1.0; // 1

    color = vec4(ambientStrength * v_color, 1.0);
}