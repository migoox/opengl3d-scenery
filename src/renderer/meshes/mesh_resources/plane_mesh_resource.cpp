#include "plane_mesh_resource.hpp"
#include "../../gl_debug.h"

using namespace renderer;
PlaneMeshResource::PlaneMeshResource() : MeshResource() {
    std::vector<Vertex> vertices = {
            Vertex(
                    glm::vec3(
                            0.f,
                            0.f,
                            0.f),
                    glm::vec3(1.f, 0.f, 0.f),
                    glm::vec3(0.f, 0.f, -1.f),
                    glm::vec3(0.f, 1.f, 0.f),
                    glm::vec2(
                            0.f,
                            0.f
                    )),
            Vertex(
                    glm::vec3(
                            0.f,
                            0.f,
                            1.f),
                    glm::vec3(1.f, 0.f, 0.f),
                    glm::vec3(0.f, 0.f, -1.f),
                    glm::vec3(0.f, 1.f, 0.f),
                    glm::vec2(
                            0.f,
                            1.f
                    )),
            Vertex(
                    glm::vec3(
                            1.f,
                            0.f,
                            1.f),
                    glm::vec3(1.f, 0.f, 0.f),
                    glm::vec3(0.f, 0.f, -1.f),
                    glm::vec3(0.f, 1.f, 0.f),
                    glm::vec2(
                            1.f,
                            1.f
                    )),
            Vertex(
                    glm::vec3(
                            1.f,
                            0.f,
                            0.f),
                    glm::vec3(1.f, 0.f, 0.f),
                    glm::vec3(0.f, 0.f, -1.f),
                    glm::vec3(0.f, 1.f, 0.f),
                    glm::vec2(
                            1.f,
                            0.f
                    )),
    };

    std::vector<uint32_t> indices = {
            0, 1, 2,
            2, 3, 0
    };

    this->set(vertices, indices);
}

PlaneMeshResource::PlaneMeshResource(uint32_t patches_x, uint32_t patches_y)
: MeshResource() {
    std::vector<Vertex> vertices;
    //std::vector<uint32_t> indices;

    float width = 1.f;
    float height = 1.f;

    for (uint32_t y = 0; y < patches_y - 1; ++y) {
        for (uint32_t x = 0; x < patches_x - 1; ++x) {
            vertices.emplace_back(
                    glm::vec3(
                            -width / 2.f + width * static_cast<float>(x) / static_cast<float>(patches_x),
                            0.f,
                            -height / 2.f + height * static_cast<float>(y) / static_cast<float>(patches_y)),
                    glm::vec3(1.f, 0.f, 0.f),
                    glm::vec3(0.f, 0.f, -1.f),
                    glm::vec3(0.f, 1.f, 0.f),
                    glm::vec2(
                            static_cast<float>(x) / static_cast<float>(patches_x),
                            static_cast<float>(y) / static_cast<float>(patches_y)
                    ));
            vertices.emplace_back(
                    glm::vec3(
                            -width / 2.f + width * static_cast<float>(x) / static_cast<float>(patches_x),
                            0.f,
                            -height / 2.f + height * static_cast<float>(y + 1) / static_cast<float>(patches_y)),
                    glm::vec3(1.f, 0.f, 0.f),
                    glm::vec3(0.f, 0.f, -1.f),
                    glm::vec3(0.f, 1.f, 0.f),
                    glm::vec2(
                            static_cast<float>(x) / static_cast<float>(patches_x),
                            static_cast<float>(y + 1) / static_cast<float>(patches_y)
                    ));
            vertices.emplace_back(
                    glm::vec3(
                            -width / 2.f + width * static_cast<float>(x + 1) / static_cast<float>(patches_x),
                            0.f,
                            -height / 2.f + height * static_cast<float>(y) / static_cast<float>(patches_y)),
                    glm::vec3(1.f, 0.f, 0.f),
                    glm::vec3(0.f, 0.f, -1.f),
                    glm::vec3(0.f, 1.f, 0.f),
                    glm::vec2(
                            static_cast<float>(x + 1) / static_cast<float>(patches_x),
                            static_cast<float>(y) / static_cast<float>(patches_y)
                    ));
            vertices.emplace_back(
                    glm::vec3(
                            -width / 2.f + width * static_cast<float>(x + 1) / static_cast<float>(patches_x),
                            0.f,
                            -height / 2.f + height * static_cast<float>(y + 1) / static_cast<float>(patches_y)),
                    glm::vec3(1.f, 0.f, 0.f),
                    glm::vec3(0.f, 0.f, -1.f),
                    glm::vec3(0.f, 1.f, 0.f),
                    glm::vec2(
                            static_cast<float>(x + 1) / static_cast<float>(patches_x),
                            static_cast<float>(y + 1) / static_cast<float>(patches_y)
                    ));
        }
    }

    m_tesselate = true;
    m_tess_vert_per_patch = 4;
    this->set(vertices);
}

std::shared_ptr<PlaneMeshResource> renderer::PlaneMeshResource::create() {
    auto *raw = new PlaneMeshResource();
    return std::shared_ptr<PlaneMeshResource>(raw);
}

std::shared_ptr<PlaneMeshResource> PlaneMeshResource::create_tesselable(uint32_t patches_x, uint32_t patches_y) {
    auto *raw = new PlaneMeshResource(patches_x, patches_y);
    return std::shared_ptr<PlaneMeshResource>(raw);
}