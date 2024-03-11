#ifndef SPHERE_MESH_RESOURCE_H
#define SPHERE_MESH_RESOURCE_H
#include "mesh_resource.hpp"
#include <vector>

namespace renderer {
    class SphereMeshResource : public MeshResource {
    public:
        SphereMeshResource() = delete;
        static std::shared_ptr<SphereMeshResource> create(int count);

    private:
        explicit SphereMeshResource(int count);

    private:
        float m_width, m_height, m_depth;
    };
}

#endif
