#include "skybox.hpp"
#include "../gl_debug.h"
#include "../../vendor/stbi_image.h"
#include <iostream>

renderer::SkyBox::SkyBox(const std::vector<std::string> &faces_paths) {
    GLCall( glGenTextures(1, &m_txt_id) );
    GLCall( glBindTexture(GL_TEXTURE_CUBE_MAP, m_txt_id) );

    int width, height, nr_channels;
    for (unsigned int i = 0; i < faces_paths.size(); i++)
    {
        unsigned char *data = stbi_load(faces_paths[i].c_str(), &width, &height, &nr_channels, 0);
        if (data)
        {
            GLCall( glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            ));
            stbi_image_free(data);
        }
        else
        {
            std::cerr << "[Cubemap]: Tex failed to load at path: " << faces_paths[i] << std::endl;
            stbi_image_free(data);
        }
    }

    GLCall( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR) );
    GLCall( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
    GLCall( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE) );
    GLCall( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE) );
    GLCall( glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE) );

    float vertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };
    GLCall( glGenVertexArrays(1, &m_vao) );
    GLCall( glGenBuffers(1, &m_vbo) );

    GLCall( glBindVertexArray(m_vao) );

    GLCall( glBindBuffer(GL_ARRAY_BUFFER, m_vbo) );
    GLCall( glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW) );

    // Vertex positions
    GLCall( glEnableVertexAttribArray(0) );
    GLCall( glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr));
}

renderer::SkyBox::~SkyBox() {
    if (m_txt_id > 0) {
        GLCall( glDeleteTextures(1, &m_txt_id) );
        m_txt_id = 0;
    }
    if (m_vao > 0) {
        GLCall( glDeleteTextures(1, &m_vao) );
        m_vao = 0;
    }
    if (m_vbo > 0) {
        GLCall( glDeleteTextures(1, &m_vbo) );
        m_vbo = 0;
    }
}

void renderer::SkyBox::bind(uint32_t unit) {
    m_sh_prog.bind();
    GLCall( glBindVertexArray(m_vao) );
    GLCall( glBindTexture(m_txt_id, unit) );
}

void renderer::SkyBox::draw(const glm::mat4 &proj, const glm::mat4 &view, const glm::vec3& fog_color) {
    glDepthMask(GL_FALSE);
    this->bind(0);
    m_sh_prog.set_uniform_mat4f("u_proj", proj);
    m_sh_prog.set_uniform_mat4f("u_view", glm::mat4(glm::mat3(view)));
    m_sh_prog.set_uniform_1f("u_skybox", 0);
    m_sh_prog.set_uniform_1b("u_show_fog", true);
    m_sh_prog.set_uniform_3f("u_fog_color", fog_color);
    GLCall( glDrawArrays(GL_TRIANGLES, 0, 36) );
    glDepthMask(GL_TRUE);
}

void renderer::SkyBox::draw(const glm::mat4 &proj, const glm::mat4 &view) {
    glDepthMask(GL_FALSE);
    this->bind(0);
    m_sh_prog.set_uniform_mat4f("u_proj", proj);
    m_sh_prog.set_uniform_mat4f("u_view", glm::mat4(glm::mat3(view)));
    m_sh_prog.set_uniform_1f("u_skybox", 0);
    m_sh_prog.set_uniform_1b("u_show_fog", false);
    GLCall( glDrawArrays(GL_TRIANGLES, 0, 36) );
    glDepthMask(GL_TRUE);
}
