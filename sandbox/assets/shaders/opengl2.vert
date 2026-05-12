#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out vec2 vTexCoord;
out vec3 vNormal;
out vec3 vFragPos;
out vec3 vTangent;
out vec3 vBitangent;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    mat3 normalMatrix = mat3(transpose(inverse(u_model)));

    vec4 worldPos = u_model * vec4(aPosition, 1.0);
    vFragPos      = worldPos.xyz;
    vTexCoord     = aTexCoord;
    vNormal       = normalize(normalMatrix * aNormal);
    vTangent      = normalMatrix * aTangent;
    //vTangent = vec3(1.0, 0.0, 0.0);
    vBitangent    = normalMatrix * aBitangent;

    gl_Position = u_projection * u_view * worldPos;
}