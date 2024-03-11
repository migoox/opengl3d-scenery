#version 330 core
#define MAX_MAT_MAPS 4
#define MAX_LIGHTS 8

struct Material {
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
in vec3 normal;
in vec2 tex_coords;

out vec4 frag_color;

Material u_material;

uniform InfinitePointLight u_inf_point_lights[MAX_LIGHTS];
uniform int u_inf_point_lights_len;

uniform FinitePointLight u_fin_point_lights[MAX_LIGHTS];
uniform int u_fin_point_lights_len;

uniform DirectionalLight u_dir_lights[MAX_LIGHTS];
uniform int u_dir_lights_len;

uniform Spotlight u_spotlights[MAX_LIGHTS];
uniform int u_spotlights_len;

uniform vec3 u_camera_pos;

vec3 calc_inf_point_lights(vec3 obj_color, vec3 specular_factor, vec3 final_normal);
vec3 calc_fin_point_lights(vec3 obj_color, vec3 specular_factor, vec3 final_normal);
vec3 calc_dir_lights(vec3 obj_color, vec3 specular_factor, vec3 final_normal);
vec3 calc_spotlights(vec3 obj_color, vec3 specular_factor, vec3 final_normal);

void main() {
    u_material.albedo = vec3(135,206,235)/255.f;
    u_material.diffuse= vec3(0.8f);
    u_material.specular= vec3(0.9f);
    u_material.shininess = 128.f;

    // Find the object color
    vec3 obj_color = u_material.albedo;
    float obj_alpha = 1.f;

    // Find the final normal
    vec3 final_normal = normalize(normal);

    // Specular mapping
    vec3 specular_factor = vec3(1.f);

    // Lighting calculation
    vec3 res = calc_inf_point_lights(obj_color, specular_factor, final_normal);
    res += calc_fin_point_lights(obj_color, specular_factor, final_normal);
    res += calc_dir_lights(obj_color, specular_factor, final_normal);
    res += calc_spotlights(obj_color, specular_factor, final_normal);

    // Final fragment color
    frag_color = vec4(res, obj_alpha);
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