#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H
#include "light.hpp"

namespace renderer {
    class DirectionalLight : public Light {
    public:
        void add_to_sh(IShaderObject& renderer) const override;
    };
}

#endif
