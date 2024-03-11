#ifndef PLANE_MESH_RESOURCE_H
#define PLANE_MESH_RESOURCE_H
#include <memory>
#include <cstdint>
#include "mesh_resource.hpp"

namespace renderer {
    class PlaneMeshResource : public MeshResource {
    public:
        static std::shared_ptr<PlaneMeshResource> create();
        static std::shared_ptr<PlaneMeshResource> create_tesselable(uint32_t patches_x, uint32_t patches_y);
    private:

        PlaneMeshResource();
        PlaneMeshResource(uint32_t patches_x, uint32_t patches_y);
    };
}

#endif
