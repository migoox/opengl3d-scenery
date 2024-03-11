#ifndef LIGHT_H
#define LIGHT_H
#include "../transform3d.hpp"

namespace renderer {
    class IShaderObject;

    class Light : public Transform3D {
    public:
        virtual void add_to_sh(IShaderObject& renderer) const = 0;

    public:
        glm::vec3 color = glm::vec3(1.f);
        glm::vec3 ambient = glm::vec3(0.1f);
    };}
#endif
