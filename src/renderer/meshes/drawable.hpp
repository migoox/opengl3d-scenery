#ifndef DRAWABLE_H
#define DRAWABLE_H
#include "../transform3d.hpp"

namespace renderer {
    class IShaderObject;

    class IDrawable : public Transform3D {
    public:
        virtual void draw(IShaderObject& renderer) const = 0;
    };
}

#endif
