#version 330 core

out vec4 out_color;

in vec3  f_normal;
in vec3 f_world_pos;

uniform vec4 u_points_color;
uniform int u_is_circle;

void main() {
    if (u_is_circle > 0 && length(gl_PointCoord - 0.5) > 0.5)
        discard;

    out_color = u_points_color;
}
