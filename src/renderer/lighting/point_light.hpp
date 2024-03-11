#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H
#include "light.hpp"

namespace renderer {
    enum class PointLightType {
        Finite,
        Infinite
    };

    class PointLight : public Light {
    public:
        void add_to_sh(IShaderObject &renderer) const override;

    public:
        float radius = 2.f;
        float linear = 1.f;
        float quadratic = 1.f;
        PointLightType type = PointLightType::Finite;
    };
}

#endif
