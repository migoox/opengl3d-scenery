#include "sphere_mesh_resource.hpp"
using namespace renderer;

SphereMeshResource::SphereMeshResource(int count)
: MeshResource() {
    constexpr double pi = 3.14159265358979323846;

    auto radius = 1.f;
    auto center = glm::vec3(0.f, 0.f, 0.f);

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    // find vertices
    for (int i = 0; i < count + 2; ++i) {
        // polar angle
        float u = (static_cast<float>(i)) / static_cast<float>(count);
        float u_angle = u * pi;

        for (int j = 0; j < count + 1; ++j) {
            // azim angle
            float v = static_cast<float>(j) / static_cast<float>(count);
            float v_angle = v * 2.f * pi;

            auto point_pos = center + glm::vec3(
                    radius * sin(u_angle) * cos(v_angle),
                    radius * cos(u_angle),
                    radius * sin(u_angle) * sin(v_angle)
            );

            // Find normal
            auto normal = glm::normalize((point_pos - center));

            // Find tangent and binormal
            auto tangent = glm::normalize(glm::vec3(
                    radius * sin(u_angle) * (-sin(v_angle)),
                    0.f,
                    radius * sin(u_angle) * cos(v_angle)
            ));


            auto binormal = glm::normalize(glm::vec3(
                    radius * cos(u_angle) * cos(v_angle),
                    radius * (-sin(u_angle)),
                    radius * cos(u_angle) * sin(v_angle)
            ));

            if (i == 0) {
                // fix tangent
                tangent = glm::normalize(glm::cross(normal, binormal));
            }
            vertices.emplace_back(point_pos, tangent, binormal, normal, glm::vec2(u, v));
        }
    }

    // find indices
    for (int i = 0; i < count + 1; ++i) {
        for (int j = 0; j < count; ++j) {
            indices.push_back(i * count + j);
            indices.push_back((i + 1) * count + j);
            indices.push_back((i + 1) * count + (j + 1));

            indices.push_back((i + 1) * count + (j + 1));
            indices.push_back(i * count + (j + 1));
            indices.push_back(i * count + j);
        }
    }

    this->set(vertices, indices);
}

std::shared_ptr<SphereMeshResource> SphereMeshResource::create(int count) {
    auto *res = new SphereMeshResource(count);
    return std::shared_ptr<SphereMeshResource>(res);
}