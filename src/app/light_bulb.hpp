#ifndef OPENGL_3D_SCENERY_LIGHT_BULB_H
#define OPENGL_3D_SCENERY_LIGHT_BULB_H
#include "../renderer/shaders.hpp"
#include "../renderer/meshes.hpp"

namespace app {
    class LightBulb {
    public:
        explicit LightBulb(const std::shared_ptr<renderer::SphereMeshResource>& sphere_res);
        bool show_sphere = true;
        void draw(renderer::IShaderObject& sh_obj);
        renderer::Transform3D& get_transform() { return mesh; }

        renderer::PointLight light;
        renderer::MeshInstance mesh;
    };
}

#endif
