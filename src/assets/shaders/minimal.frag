#version 330 core

out vec4 out_color;

in vec3  f_normal;
in vec3 f_world_pos;

uniform int is_stripple;
uniform int u_is_solid_color;
uniform vec4 u_solid_color;

void main() {
    if (is_stripple == 1){
        float stipplePattern = mod((f_world_pos.x + f_world_pos.y + f_world_pos.z) * 2.0, 1.0);
        if (stipplePattern > 0.5) discard;
    }
    out_color = u_is_solid_color > 0 ? u_solid_color : vec4(f_normal, 1.0);
}
