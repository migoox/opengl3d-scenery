#version 410 core

layout (vertices = 4) out;

in vec2 tex_coords[];
out vec2 tex_coords2[];

uniform int u_tess_level;

void main()
{
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    tex_coords2[gl_InvocationID] = tex_coords[gl_InvocationID];

    if (gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = 2.f * u_tess_level;
        gl_TessLevelOuter[1] = u_tess_level;
        gl_TessLevelOuter[2] = 2.f * u_tess_level;
        gl_TessLevelOuter[3] = u_tess_level;

        gl_TessLevelInner[0] = 2.f * u_tess_level;
        gl_TessLevelInner[1] = u_tess_level;
    }
}