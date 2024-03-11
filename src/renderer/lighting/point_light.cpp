#include "point_light.hpp"
#include "../shaders.hpp"

void renderer::PointLight::add_to_sh(renderer::IShaderObject &sh_obj) const {
    sh_obj.add_point_light(*this);
}