#version 330 core
out vec4 frag_color;

in vec3 tex_coords;

uniform samplerCube u_skybox;
uniform bool u_show_fog;
uniform vec3 u_fog_color;

void main() {
    if (u_show_fog) {
        frag_color = mix(texture(u_skybox, tex_coords), vec4(u_fog_color, 1.f), 0.92);
    } else {
        frag_color = texture(u_skybox, tex_coords);
    }
}