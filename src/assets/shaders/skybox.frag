#version 330 core

out vec4 out_color;

in vec2 f_tex_pos;
in float f_texture_id;

uniform sampler2DArray u_textures;

void main() {
    out_color = texture(u_textures, vec3(f_tex_pos, f_texture_id));
}
