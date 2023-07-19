#shader vertex
#version 450 core

layout (location = 0) in vec2 v_position;
layout (location = 1) in vec4 vert_color;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in float tex_idx;

out vec2 v_tex_coord;
out vec4 v_color;
out float v_tex_idx;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(v_position, 0.0, 1.0);
    v_color = vert_color;
    v_tex_coord = tex_coord;
    v_tex_idx = tex_idx;
};

#shader fragment
#version 450 core

layout(location = 0) out vec4 color;

in vec4 v_color;
in vec2 v_tex_coord;
in float v_tex_idx;

uniform vec4 u_color;
uniform sampler2D u_textures[3];

void main()
{
    int idx = int(v_tex_idx);

    if (idx != 0) {
        vec4 tex_color = texture(u_textures[idx], v_tex_coord);

        if (idx == 1)
           color = tex_color + u_color;
        else
           color = u_color;
    } else 
        color = v_color + u_color;   
};
