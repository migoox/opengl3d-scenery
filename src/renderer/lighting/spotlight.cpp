#include "spotlight.hpp"
#include "../shaders.hpp"

void renderer::Spotlight::add_to_sh(renderer::IShaderObject &sh_obj) const {
    sh_obj.add_spotlight(*this);
}
