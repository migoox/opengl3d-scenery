#include "box_mesh_resource.hpp"

using namespace renderer;

BoxMeshResource::BoxMeshResource(float width, float height, float depth)
: MeshResource(), m_width(width), m_height(height), m_depth(depth) {
    // TODO: tex coord type (2 types)
    glm::vec3 positions[] {
            // front
            { 0.0, 0.0,  m_depth }, // bottom left,
            { m_width, 0.0,  m_depth }, // bottom right,
            { m_width,  m_height, m_depth }, // top right,
            { 0.0,  m_height,  m_depth }, // top left,
            // back
            { 0.0, 0.0, 0.0 }, // bottom left,
            { m_width, 0.0, 0.0 }, // bottom right,
            { m_width,  m_height, 0.0 }, // top right,
            { 0.0,  m_height, 0.0 } // top left,
    };

    glm::vec3 tangents[] = {
        { 1.0f, 0.0f, 0.0f },  // front face
        { -1.0f, 0.0f, 0.0f }, // back face
        { 0.0f, 0.0f, -1.0f }, // right face
        { 0.0f, 0.0f, 1.0f },  // left face
        { 1.0f, 0.0f, 0.0f },  // top face
        { 1.0f, 0.0f, 0.0f }   // bottom face
    };

    glm::vec3 bitangents[] = {
        { 0.0f, 1.0f, 0.0f },  // front face
        { 0.0f, 1.0f, 0.0f },  // back face
        { 0.0f, 1.0f, 0.0f },  // right face
        { 0.0f, 1.0f, 0.0f },  // left face
        { 0.0f, 0.0f, -1.0f }, // top face
        { 0.0f, 0.0f, 1.0f }   // bottom face
    };

    glm::vec3 normals[] = {
        { 0.0f, 0.0f, 1.0f },  // front face
        { 0.0f, 0.0f, -1.0f }, // back face
        { 1.0f, 0.0f, 0.0f },  // right face
        { -1.0f, 0.0f, 0.0f }, // left face
        { 0.0f, 1.0f, 0.0f },  // top face
        { 0.0f, -1.0f, 0.0f }  // bottom face
    };

#define TBN(i) tangents[i], bitangents[i], normals[i]

    std::vector<Vertex> vertices = {
            Vertex(positions[0], TBN(0), glm::vec2(0.f, 0.f)), // 0
            Vertex(positions[0], TBN(5), glm::vec2(0.f, 1.f)), // 1
            Vertex(positions[0], TBN(3), glm::vec2(1.f, 0.f)), // 2

            Vertex(positions[1], TBN(0), glm::vec2(1.f, 0.f)), // 3
            Vertex(positions[1], TBN(5), glm::vec2(1.f, 1.f)), // 4
            Vertex(positions[1], TBN(2), glm::vec2(0.f, 0.f)), // 5

            Vertex(positions[2], TBN(0), glm::vec2(1.f, 1.f)), // 6
            Vertex(positions[2], TBN(4), glm::vec2(1.f, 0.f)), // 7
            Vertex(positions[2], TBN(2), glm::vec2(0.f, 1.f)), // 8

            Vertex(positions[3], TBN(0), glm::vec2(0.f, 1.f)), // 9
            Vertex(positions[3], TBN(4), glm::vec2(0.f, 0.f)), // 10
            Vertex(positions[3], TBN(3), glm::vec2(1.f, 1.f)), // 11


            Vertex(positions[4], TBN(1), glm::vec2(1.f, 0.f)), // 12
            Vertex(positions[4], TBN(5), glm::vec2(0.f, 0.f)), // 13
            Vertex(positions[4], TBN(3), glm::vec2(0.f, 0.f)), // 14

            Vertex(positions[5], TBN(1), glm::vec2(0.f, 0.f)), // 15
            Vertex(positions[5], TBN(5), glm::vec2(1.f, 0.f)), // 16
            Vertex(positions[5], TBN(2), glm::vec2(1.f, 0.f)), // 17

            Vertex(positions[6], TBN(1), glm::vec2(0.f, 1.f)), // 18
            Vertex(positions[6], TBN(4), glm::vec2(1.f, 1.f)), // 19
            Vertex(positions[6], TBN(2), glm::vec2(1.f, 1.f)), // 20

            Vertex(positions[7], TBN(1), glm::vec2(1.f, 1.f)), // 21
            Vertex(positions[7], TBN(4), glm::vec2(0.f, 1.f)), // 22
            Vertex(positions[7], TBN(3), glm::vec2(0.f, 1.f))  // 23
    };

    std::vector<uint32_t> indices {
        // front face
        0, 3, 6,
        6, 9, 0,
        // back face
        15, 12, 21,
        21, 18, 15,
        // right face
        5, 17, 20,
        20, 8, 5,
        // left face
        14, 2, 11,
        11, 23, 14,
        // top face
        10, 7, 19,
        19, 22, 10,
        // bottom face
        13, 16, 4,
        4, 1, 13
    };

    this->set(vertices, indices);
}

std::shared_ptr<BoxMeshResource> BoxMeshResource::create() {
    auto *raw = new BoxMeshResource(1.f, 1.f, 1.f);
    return std::shared_ptr<BoxMeshResource>(raw);
}
