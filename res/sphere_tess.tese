// tessellation evaluation shader
#version 410 core
#define PI 3.14159265359

layout (quads, fractional_odd_spacing, ccw) in;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat3 u_normal_mat;

in vec2 tex_coords2[];

out vec3 frag_pos;
out vec2 tex_coords;
out vec3 normal;

void main() {

    // get patch coordinate
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // ----------------------------------------------------------------------
    // retrieve control point texture coordinates
    vec2 t00 = tex_coords2[0];
    vec2 t01 = tex_coords2[1];
    vec2 t10 = tex_coords2[2];
    vec2 t11 = tex_coords2[3];

    // bilinearly interpolate texture coordinate across patch
    vec2 t0 = (t01 - t00) * u + t00;
    vec2 t1 = (t11 - t10) * u + t10;
    vec2 texCoord = (t1 - t0) * v + t0;
    tex_coords = texCoord;

    float r = 1.f;
    vec4 p = vec4(
    r * sin(PI * tex_coords.y) * cos(2.f * PI * tex_coords.x),
    r * cos(PI * tex_coords.y),
    r * sin(PI * tex_coords.y) * sin(2.f * PI * tex_coords.x), 1.f);

    normal = normalize(u_normal_mat * normalize(vec3(p)));
    frag_pos = vec3(u_model * p);

    gl_Position = u_proj * u_view * u_model * p;
}