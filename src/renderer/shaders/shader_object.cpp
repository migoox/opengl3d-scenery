#include "shader_object.hpp"

void renderer::MaterialShaderObject::add_directional_light(const renderer::DirectionalLight &light) {
    m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_3f("u_dir_lights[" + std::to_string(m_dir_light_count) + "].dir", -light.get_world_front());
    m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_3f("u_dir_lights[" + std::to_string(m_dir_light_count) + "].color", light.color);
    m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_3f("u_dir_lights[" + std::to_string(m_dir_light_count) + "].ambient", light.ambient);

    m_dir_light_count++;
    m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_1i("u_dir_lights_len", m_dir_light_count);
}

void renderer::MaterialShaderObject::add_point_light(const renderer::PointLight &light) {
    if (light.type == PointLightType::Infinite) {
        m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_3f("u_inf_point_lights[" + std::to_string(m_inf_point_light_count) + "].pos", light.get_world_pos());
        m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_3f("u_inf_point_lights[" + std::to_string(m_inf_point_light_count) + "].color", light.color);
        m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_3f("u_inf_point_lights[" + std::to_string(m_inf_point_light_count) + "].ambient", light.ambient);

        m_inf_point_light_count++;
        m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_1i("u_inf_point_lights_len", m_inf_point_light_count);
    } else {
        m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_3f("u_fin_point_lights[" + std::to_string(m_fin_point_light_count) + "].pos", light.get_world_pos());
        m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_3f("u_fin_point_lights[" + std::to_string(m_fin_point_light_count) + "].color", light.color);
        m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_1f("u_fin_point_lights[" + std::to_string(m_fin_point_light_count) + "].quadratic", light.quadratic);
        m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_1f("u_fin_point_lights[" + std::to_string(m_fin_point_light_count) + "].linear", light.linear);
        m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_1f("u_fin_point_lights[" + std::to_string(m_fin_point_light_count) + "].radius", light.radius);
        m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_3f("u_fin_point_lights[" + std::to_string(m_fin_point_light_count) + "].ambient", light.ambient);

        m_fin_point_light_count++;
        m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_1i("u_fin_point_lights_len", m_fin_point_light_count);
    }
}

void renderer::MaterialShaderObject::add_spotlight(const renderer::Spotlight &light) {
    m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_3f("u_spotlights[" + std::to_string(m_spotlight_count) + "].pos", light.get_world_pos());
    m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_3f("u_spotlights[" + std::to_string(m_spotlight_count) + "].dir", -light.get_world_front());
    m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_1f("u_spotlights[" + std::to_string(m_spotlight_count) + "].inner_angle", light.inner_angle);
    m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_1f("u_spotlights[" + std::to_string(m_spotlight_count) + "].outer_angle", light.outer_angle);
    m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_3f("u_spotlights[" + std::to_string(m_spotlight_count) + "].color", light.color);
    m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_3f("u_spotlights[" + std::to_string(m_spotlight_count) + "].ambient", light.ambient);

    m_spotlight_count++;
    m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_1i("u_spotlights_len", m_spotlight_count);
}

void renderer::MaterialShaderObject::set_camera(const renderer::Camera &camera) {
    m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_mat4f("u_proj", camera.get_proj());
    m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_mat4f("u_view", camera.get_view());
    m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_3f("u_camera_pos", camera.get_pos());
}

void renderer::MaterialShaderObject::new_frame() {
    m_sh_prog[static_cast<int>(m_shading_type)].bind();
    m_spotlight_count = 0;
    m_dir_light_count = 0;
    m_inf_point_light_count = 0;
    m_fin_point_light_count = 0;
}

void set_material(renderer::ShaderProgram& prog, const renderer::Material& mat) {
    prog.set_uniform_1i("u_material.use_albedo_mapping", mat.use_albedo_mapping);
    prog.set_uniform_1i("u_material.use_specular_mapping", mat.use_specular_mapping);
    prog.set_uniform_1i("u_material.use_normal_mapping", mat.use_normal_mapping);
    prog.set_uniform_1i("u_material.add_normals", mat.add_normals);
    prog.set_uniform_1i("u_material.use_fragment_discarding", mat.use_fragment_discarding);

    prog.set_uniform_1f("u_material.discard_threshold", mat.get_discard_threshold());

    int i = 1;
    int albedo_id = 0, specular_id = 0, normal_id = 0;
    std::string text;
    for (auto& txt : mat.get_albedo_maps()) {
        text = "u_material.albedo_maps[" + std::to_string(albedo_id++) + "]";
        prog.set_uniform_1i(text.c_str(), i);
        txt.bind(i);
        i++;
    }
    for (auto& txt : mat.get_specular_maps()) {
        text = "u_material.specular_maps[" + std::to_string(specular_id++) + "]";
        prog.set_uniform_1i(text.c_str(), i);
        txt.bind(i);
        i++;
    }
    for (auto& txt : mat.get_normal_maps()) {
        text = "u_material.normal_maps[" + std::to_string(normal_id++) + "]";
        prog.set_uniform_1i(text.c_str(), i);
        txt.bind(i);
        i++;
    }

    prog.set_uniform_3f("u_material.albedo", mat.get_albedo());
    prog.set_uniform_3f("u_material.diffuse", mat.get_diffuse());
    prog.set_uniform_3f("u_material.specular", mat.get_specular());
    prog.set_uniform_1f("u_material.shininess", mat.get_shininnes());
}

void renderer::MaterialShaderObject::add_mesh(const renderer::MeshInstance &mesh) {
    m_sh_prog[static_cast<int>(m_shading_type)].bind();
    m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_mat4f("u_model", mesh.get_world_model_mat());
    m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_mat3f("u_normal_mat", mesh.get_world_normal_mat());
    set_material(m_sh_prog[static_cast<int>(m_shading_type)], mesh.get_material());
}

void renderer::MaterialShaderObject::add_drawable_obj(const IDrawable& obj, const Material& mat) {
    m_sh_prog[static_cast<int>(m_shading_type)].bind();
    m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_mat4f("u_model", obj.get_world_model_mat());
    m_sh_prog[static_cast<int>(m_shading_type)].set_uniform_mat3f("u_normal_mat", obj.get_world_normal_mat());
    set_material(m_sh_prog[static_cast<int>(m_shading_type)], mat);
}

void renderer::MaterialShaderObject::set_type(renderer::ShadingType type) {
    m_shading_type = type;
}

void renderer::IShaderObject::add_light(const renderer::Light &light) {
    light.add_to_sh(*this);
}
