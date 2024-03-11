#ifndef MATERIAL_RESOURCE_H
#define MATERIAL_RESOURCE_H
#include <string>
#include <vector>
#include <memory>
#include "../textures.hpp"

namespace renderer {
    class MaterialResource {
    public:
        std::string name;
        std::vector<std::shared_ptr<const TextureResource>> albedo_maps;
        std::vector<std::shared_ptr<const TextureResource>> specular_maps;
        std::vector<std::shared_ptr<const TextureResource>> normal_maps;
    };
}

#endif
