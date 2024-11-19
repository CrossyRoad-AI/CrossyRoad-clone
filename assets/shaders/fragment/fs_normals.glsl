#version 420 core

in VertexData {
    vec3 v_color;
    vec3 v_normal;
    vec3 v_frag_position;
} vertexData;

layout(std140, binding = 1) uniform viewSettings_fs {
    vec3 viewPos;
};

layout(std140, binding = 2) uniform ambientLight_fs {
    vec3 ambient;
};

layout(location = 0) out vec4 color;

void main() {
    vec3 lightPos = vec3(0.0, 30.0, -40.0);
    vec3 lightColor = vec3(0.788, 0.886, 1.0);

    // Diffuse
    vec3 lightDir = normalize(lightPos - vertexData.v_frag_position);
    float diff = max(dot(vertexData.v_normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - vertexData.v_frag_position);
    vec3 reflectDir = reflect(-lightDir, vertexData.v_normal);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Final color
    color = vec4((ambient + diffuse + specular) * vertexData.v_color, 1.0);
}