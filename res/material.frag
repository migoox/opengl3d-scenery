#version 330 core
#define MAX_MAT_MAPS 4
#define MAX_LIGHTS 8

struct Material {
    bool use_albedo_mapping;
    bool use_specular_mapping;
    bool use_normal_mapping;
    bool add_normals;
    bool use_fragment_discarding;

    float discard_threshold;

    sampler2D albedo_maps[MAX_MAT_MAPS];
    sampler2D specular_maps[MAX_MAT_MAPS];
    sampler2D normal_maps[MAX_MAT_MAPS];

    vec3 albedo;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct InfinitePointLight {
    vec3 pos;
    vec3 color;
    vec3 ambient;
};

struct FinitePointLight {
    vec3 pos;
    vec3 color;
    float linear;
    float quadratic;
    float radius;
    vec3 ambient;
};

struct DirectionalLight {
    vec3 dir;
    vec3 color;
    vec3 ambient;
};

struct Spotlight {
    vec3 pos;
    vec3 color;
    vec3 dir;
    float inner_angle; // radians
    float outer_angle; // radians
    vec3 ambient;
};

in vec3 frag_pos;
in vec3 tangent;
in vec3 bitangent;
in vec3 normal;
in vec2 tex_coords;

out vec4 frag_color;

uniform Material u_material;

uniform InfinitePointLight u_inf_point_lights[MAX_LIGHTS];
uniform int u_inf_point_lights_len;

uniform FinitePointLight u_fin_point_lights[MAX_LIGHTS];
uniform int u_fin_point_lights_len;

uniform DirectionalLight u_dir_lights[MAX_LIGHTS];
uniform int u_dir_lights_len;

uniform Spotlight u_spotlights[MAX_LIGHTS];
uniform int u_spotlights_len;

uniform vec3 u_camera_pos;

// Fog
uniform bool u_show_distance_fog;
uniform float u_view_range;
uniform vec3 u_fog_color;

vec3 calc_inf_point_lights(vec3 obj_color, vec3 specular_factor, vec3 final_normal);
vec3 calc_fin_point_lights(vec3 obj_color, vec3 specular_factor, vec3 final_normal);
vec3 calc_dir_lights(vec3 obj_color, vec3 specular_factor, vec3 final_normal);
vec3 calc_spotlights(vec3 obj_color, vec3 specular_factor, vec3 final_normal);

void main() {
    // Find the object color
    vec3 obj_color;
    float obj_alpha = 1.f;
    if (u_material.use_albedo_mapping) {
        vec4 txt = texture(u_material.albedo_maps[0], tex_coords);
        obj_alpha = txt.w;
        obj_color = txt.xyz;
    } else {
        obj_color = u_material.albedo;
    }

    // Discard the fragment if the conditions are met
    if (u_material.use_fragment_discarding) {
        if (obj_alpha < u_material.discard_threshold) {
            discard;
        }
        obj_alpha = 1.0;
    }

    // Find the final normal
    vec3 final_normal;
    if (u_material.use_normal_mapping) {
        vec3 map_normal = 2.f * texture(u_material.normal_maps[0], tex_coords).xyz - 1.f;
        if (u_material.add_normals) {
            final_normal = normalize(normal + normalize(mat3(tangent, bitangent, normal) * map_normal));
        } else {
            final_normal = normalize(normalize(mat3(tangent, bitangent, normal) * map_normal));
        }
    } else {
        final_normal = normalize(normal);
    }

    // Specular mapping
    vec3 specular_factor;
    if (u_material.use_specular_mapping) {
        specular_factor = vec3(texture(u_material.specular_maps[0], tex_coords));
    } else {
        specular_factor = vec3(1.f);
    }

    // Lighting calculation
    vec3 res = calc_inf_point_lights(obj_color, specular_factor, final_normal);
    res += calc_fin_point_lights(obj_color, specular_factor, final_normal);
    res += calc_dir_lights(obj_color, specular_factor, final_normal);
    res += calc_spotlights(obj_color, specular_factor, final_normal);

    // Final fragment color
    frag_color = vec4(res, obj_alpha);

    // Optional linear fog
    if (u_show_distance_fog) {
        float d = distance(frag_pos, u_camera_pos);

        float start_distance = u_view_range * 0.25f;
        float end_distance = u_view_range;
        float fog_factor = (end_distance - d) / (end_distance - start_distance);

        fog_factor = 1.f - clamp(fog_factor, 0.f, 1.f);
        frag_color = mix(frag_color, vec4(u_fog_color, 1.f), fog_factor);
    }
}

vec3 calc_inf_point_lights(vec3 obj_color, vec3 specular_factor, vec3 final_normal) {
    vec3 ambient = vec3(0.f);
    vec3 diffuse = vec3(0.f);
    vec3 specular = vec3(0.f);
    vec3 light_pos;
    vec3 out_color = vec3(0.f);

    for (int i = 0; i < u_inf_point_lights_len; ++i) {
        light_pos = u_inf_point_lights[i].pos;

        ambient = u_inf_point_lights[i].ambient;
        diffuse = u_material.diffuse * clamp(dot(final_normal, normalize(light_pos - frag_pos)), 0.f, 1.f);
        specular = u_material.specular * pow(clamp(dot(normalize(u_camera_pos - frag_pos), reflect(normalize(frag_pos - light_pos), final_normal)), 0.f, 1.f), u_material.shininess);

        out_color += obj_color * (ambient + u_inf_point_lights[i].color * (diffuse + specular_factor * specular));
    }

    return out_color;
}

vec3 calc_fin_point_lights(vec3 obj_color, vec3 specular_factor, vec3 final_normal) {
    vec3 ambient = vec3(0.f);
    vec3 diffuse = vec3(0.f);
    vec3 specular = vec3(0.f);
    vec3 light_pos;
    float factor;
    vec3 out_color = vec3(0.f);

    for (int i = 0; i < u_fin_point_lights_len; ++i) {
        light_pos = u_fin_point_lights[i].pos;

        ambient = u_fin_point_lights[i].ambient;
        diffuse = u_material.diffuse * clamp(dot(final_normal, normalize(light_pos - frag_pos)), 0.f, 1.f);
        specular = u_material.specular * pow(clamp(dot(normalize(u_camera_pos - frag_pos), reflect(normalize(frag_pos - light_pos), final_normal)), 0.f, 1.f), u_material.shininess);

        float d = distance(light_pos, frag_pos) / u_fin_point_lights[i].radius;
        factor = 1.f / (1.f + u_fin_point_lights[i].linear * d + u_fin_point_lights[i].quadratic * d * d);

        out_color += factor * obj_color * (ambient +  u_fin_point_lights[i].color * (diffuse + specular_factor * specular));
    }

    return out_color;
}

vec3 calc_dir_lights(vec3 obj_color, vec3 specular_factor, vec3 final_normal) {
    vec3 ambient = vec3(0.f);
    vec3 diffuse = vec3(0.f);
    vec3 specular = vec3(0.f);
    vec3 out_color = vec3(0.f);

    for (int i = 0; i < u_dir_lights_len; ++i) {
        ambient = u_dir_lights[i].ambient;
        diffuse = u_material.diffuse * clamp(dot(final_normal, normalize(-u_dir_lights[i].dir)), 0.f, 1.f);
        specular = u_material.specular * pow(clamp(dot(normalize(u_camera_pos - frag_pos), reflect(normalize(u_dir_lights[i].dir), final_normal)), 0.f, 1.f), u_material.shininess);

        out_color += obj_color * (ambient + u_dir_lights[i].color * (diffuse + specular_factor * specular));
    }

    return out_color;
}

vec3 calc_spotlights(vec3 obj_color, vec3 specular_factor, vec3 final_normal) {
    vec3 ambient = vec3(0.f);
    vec3 diffuse = vec3(0.f);
    vec3 specular = vec3(0.f);
    vec3 out_color = vec3(0.f);

    for (int i = 0; i < u_spotlights_len; ++i) {
        vec3 light_dir = normalize(u_spotlights[i].pos - frag_pos);
        float theta = acos(dot(light_dir, normalize(-u_spotlights[i].dir)));
        float epsilon = u_spotlights[i].outer_angle - u_spotlights[i].inner_angle;
        float intensity = smoothstep(0.f, 1.f, (u_spotlights[i].outer_angle - theta) / epsilon);

        ambient = u_spotlights[i].ambient;
        diffuse = u_material.diffuse * clamp(dot(final_normal, normalize(light_dir)), 0.f, 1.f);
        specular = u_material.specular * pow(clamp(dot(normalize(u_camera_pos - frag_pos), reflect(normalize(-light_dir), final_normal)), 0.f, 1.f), u_material.shininess);

        out_color += obj_color * (ambient + intensity * u_spotlights[i].color * (diffuse + specular_factor * specular));
    }

    return out_color;
}