#ifndef OPENGL_3D_SCENERY_TESS_SPHERE_HPP
#define OPENGL_3D_SCENERY_TESS_SPHERE_HPP
#include "../renderer/shaders.hpp"
#include "../renderer/transform3d.hpp"
#include "../renderer/lighting.hpp"

namespace app {
    class TessSphere : public renderer::Transform3D {
    public:
        TessSphere();
        ~TessSphere();

        void draw();
        int get_tess_level() const { return m_tess_level; };
        void set_tess_level(int tess_level);

        renderer::ShaderProgram& get_sh() { return m_sh_prog[static_cast<int>(m_type)]; }

        void add_lights(const std::vector<renderer::Spotlight*>& spotlights,
                        const std::vector<renderer::DirectionalLight*>& dir_lights,
                        const std::vector<renderer::PointLight*>& point_lights
                        );

        bool wireframe = false;
        bool show = false;

        void set_type(renderer::ShadingType type);
    private:

        renderer::ShadingType m_type = renderer::ShadingType::Phong;
        int m_tess_level = 8;
        GLuint m_vao = 0, m_vbo = 0;

        renderer::ShaderProgram m_sh_prog[3] = {
                renderer::ShaderProgram(
                        renderer::ShaderResource("res/sphere_tess.vert"),
                        renderer::ShaderResource("res/sphere_tess.tesc"),
                        renderer::ShaderResource("res/sphere_tess.tese"),
                        renderer::ShaderResource("res/sphere_tess.frag")
                ),
                renderer::ShaderProgram(
                        renderer::ShaderResource("res/sphere_tess.vert"),
                        renderer::ShaderResource("res/sphere_tess.tesc"),
                        renderer::ShaderResource("res/gouraud_sphere_tess.tese"),
                        renderer::ShaderResource("res/gouraud_sphere_tess.frag")
                ),
                renderer::ShaderProgram(
                        renderer::ShaderResource("res/sphere_tess.vert"),
                        renderer::ShaderResource("res/sphere_tess.tesc"),
                        renderer::ShaderResource("res/const_sphere_tess.tese"),
                        renderer::ShaderResource("res/const_sphere_tess.frag")
                )
        };
    };
}

#endif
