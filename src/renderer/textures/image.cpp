#include "image.hpp"
#include "../../vendor/stbi_image.h"
#include <cmath>
#include <string>
#include <vector>
#include <iostream>

using namespace renderer;

uint32_t Color::from_rgb_norm(float r, float g, float b) {
    return static_cast<uint32_t>(std::round(r * 255.f)) << 24 |
           static_cast<uint32_t>(std::round(g * 255.f)) << 16 |
           static_cast<uint32_t>(std::round(b * 255.f)) << 8 |
           0xFF;
}

uint32_t Color::from_rgb(uint8_t r, uint8_t g, uint8_t b) {
    return static_cast<uint32_t>(r) << 24 |
           static_cast<uint32_t>(g) << 16 |
           static_cast<uint32_t>(b) << 8 |
           0xFF;
}

uint32_t Color::from_rgba_norm(float r, float g, float b, float a) {
    return static_cast<uint32_t>(std::round(r * 255.f)) << 24 |
           static_cast<uint32_t>(std::round(g * 255.f)) << 16 |
           static_cast<uint32_t>(std::round(b * 255.f)) << 8 |
           static_cast<uint32_t>(std::round(a * 255.f));
}

uint32_t Color::from_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return static_cast<uint32_t>(r) << 24 |
           static_cast<uint32_t>(g) << 16 |
           static_cast<uint32_t>(b) << 8 |
           static_cast<uint32_t>(a);
}

float Color::red_norm(uint32_t color) {
    // Extract the red component from the uint32_t color and normalize it to the range [0, 1]
    return static_cast<float>((color >> 24) & 0xFF) / 255.f;
}

uint8_t Color::red(uint32_t color) {
    // Extract and return the red component as an 8-bit unsigned integer
    return static_cast<uint8_t>((color >> 24) & 0xFF);
}

float Color::green_norm(uint32_t color) {
    // Extract the green component from the uint32_t color and normalize it to the range [0, 1]
    return static_cast<float>((color >> 16) & 0xFF) / 255.f;
}

uint8_t Color::green(uint32_t color) {
    // Extract and return the green component as an 8-bit unsigned integer
    return static_cast<uint8_t>((color >> 16) & 0xFF);
}

float Color::blue_norm(uint32_t color) {
    // Extract the blue component from the uint32_t color and normalize it to the range [0, 1]
    return static_cast<float>((color >> 8) & 0xFF) / 255.f;
}

uint8_t Color::blue(uint32_t color) {
    // Extract and return the blue component as an 8-bit unsigned integer
    return static_cast<uint8_t>((color >> 8) & 0xFF);
}

float Color::alpha_norm(uint32_t color) {
    // Extract the alpha component from the uint32_t color and normalize it to the range [0, 1]
    return static_cast<float>(color & 0xFF) / 255.f;
}

uint8_t Color::alpha(uint32_t color) {
    // Extract and return the alpha component as an 8-bit unsigned integer
    return static_cast<uint8_t>(color & 0xFF);
}


Image::Image(uint32_t width, uint32_t height, uint32_t color)
: m_width(width), m_height(height), m_bpp(4), m_path(std::nullopt) {
    m_data = std::vector<uint32_t>(m_width * m_height, color);
}

void swap_endianess(uint32_t *pixel) {
    *pixel = ((*pixel & 0xFF000000) >> 24) |
             ((*pixel & 0x00FF0000) >> 8)  |
             ((*pixel & 0x0000FF00) << 8)  |
             ((*pixel & 0x000000FF) << 24);
}

Image::Image(const std::string& path)
: m_path(path) {
    stbi_set_flip_vertically_on_load(1);
    int width, height, bpp;
    auto buff = reinterpret_cast<uint32_t*>(stbi_load(path.c_str(), &width, &height, &bpp, 4));

    for (int i = 0; i < width * height; ++i) {
        swap_endianess(&buff[i]);
    }
    m_width = width;
    m_height = height;
    m_bpp = bpp;

    if (!buff) {
        std::cerr << "[STBI]: " << "Couldn't load texture " << path << "\n";
        return;
    }

    m_data.insert(m_data.end(), &buff[0], &buff[m_width * m_height]);
    stbi_image_free(buff);
}

void Image::clear(uint32_t color) {
    for (auto& pixel : m_data) {
        pixel = color;
    }
}

void Image::set_pixel_safe(uint32_t x, uint32_t y, uint32_t color) {
    if (!is_in_bounds(x, y)) {
        return;
    }

    m_data[x + y * m_width] = color;
}

void Image::set_pixel(uint32_t x, uint32_t y, uint32_t color) {
    m_data[x + y * m_width] = color;
}

void Image::resize(uint32_t new_width, uint32_t new_height, uint32_t color) {
    m_data.resize(new_width * new_height, color);
    m_width = new_width;
    m_height = new_height;
}

bool Image::is_in_bounds(uint32_t x, uint32_t y) const {
    return x < m_width && y < m_height;
}

uint32_t Image::get_pixel(uint32_t x, uint32_t y) const {
    return m_data[x + y * m_width];
}