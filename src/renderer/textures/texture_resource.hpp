#ifndef TEXTURE_RESOURCE_H
#define TEXTURE_RESOURCE_H
#include <string>
#include <GL/glew.h>
#include <memory>
#include "image.hpp"

namespace renderer {
    class TextureResource {
    public:
        TextureResource() = delete;
        static std::shared_ptr<TextureResource> from_path(const std::string& path, bool mipmap = true);
        static std::shared_ptr<TextureResource> from_img(const Image& img, bool mipmap = true);

        TextureResource(TextureResource&& other) noexcept;
        TextureResource(const TextureResource& other) = delete;
        TextureResource& operator=(TextureResource&& other) = delete;
        TextureResource& operator=(const TextureResource& other) = delete;

        ~TextureResource();

        void rebuild(const Image& img, bool mipmap = false);
        void update(const Image& img);
        void update(int x, int y, const Image& img);

        bool is_mipmap() const { return m_mipmap; }

        void bind(uint32_t unit) const;
        void unbind() const;

    private:
        explicit TextureResource(const std::string& path, bool mipmap = true);
        explicit TextureResource(const Image& img, bool mipmap = true);

    private:
        GLuint m_gl_id;
        int m_width;
        int m_height;
        int m_bpp;
        bool m_mipmap;
    };
}
#endif
