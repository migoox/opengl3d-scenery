#ifndef MESH_RESOURCE_H
#define MESH_RESOURCE_H
#include <memory>
#include <vector>
#include <string>
#include "GL/glew.h"
#include "../mesh_vertex.hpp"

namespace renderer {
    class MeshInstance;
    class MeshArrayInstance;

    class MeshResource {
    public:
        static std::shared_ptr<MeshResource> create(
                const std::vector<Vertex>& vertices,
                const std::vector<uint32_t>& indices,
                const std::string& name = ""
        );

        MeshResource(MeshResource&& other) noexcept;

        MeshResource(const MeshResource& other) = delete;
        MeshResource& operator=(const MeshResource& other) = delete;
        MeshResource& operator=(MeshResource&& other) = delete;

        ~MeshResource();

        uint32_t get_count() const { return m_count; };

        const std::string& get_name() const { return m_name; }

        friend MeshInstance;
        friend MeshArrayInstance;

    protected:
        MeshResource();

        void bind() const;
        static void unbind() ;

        void set(
                const std::vector<Vertex>& vertices,
                const std::vector<uint32_t>& indices
        );

        void set(const std::vector<Vertex>& vertices);

    protected:
        bool m_tesselate = false;
        uint32_t m_tess_vert_per_patch = 0;

    private:
        void draw_gl() const;
        // void draw_instanced(const ShaderProgram& shader, size_t elems) const;

        MeshResource(
                const std::vector<Vertex>& vertices,
                const std::vector<uint32_t>& indices,
                const std::string& name
        );

        void init_gl();

    private:
        GLuint m_vao = 0, m_vbo = 0, m_ebo = 0;
        uint32_t m_count = 0;
        std::string m_name;
    };
}

#endif
