#ifndef OPENGL_3D_SCENERY_SKYBOX_H
#define OPENGL_3D_SCENERY_SKYBOX_H
#include <gl/glew.h>
#include <vector>
#include <string>
#include "../shaders/shader_program.hpp"

namespace renderer {
    class SkyBox {
    public:
        SkyBox(const std::vector<std::string>& faces_paths);
        ~SkyBox();

        void bind(uint32_t unit);
        void draw(const glm::mat4& proj, const glm::mat4& view, const glm::vec3& fog_color);
        void draw(const glm::mat4& proj, const glm::mat4& view);
    private:
        ShaderProgram m_sh_prog = ShaderProgram(ShaderResource("res/skybox.vert"), ShaderResource("res/skybox.frag"));

        GLuint m_txt_id = 0;
        GLuint m_vao = 0, m_vbo = 0;
    };
}

#endif
