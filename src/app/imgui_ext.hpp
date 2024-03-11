#ifndef OPENGL_3D_SCENERY_IMGUI_EXT_H
#define OPENGL_3D_SCENERY_IMGUI_EXT_H
#include <glm/glm.hpp>
#include "../renderer/lighting.hpp"

namespace imgui_ext {
    class UniqueID {
    public:
        static std::string get();
        static void reset();
    private:
        static int current_id;

    };

    void spotlight_opts(renderer::Spotlight &spotlight);
    void begin_frame();
    void position_sliders(glm::vec3& pos);

    void point_light_opts(renderer::PointLight& point_light);
    void dir_light_opts(renderer::DirectionalLight& dir_light);


};


#endif
