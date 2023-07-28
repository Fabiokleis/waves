#version 450 core

layout (location = 0) in vec2 v_position;
layout (location = 1) in vec4 vert_color;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in float tex_idx;

out vec2 v_tex_coord;
out vec4 v_color;
out float v_tex_idx;

uniform mat4 u_camera;
uniform mat4 u_model;

void main()
{
    gl_Position = u_camera * u_model * vec4(v_position, 0.0, 1.0);
    v_color = vert_color;
    v_tex_coord = tex_coord;
    v_tex_idx = tex_idx;
}
