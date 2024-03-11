#include "tess_sphere.hpp"
#include "../renderer/gl_debug.h"

app::TessSphere::TessSphere() {
    GLCall( glGenVertexArrays(1, &m_vao) );
    GLCall( glBindVertexArray(m_vao) );

    GLCall( glGenBuffers(1, &m_vbo) );
    GLCall( glBindBuffer(GL_ARRAY_BUFFER, m_vbo) );

    float vertices[] = {
            -1.f, 0.f, 1.f, 0.f, 0.f,
            1.f, 0.f, 1.f, 1.f, 0.f,
            -1.f, 0.f, -1.f, 0.f, 1.f,
            1.f, 0.f, -1.f, 1.f, 1.f
    };

    GLCall( glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (void*)vertices, GL_STATIC_DRAW) );

    GLCall( glEnableVertexAttribArray(0) );
    GLCall( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)nullptr) );

    GLCall( glEnableVertexAttribArray(1) );
    GLCall( glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))) );

    GLCall( glPatchParameteri(GL_PATCH_VERTICES, 4) );

    m_sh_prog[static_cast<int>(m_type)].set_uniform_1i("u_tess_level", m_tess_level);
}

app::TessSphere::~TessSphere() {
    if (m_vao > 0) {
        GLCall( glDeleteVertexArrays(1, &m_vao) );
        m_vao = 0;
    }

    if (m_vbo > 0) {
        GLCall( glDeleteBuffers(1, &m_vbo) );
        m_vbo = 0;
    }

}

void app::TessSphere::draw() {
    if (!show) {
        return;
    }

    if (wireframe) {
        GLCall( glPolygonMode( GL_FRONT_AND_BACK, GL_LINE) );
    }

    m_sh_prog[static_cast<int>(m_type)].bind();
    m_sh_prog[static_cast<int>(m_type)].set_uniform_mat4f("u_model", this->get_world_model_mat());
    m_sh_prog[static_cast<int>(m_type)].set_uniform_mat3f("u_normal_mat", this->get_world_normal_mat());
    GLCall( glBindVertexArray(m_vao) );
    GLCall( glDrawArrays(GL_PATCHES, 0, 4) );

    if (wireframe) {
        GLCall( glPolygonMode( GL_FRONT_AND_BACK, GL_FILL) );
    }
}

void app::TessSphere::set_tess_level(int tess_level) {
    m_tess_level = tess_level;
    m_sh_prog[static_cast<int>(m_type)].set_uniform_1i("u_tess_level", tess_level);
}

void app::TessSphere::add_lights(const std::vector<renderer::Spotlight*> &spotlight,
                                 const std::vector<renderer::DirectionalLight*> &dir_light,
                                 const std::vector<renderer::PointLight*> &point_light) {

    int count = 0;
    for (auto& light : dir_light) {
        m_sh_prog[static_cast<int>(m_type)].set_uniform_3f("u_dir_lights[" + std::to_string(count) + "].dir", -light->get_world_front());
        m_sh_prog[static_cast<int>(m_type)].set_uniform_3f("u_dir_lights[" + std::to_string(count) + "].color", light->color);
        m_sh_prog[static_cast<int>(m_type)].set_uniform_3f("u_dir_lights[" + std::to_string(count) + "].ambient", light->ambient);
        count++;
    }
    m_sh_prog[static_cast<int>(m_type)].set_uniform_1i("u_dir_lights_len", count);

    count = 0;
    for (auto& light : spotlight) {
        m_sh_prog[static_cast<int>(m_type)].set_uniform_3f("u_spotlights[" + std::to_string(count) + "].pos", light->get_world_pos());
        m_sh_prog[static_cast<int>(m_type)].set_uniform_3f("u_spotlights[" + std::to_string(count) + "].dir", -light->get_world_front());
        m_sh_prog[static_cast<int>(m_type)].set_uniform_1f("u_spotlights[" + std::to_string(count) + "].inner_angle", light->inner_angle);
        m_sh_prog[static_cast<int>(m_type)].set_uniform_1f("u_spotlights[" + std::to_string(count) + "].outer_angle", light->outer_angle);
        m_sh_prog[static_cast<int>(m_type)].set_uniform_3f("u_spotlights[" + std::to_string(count) + "].color", light->color);
        m_sh_prog[static_cast<int>(m_type)].set_uniform_3f("u_spotlights[" + std::to_string(count) + "].ambient", light->ambient);
        count++;
    }
    m_sh_prog[static_cast<int>(m_type)].set_uniform_1i("u_spotlights_len", count);

    count = 0;
    int fin_count = 0;
    for (auto& light : point_light) {
        if (light->type == renderer::PointLightType::Infinite) {
            m_sh_prog[static_cast<int>(m_type)].set_uniform_3f("u_inf_point_lights[" + std::to_string(count) + "].pos", light->get_world_pos());
            m_sh_prog[static_cast<int>(m_type)].set_uniform_3f("u_inf_point_lights[" + std::to_string(count) + "].color", light->color);
            m_sh_prog[static_cast<int>(m_type)].set_uniform_3f("u_inf_point_lights[" + std::to_string(count) + "].ambient", light->ambient);
            count++;
        } else {
            m_sh_prog[static_cast<int>(m_type)].set_uniform_3f("u_fin_point_lights[" + std::to_string(fin_count) + "].pos", light->get_world_pos());
            m_sh_prog[static_cast<int>(m_type)].set_uniform_3f("u_fin_point_lights[" + std::to_string(fin_count) + "].color", light->color);
            m_sh_prog[static_cast<int>(m_type)].set_uniform_1f("u_fin_point_lights[" + std::to_string(fin_count) + "].quadratic", light->quadratic);
            m_sh_prog[static_cast<int>(m_type)].set_uniform_1f("u_fin_point_lights[" + std::to_string(fin_count) + "].linear", light->linear);
            m_sh_prog[static_cast<int>(m_type)].set_uniform_1f("u_fin_point_lights[" + std::to_string(fin_count) + "].radius", light->radius);
            m_sh_prog[static_cast<int>(m_type)].set_uniform_3f("u_fin_point_lights[" + std::to_string(fin_count) + "].ambient", light->ambient);
            fin_count++;
        }
    }

    m_sh_prog[static_cast<int>(m_type)].set_uniform_1i("u_inf_point_lights_len", count);
    m_sh_prog[static_cast<int>(m_type)].set_uniform_1i("u_fin_point_lights_len", fin_count);
}


void app::TessSphere::set_type(renderer::ShadingType type) {
    m_type = type;
    m_sh_prog[static_cast<int>(m_type)].set_uniform_1i("u_tess_level", m_tess_level);
}