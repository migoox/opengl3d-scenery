#include "mesh_vertex.hpp"

using namespace renderer;

Vertex::Vertex(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &tex_coords)
: position(position), normal(normal), tex_coords(tex_coords) { }

Vertex::Vertex(const glm::vec3 &position, const glm::vec3 &tangent, const glm::vec3 &bitangent,
               const glm::vec3 &normal, const glm::vec2 &tex_coords)
        : position(position), tangent(tangent), bitangent(bitangent), normal(normal), tex_coords(tex_coords) { }