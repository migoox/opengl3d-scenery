#version 330 core
flat in vec2 tex_coords;
flat in vec3 camera_pos;
flat in int discard_frag;
flat in float discard_threshold;
flat in float obj_alpha;

flat in vec4 light_model_result;
in vec3 frag_pos;

out vec4 frag_color;

// Fog
uniform bool u_show_distance_fog;
uniform float u_view_range;
uniform vec3 u_fog_color;

void main() {
    // Discard the fragment if the conditions are met
    if (discard_frag == 1) {
        if (obj_alpha < discard_threshold) {
            discard;
        }
    }

    // Final fragment color
    frag_color = light_model_result;

    // Optional linear fog
    if (u_show_distance_fog) {
        float d = distance(frag_pos, camera_pos);

        float start_distance = u_view_range * 0.25f;
        float end_distance = u_view_range;
        float fog_factor = (end_distance - d) / (end_distance - start_distance);

        fog_factor = 1.f - clamp(fog_factor, 0.f, 1.f);
        frag_color = mix(frag_color, vec4(u_fog_color, 1.f), fog_factor);
    }
}
