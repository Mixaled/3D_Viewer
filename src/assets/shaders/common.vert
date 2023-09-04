#version 330 core

layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_normal;

out vec3 f_normal;
out vec3 f_world_pos;

uniform float u_aspect_ratio;
uniform mat4 u_vp, u_object;

void main() {
    vec3 n = (u_object * vec4(v_normal, 0.0)).xyz;
    f_normal = length(n) <= 0.000001 ? n : (n / length(n));
    
    vec4 world_pos = u_object * vec4(v_pos, 1.0);
    f_world_pos = (world_pos / world_pos.w).xyz;
    
    vec4 pos = u_vp * world_pos;
    gl_Position = pos;
}