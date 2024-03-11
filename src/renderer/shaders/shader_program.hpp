#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H
#include "shader_resource.hpp"

namespace renderer {
    class ShaderProgram {
    public:
        ShaderProgram(const ShaderResource& vert_sh, const ShaderResource& frag_sh);

        ShaderProgram(const ShaderResource& vert_sh, const ShaderResource& tesc_sh,
                      const ShaderResource& tese_sh, const ShaderResource& frag_sh);

        ShaderProgram(ShaderProgram&& other) noexcept;

        ShaderProgram(const ShaderProgram& other) = delete;
        ShaderProgram& operator=(const ShaderProgram& other) = delete;
        ShaderProgram& operator=(ShaderProgram&& other) = delete;

        ~ShaderProgram();

        void bind() const;
        static void unbind();

        void set_uniform_1b(std::string_view name, bool value);
        void set_uniform_1i(std::string_view name, int value);
        void set_uniform_2i(std::string_view name, int v1, int v2);
        void set_uniform_1f(std::string_view name, float value);
        void set_uniform_2f(std::string_view name, float f0, float f1);
        void set_uniform_3f(std::string_view name, float f0, float f1, float f2);
        void set_uniform_3f(std::string_view name, glm::vec3 vec);
        void set_uniform_4f(std::string_view name, float f0, float f1, float f2, float f3);
        void set_uniform_4f(std::string_view name, glm::vec4 vec);
        void set_uniform_mat3f(std::string_view name, const glm::mat3& matrix);
        void set_uniform_mat4f(std::string_view name, const glm::mat4& matrix);

    private:
        GLint get_uniform_location(std::string_view name);
        static GLuint link_program(GLuint prog_id);

    private:
        GLuint m_id = 0;
        std::unordered_map<std::string, int> m_uniform_location_cache;
    };
}

#endif
