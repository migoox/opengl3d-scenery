#include <fstream>
#include <sstream>
#include "glm/glm.hpp"
#include "shader_resource.hpp"
#include "../gl_debug.h"

using namespace renderer;


ShaderResource::ShaderResource(const std::string& path) : m_path(path), m_type(ShaderType::VertexShader) {
    size_t dot_pos = path.find_last_of('.');
    std::string ext;
    if (dot_pos != std::string::npos && dot_pos < path.length()) {
        ext = path.substr(dot_pos + 1);
    } else {
        std::cerr << "[Shader Resource]: " << "Error: Invalid shader file extension: " << path << "\n";
        std::terminate();
    }

    std::string source = this->parse(path);
    if (ext == "vert") {
        m_type = ShaderType::VertexShader;
        m_id = this->compile(GL_VERTEX_SHADER, source);
    } else if (ext == "frag") {
        m_type = ShaderType::FragmentShader;
        m_id = this->compile(GL_FRAGMENT_SHADER, source);
    } else if (ext == "tesc") {
        m_type = ShaderType::TesselationControlShader;
        m_id = this->compile(GL_TESS_CONTROL_SHADER, source);
    } else if (ext == "tese") {
        m_type = ShaderType::TesselationEvaluationShader;
        m_id = this->compile(GL_TESS_EVALUATION_SHADER, source);
    }
}

ShaderResource::ShaderResource(ShaderResource &&other) noexcept
: m_id(other.m_id), m_type(other.m_type), m_path(std::move(other.m_path)) {
    other.m_id = 0;
}

std::string ShaderResource::parse(const std::string &path) {
    std::ifstream file(path.data());
    std::stringstream ss;

    if (!file.is_open())
    {
        std::cerr << "[Shader Resource]: " << "Error: Cannot load shader: " << path << "\n";
        std::terminate();
    }

    ss << file.rdbuf();
    file.close();

    return ss.str();
}

GLuint ShaderResource::compile(GLenum type, const std::string &source) {
    GLCall( GLenum id = glCreateShader(type) );

    const char* src = source.c_str();
    GLCall( glShaderSource(id, 1, &src, nullptr) );
    GLCall( glCompileShader(id) );

    // Error handling
    int result;
    GLCall( glGetShaderiv(id, GL_COMPILE_STATUS, &result) );
    std::cout << "[Shader Compiler]: " << "Shader " << m_path << " compile status: " << (result ? "Success" : "Failure") << std::endl;

    if ( result == GL_FALSE )
    {
        int length;
        GLCall( glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length) );
        char* message = (char*) alloca(length * sizeof(char));
        GLCall( glGetShaderInfoLog(id, length, &length, message) );
        std::cerr << "[Shader Compiler]: "
                  << "Failed to compile shader "
                  << m_path
                  << std::endl;
        std::cerr << message << std::endl;
        GLCall( glDeleteShader(id) );

        std::terminate();
    }

    return id;
}

ShaderResource::~ShaderResource() {
    if (m_id > 0) {
        GLCall( glDeleteShader(m_id) );
    }
}
