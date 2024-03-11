#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include "texture_resource.hpp"
#include <cstdint>
#include <unordered_map>
#include <memory>
#include <string>
#include <GL/glew.h>
#include <optional>
#include <filesystem>
#include <vector>

namespace renderer {
    enum class TextureFiltering {
        Linear,
        Nearest
    };

    enum class TextureWrapping {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder,
    };

    class Texture {
    public:
        Texture();
        Texture(const std::shared_ptr<const TextureResource>& resouce);

        TextureFiltering filtering = TextureFiltering::Linear;
        TextureWrapping wrapping = TextureWrapping::Repeat;

        void clear_res();
        bool has_res() const;
        void set_res(const std::shared_ptr<const TextureResource>& resource);

        void bind(uint32_t unit) const;

    private:
        std::optional<std::shared_ptr<const TextureResource>> m_resource;
    };
}

#endif
