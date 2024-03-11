#version 330 core
#define MAX_MAT_MAPS 4
#define MAX_LIGHTS 8

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

flat out int discard_frag;
flat out float discard_threshold;
flat out vec2 tex_coords;
flat out vec4 light_model_result;
out vec3 frag_pos;
flat out vec3 camera_pos;
flat out float obj_alpha;

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

vec3 calc_inf_point_lights(vec3 obj_color, vec3 specular_factor, vec3 final_normal, vec3 frag_pos);
vec3 calc_fin_point_lights(vec3 obj_color, vec3 specular_factor, vec3 final_normal, vec3 frag_pos);
vec3 calc_dir_lights(vec3 obj_color, vec3 specular_factor, vec3 final_normal, vec3 frag_pos);
vec3 calc_spotlights(vec3 obj_color, vec3 specular_factor, vec3 final_normal, vec3 frag_pos);

void main() {
    tex_coords = a_tex_coords;
    discard_frag = u_material.use_fragment_discarding ? 1 : 0;
    discard_threshold = u_material.discard_threshold;
    camera_pos = u_camera_pos;
    vec3 tangent = normalize(u_normal_mat * normalize(a_tangent));
    vec3 bitangent = normalize(u_normal_mat * normalize(a_bitangent));
    vec3 normal = normalize(u_normal_mat * normalize(a_normal));

    // Find the object color
    vec3 obj_color;
    obj_alpha = 1.f;
    if (u_material.use_albedo_mapping) {
        vec4 txt = texture(u_material.albedo_maps[0], tex_coords);
        obj_alpha = txt.w;
        obj_color = txt.xyz;
    } else {
        obj_color = u_material.albedo;
    }


    tex_coords = a_tex_coords;

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

    // Deforming
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

    // Lighting calculation
    vec3 res = calc_inf_point_lights(obj_color, specular_factor, final_normal, frag_pos);
    res += calc_fin_point_lights(obj_color, specular_factor, final_normal, frag_pos);
    res += calc_dir_lights(obj_color, specular_factor, final_normal, frag_pos);
    res += calc_spotlights(obj_color, specular_factor, final_normal, frag_pos);

    light_model_result = vec4(res, obj_alpha);
}


vec3 calc_inf_point_lights(vec3 obj_color, vec3 specular_factor, vec3 final_normal, vec3 frag_pos) {
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

vec3 calc_fin_point_lights(vec3 obj_color, vec3 specular_factor, vec3 final_normal, vec3 frag_pos) {
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

vec3 calc_dir_lights(vec3 obj_color, vec3 specular_factor, vec3 final_normal, vec3 frag_pos) {
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

vec3 calc_spotlights(vec3 obj_color, vec3 specular_factor, vec3 final_normal, vec3 frag_pos) {
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