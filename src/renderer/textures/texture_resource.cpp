#include "texture_resource.hpp"
#include "../gl_debug.h"
#include "../../vendor/stbi_image.h"

using namespace renderer;


std::shared_ptr<TextureResource> TextureResource::from_path(const std::string& path, bool mipmap) {
    auto *raw = new TextureResource(path, mipmap);
    return std::shared_ptr<TextureResource>(raw);
}

std::shared_ptr<TextureResource> TextureResource::from_img(const Image& img, bool mipmap) {
    auto *raw = new TextureResource(img, mipmap);
    return std::shared_ptr<TextureResource>(raw);
}

TextureResource::TextureResource(const std::string& path, bool mipmap)
: m_gl_id(0), m_width(0), m_height(0), m_bpp(0), m_mipmap(mipmap) {
    stbi_set_flip_vertically_on_load(1);
    auto buff = stbi_load(path.c_str(), &m_width, &m_height, &m_bpp, 4);

    if (!buff) {
        std::cerr << "[STBI]: " << "Couldn't load texture " << path << "\n";
        return;
    }

    GLCall( glGenTextures(1, &m_gl_id) );
    GLCall( glBindTexture(GL_TEXTURE_2D, m_gl_id) );

    GLCall( glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA8,
            m_width,
            m_height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            buff
    ) );

    if (mipmap) {
        GLCall( glGenerateMipmap(GL_TEXTURE_2D) );
    }

    stbi_image_free(buff);
}

TextureResource::TextureResource(const Image &img, bool mipmap)
: m_gl_id(0), m_width(img.get_width()), m_height(img.get_height()), m_bpp(4), m_mipmap(mipmap) {

    GLCall( glGenTextures(1, &m_gl_id) );
    GLCall( glBindTexture(GL_TEXTURE_2D, m_gl_id) );

    GLCall( glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            m_width,
            m_height,
            0,
            GL_RGBA,
            GL_UNSIGNED_INT_8_8_8_8,
            img.raw()
    ) );

    if (mipmap) {
        GLCall( glGenerateMipmap(GL_TEXTURE_2D) );
    }
}

TextureResource::~TextureResource() {
    if (m_gl_id > 0) {
        GLCall( glDeleteTextures(1, &m_gl_id) );
    }
}

void TextureResource::bind(uint32_t unit) const {
    GLCall( glActiveTexture(GL_TEXTURE0 + unit) );
    GLCall( glBindTexture(GL_TEXTURE_2D, m_gl_id) );
}

void TextureResource::unbind() const {
    GLCall( glBindTexture(GL_TEXTURE_2D, 0) );
}

TextureResource::TextureResource(TextureResource &&other) noexcept
: m_gl_id(other.m_gl_id), m_width(other.m_width), m_height(other.m_height),
m_bpp(other.m_bpp), m_mipmap(other.m_mipmap) {
    other.m_gl_id = 0;
}

void TextureResource::rebuild(const Image &img, bool mipmap) {
    m_width = img.get_width();
    m_height = img.get_height();
    m_mipmap = mipmap;
    GLCall( glBindTexture(GL_TEXTURE_2D, m_gl_id) );
    GLCall( glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            m_width,
            m_height,
            0,
            GL_RGBA,
            GL_UNSIGNED_INT_8_8_8_8,
            img.raw()
    ) );

    if (mipmap) {
        GLCall( glGenerateMipmap(GL_TEXTURE_2D) );
    }
}

void TextureResource::update(const Image &img) {
    GLCall( glBindTexture(GL_TEXTURE_2D, m_gl_id) );
    GLCall( glTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            0,
            0,
            img.get_width(),
            img.get_height(),
            GL_RGBA,
            GL_UNSIGNED_INT_8_8_8_8,
            img.raw()
    ) );
}

void TextureResource::update(int x, int y, const Image &img) {
    GLCall( glBindTexture(GL_TEXTURE_2D, m_gl_id) );
    GLCall( glTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            x,
            y,
            img.get_width(),
            img.get_height(),
            GL_RGBA,
            GL_UNSIGNED_INT_8_8_8_8,
            img.raw()
    ) );
}
