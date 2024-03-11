#include "material.hpp"
renderer::Material::Material(const renderer::MaterialResource &mat_res)
: m_name(mat_res.name) {
    if (!mat_res.albedo_maps.empty())  {
        use_albedo_mapping = true;
    }

    if (!mat_res.specular_maps.empty())  {
        use_specular_mapping = true;
    }

    if (!mat_res.normal_maps.empty())  {
        use_normal_mapping = true;
    }

    for (auto& map : mat_res.albedo_maps) {
        m_albedo_maps.emplace_back(map);
    }

    for (auto& map : mat_res.specular_maps) {
        m_specular_maps.emplace_back(map);
    }

    for (auto& map : mat_res.normal_maps) {
        m_normal_maps.emplace_back(map);
    }
}

void renderer::Material::set_albedo(const glm::vec3 &albedo) {
    m_albedo = glm::clamp(albedo, 0.f, 1.f);
}

void renderer::Material::set_diffuse(const glm::vec3 &diffuse) {
    m_diffuse = glm::clamp(diffuse, 0.f, 1.f);
}

void renderer::Material::set_specular(const glm::vec3 &specular) {
    m_specular = glm::clamp(specular, 0.f, 1.f);
}

void renderer::Material::set_shininnes(float shininess) {
    m_shininess = glm::max(shininess, 1.f);
}

void renderer::Material::set_discard_threshold(float discard_threshold) {
    m_discard_threshold = glm::clamp(discard_threshold, 0.f, 1.f);
}

void renderer::Material::push_albedo_map(const std::shared_ptr<const TextureResource> &albedo_map) {
    m_albedo_maps.emplace_back(albedo_map);
}

void renderer::Material::push_specular_map(const std::shared_ptr<const TextureResource> &specular_map) {
    m_specular_maps.emplace_back(specular_map);
}

void renderer::Material::push_normal_map(const std::shared_ptr<const TextureResource> &normal_map) {
    m_normal_maps.emplace_back(normal_map);
}
