#version 420 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in mat4 viewMatrix;

layout(std140, binding = 0) uniform Matrices {
    mat4 projection;
    mat4 view;
};

out vec3 v_color;
out vec3 v_normal;
out vec3 v_frag_position;

void main() {
    gl_Position = projection * view * viewMatrix * vec4(position, 1.0);

    v_color = color;
    v_normal = normal;
    v_frag_position = vec3(viewMatrix * vec4(position, 1.0));
}