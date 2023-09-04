#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec2 v_tex_pos;
layout (location = 2) in float v_texture_id;

out vec2 f_tex_pos;
out float f_texture_id;

uniform mat4 u_vp, u_object;

void main() {
    vec4 world_pos = u_object * vec4(v_pos, 1.0);
    gl_Position = u_vp * world_pos;

    f_tex_pos = v_tex_pos;
    f_texture_id = v_texture_id;
}