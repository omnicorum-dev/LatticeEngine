#version 330 core

layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_texCoord;

out vec2 v_uv;

uniform float u_gameAspect;
uniform float u_viewAspect;

void main() {
    vec2 scale = vec2(
            min(u_gameAspect / u_viewAspect, 1.0),
            min(u_viewAspect / u_gameAspect, 1.0)
    );

    gl_Position = vec4(a_position * scale, 0.0, 1.0);
    v_uv = a_texCoord;
}