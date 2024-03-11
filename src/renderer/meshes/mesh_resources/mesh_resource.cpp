#include "mesh_resource.hpp"
#include "../../gl_debug.h"
using namespace renderer;

MeshResource::MeshResource(const std::vector<Vertex> &vertices,
                           const std::vector<uint32_t> &indices,
                           const std::string& name)
: m_name(name) {
    this->init_gl();
    this->set(vertices, indices);
}

MeshResource::MeshResource() {
    this->init_gl();
}

MeshResource::~MeshResource() {
    if (m_vao > 0) {
        GLCall( glDeleteVertexArrays(1, &m_vao) );
        m_vao = 0;
    }

    if (m_vbo > 0) {
        GLCall( glDeleteBuffers(1, &m_vbo) );
        m_vbo = 0;
    }

    if (m_ebo > 0) {
        GLCall( glDeleteBuffers(1, &m_ebo) );
        m_ebo = 0;
    }
}

void MeshResource::set(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices) {
    m_count = indices.size();

    GLCall( glBindVertexArray(m_vao) );

    GLCall( glBindBuffer(GL_ARRAY_BUFFER, m_vbo) );
    GLCall( glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW) );

    GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo) );
    GLCall( glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW) );

    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    // Vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    // Vertex texture coords
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // Vertex texture coords
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));

    GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
    GLCall( glBindBuffer(GL_ARRAY_BUFFER, 0) );
    GLCall( glBindVertexArray(0) );
}

void MeshResource::set(const std::vector<Vertex> &vertices) {
    m_count = vertices.size();

    GLCall( glBindVertexArray(m_vao) );

    GLCall( glBindBuffer(GL_ARRAY_BUFFER, m_vbo) );
    GLCall( glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW) );

    m_ebo = 0;

    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    // Vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    // Vertex texture coords
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    // Vertex texture coords
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));

    GLCall( glBindBuffer(GL_ARRAY_BUFFER, 0) );
    GLCall( glBindVertexArray(0) );
}

void MeshResource::init_gl() {
    GLCall( glGenVertexArrays(1, &m_vao) );
    GLCall( glGenBuffers(1, &m_vbo) );
    GLCall( glGenBuffers(1, &m_ebo) );
}

void MeshResource::bind() const {
    GLCall( glBindVertexArray(m_vao) );
    GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo) );
}

void MeshResource::unbind() {
    GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
    GLCall( glBindBuffer(GL_ARRAY_BUFFER, 0) );
    GLCall( glBindVertexArray(0) );
}

void MeshResource::draw_gl() const {
    this->bind();
    if (!m_tesselate) {
        if (m_ebo != 0) {
            GLCall( glDrawElements(GL_TRIANGLES, this->get_count(), GL_UNSIGNED_INT, nullptr) );
        } else {
            GLCall( glDrawArrays(GL_TRIANGLES, 0, this->get_count()) );
        }
    } else {
        GLCall( glPatchParameteri(GL_PATCH_VERTICES, m_tess_vert_per_patch) );
        GLCall( glDrawArrays(GL_PATCHES, 0, this->get_count()) );
    }
}

MeshResource::MeshResource(MeshResource &&other) noexcept
        : m_vao(other.m_vao), m_vbo(other.m_vbo), m_ebo(other.m_ebo), m_count(other.m_count) {
    other.m_vao = 0;
    other.m_vbo = 0;
    other.m_ebo = 0;
    other.m_count = 0;
}

std::shared_ptr<MeshResource>
MeshResource::create(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices, const std::string& name) {
    auto *raw = new MeshResource(vertices, indices, name);
    return std::shared_ptr<MeshResource>(raw);
}

