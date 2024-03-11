#ifndef MESH_VERTEX_H
#define MESH_VERTEX_H
#include <glm/glm.hpp>

namespace renderer {
    struct Vertex {
        glm::vec3 position = glm::vec3(0.f);
        glm::vec3 tangent = glm::vec3(1.f, 0.f, 0.f);
        glm::vec3 bitangent = glm::vec3(0.f, 1.f, 0.f);
        glm::vec3 normal = glm::vec3(0.f, 0.f, 1.f);
        glm::vec2 tex_coords = glm::vec2(0.f);

        Vertex(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& tex_coords);
        Vertex(const glm::vec3& position, const glm::vec3& tangent, const glm::vec3& bitangent,
               const glm::vec3& normal, const glm::vec2& tex_coords);
    };
}

#endif
