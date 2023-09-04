#version 330 core

out vec4 out_color;

in vec2 f_tex_pos;

uniform sampler2D u_texture;

void main() {
    out_color = texture(u_texture, f_tex_pos);
}
