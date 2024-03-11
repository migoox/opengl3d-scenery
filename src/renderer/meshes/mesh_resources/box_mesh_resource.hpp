#ifndef BOX_MESH_RESOURCE_H
#define BOX_MESH_RESOURCE_H
#include <memory>
#include "mesh_resource.hpp"

namespace renderer {
    class BoxMeshResource : public MeshResource {
    public:
        BoxMeshResource() = delete;
        static std::shared_ptr<BoxMeshResource> create();

    private:
        BoxMeshResource(float width, float height, float depth);

    private:
        float m_width, m_height, m_depth;
    };
}

#endif
