#version 330 core

out vec4 color;

in vec2 v_uv;

uniform sampler2D u_buffer;

void main() {
    color = texture(u_buffer, v_uv);
}