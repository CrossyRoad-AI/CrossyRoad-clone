#version 420 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 normal;

layout (location = 3) in mat4 model;
layout (location = 7) in mat3 normalMatrix;

layout(std140, binding = 0) uniform viewSettings_vs {
    mat4 projection;
    mat4 view;
};

out VertexData {
    vec3 v_color;
    vec3 v_normal;
    vec3 v_frag_position;
} vertexData;

void main() {
    vec4 worldPosition = model * vec4(position, 1.0);
    gl_Position = projection * view * worldPosition;

    vertexData.v_color = color;
    vertexData.v_normal = normalize(normalMatrix * normal);
    vertexData.v_frag_position = vec3(worldPosition);
}