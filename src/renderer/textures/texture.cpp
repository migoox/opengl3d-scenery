#include "texture.hpp"
#include "../gl_debug.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "../../vendor/stbi_image.h"
using namespace renderer;

Texture::Texture() : m_resource(std::nullopt) { }
Texture::Texture(const std::shared_ptr<const TextureResource>& resource) : m_resource(resource) { }

void Texture::bind(uint32_t unit) const {
    if (!m_resource.has_value()) {
        return;
    }

    m_resource->get()->bind(unit);

    if (this->filtering == TextureFiltering::Linear) {
        if (m_resource->get()->is_mipmap()) {
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        } else {
            GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
        }
        GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
    } else {
        if (m_resource->get()->is_mipmap()) {
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
        } else {
            GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST) );
        }
        GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST) );
    }

    if (this->wrapping == TextureWrapping::Repeat) {
        GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) );
        GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) );
    } else if (this->wrapping == TextureWrapping::ClampToEdge) {
        GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
        GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
    } else if (this->wrapping == TextureWrapping::ClampToBorder) {
        GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER) );
        GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER) );
    } else {
        GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT) );
        GLCall( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT) );
    }
}

void Texture::clear_res() {
    m_resource.reset();
}

bool Texture::has_res() const {
    return m_resource.has_value();
}

void Texture::set_res(const std::shared_ptr<const TextureResource>& resource) {
    m_resource.emplace(resource);
}

