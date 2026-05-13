#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec2 a_texCoord;

out vec2 v_uv;

uniform float u_sourceAspect;
uniform float u_targetAspect;

void main() {
    vec2 scale = vec2(
            min(u_sourceAspect / u_targetAspect, 1.0),
            min(u_targetAspect / u_sourceAspect, 1.0)
    );

    gl_Position = vec4(a_position.xy * scale, 0.0, 1.0);
    v_uv = a_texCoord;
}