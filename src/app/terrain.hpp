#ifndef OPENGL_3D_SCENERY_TERRAIN_H
#define OPENGL_3D_SCENERY_TERRAIN_H
#include "../renderer/meshes.hpp"

namespace app {
    class Terrain : public renderer::MeshArrayInstance {

    public:
        Terrain();
        void draw(renderer::IShaderObject& sh_obj) const override;
    };

}

#endif

