#include "directional_light.hpp"
#include "../shaders.hpp"

void renderer::DirectionalLight::add_to_sh(renderer::IShaderObject &sh_obj) const {
    sh_obj.add_directional_light(*this);
}