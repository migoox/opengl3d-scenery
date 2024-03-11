#version 330 core
layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_tangent;
layout (location = 2) in vec3 a_bitangent;
layout (location = 3) in vec3 a_normal;
layout (location = 4) in vec2 a_tex_coords;

uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;
uniform mat3 u_normal_mat;

uniform bool u_deform;
uniform float u_time;

out vec3 frag_pos;
out vec3 tangent;
out vec3 bitangent;
out vec3 normal;
out vec2 tex_coords;

void main() {
    tangent = normalize(u_normal_mat * normalize(a_tangent));
    bitangent = normalize(u_normal_mat * normalize(a_bitangent));
    normal = normalize(u_normal_mat * normalize(a_normal));

    tex_coords = a_tex_coords;

    if (u_deform) {
        vec3 pos = vec3(u_model * vec4(a_pos, 1.0));
        uint seed = 0x578437adU;
        float val = 0.2 * log(pow(sin(1.4*u_time + sqrt(abs(pos.x))) * (smoothstep(0.0, 1.0, 1.0 - a_tex_coords.y)), 2.0) + 1.0);
        pos.x += val;
        pos.y += val * 0.1;

        frag_pos = pos;
        gl_Position = u_proj * u_view * vec4(pos, 1.0);
    } else {
        frag_pos = vec3(u_model * vec4(a_pos, 1.0)); // World space

        gl_Position = u_proj * u_view * u_model * vec4(a_pos, 1.0);
    }
}
