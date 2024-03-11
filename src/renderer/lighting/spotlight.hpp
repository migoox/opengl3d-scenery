#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H
#include "light.hpp"

namespace renderer {
    class Spotlight : public Light {
    public:
        void add_to_sh(IShaderObject& renderer) const override;
    public:
        float inner_angle = glm::radians(30.f);
        float outer_angle = glm::radians(40.f);
    };
}

#endif
