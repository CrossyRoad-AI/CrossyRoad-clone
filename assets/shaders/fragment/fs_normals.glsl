#version 420 core

in vec3 v_color;
in vec3 v_normal;
in vec3 v_frag_position;

layout(location = 0) out vec4 color;

void main() {
    vec3 lightPos = vec3(-30.0, 60.0, v_frag_position[2] + 10.0);
    vec3 lightColor = vec3(0.788, 0.886, 1.0);

    float ambientStrength = 0.6; // 0.1
    vec3 ambient = ambientStrength * lightColor;

    vec3 lightDir = normalize(lightPos - v_frag_position);
    float diff = max(dot(v_normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * 0.7;

    color = vec4((ambient + diffuse) * v_color, 1.0);
}