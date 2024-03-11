#ifndef IMAGE_H
#define IMAGE_H
#include <cstdint>
#include <string>
#include <optional>
#include <vector>

namespace renderer {
        class Color {
        static uint32_t from_rgb_norm(float r, float g, float b);
        static uint32_t from_rgb(uint8_t r, uint8_t g, uint8_t b);

        static uint32_t from_rgba_norm(float r, float g, float b, float a);
        static uint32_t from_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

        static float red_norm(uint32_t color);
        static uint8_t red(uint32_t color);

        static float green_norm(uint32_t color);
        static uint8_t green(uint32_t color);

        static float blue_norm(uint32_t color);
        static uint8_t blue(uint32_t color);

        static float alpha_norm(uint32_t color);
        static uint8_t alpha(uint32_t color);
    };

    class Image {
    public:
        explicit Image(uint32_t width, uint32_t height, uint32_t color = 0x000000FF);

        explicit Image(const std::string &path);

        bool is_in_bounds(uint32_t x, uint32_t y) const;

        void set_pixel(uint32_t x, uint32_t y, uint32_t color);

        void set_pixel_safe(uint32_t x, uint32_t y, uint32_t color);

        uint32_t get_pixel(uint32_t x, uint32_t y) const;

        void resize(uint32_t new_width, uint32_t new_height, uint32_t color = 0x000000FF);

        void clear(uint32_t color);

        uint32_t get_width() const { return m_width; }

        uint32_t get_height() const { return m_height; }

        std::optional <std::string_view> get_path() const { return m_path; }

        const uint32_t *raw() const { return m_data.data(); }

        const uint8_t *raw_as_bytes() const { return reinterpret_cast<const uint8_t *>(m_data.data()); }

    private:
        uint32_t m_width, m_height, m_bpp;
        std::vector <uint32_t> m_data;

        std::optional <std::string> m_path;
    };
}

#endif
