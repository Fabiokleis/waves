#version 450 core

layout(location = 0) out vec4 color;

in vec4 v_color;
in vec2 v_tex_coord;
in float v_tex_idx;

uniform vec4 u_color;
uniform sampler2D u_textures[32];
uniform float u_time;

void main()
{
    int idx = int(v_tex_idx);

    if (idx != 0) {
        vec4 tex_color = texture(u_textures[idx], v_tex_coord);

        color = tex_color;

    } else 
        color = v_color + sin(u_time)/2 - 0.2;

}