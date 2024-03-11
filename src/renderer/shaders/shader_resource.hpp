#ifndef SHADER_PROGRAM_HPP
#define SHADER_PROGRAM_HPP
#include "GL/glew.h"
#include "glm/glm.hpp"
#include <string>
#include <unordered_map>

namespace renderer {
    enum class ShaderType {
        VertexShader,
        FragmentShader,
        TesselationControlShader,
        TesselationEvaluationShader
    };

    class ShaderProgram;

    class ShaderResource {
    public:
        ShaderResource() = delete;

        explicit ShaderResource(const std::string& path);

        ShaderResource(const ShaderResource& other) = delete;
        ShaderResource(ShaderResource&& other) noexcept;

        ShaderResource& operator=(const ShaderResource& other) = delete;
        ShaderResource& operator=(ShaderResource&& other) = delete;
        ~ShaderResource();

        ShaderType get_type() const { return m_type; };

        friend ShaderProgram;

    private:
        static std::string parse(const std::string& path);
        GLuint compile(GLenum type, const std::string& source);

    private:
        GLuint m_id = 0;
        std::string m_path;
        ShaderType m_type;
    };
}

#endif