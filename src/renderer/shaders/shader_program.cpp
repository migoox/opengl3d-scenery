#include "shader_program.hpp"
#include "../gl_debug.h"
using namespace renderer;

ShaderProgram::ShaderProgram(const ShaderResource &vert_sh, const ShaderResource &frag_sh) {
    GLCall( m_id = glCreateProgram() );
    GLCall( glAttachShader(m_id, vert_sh.m_id) );
    GLCall( glAttachShader(m_id, frag_sh.m_id) );
    link_program(m_id);
}

ShaderProgram::ShaderProgram(const ShaderResource &vert_sh, const ShaderResource &tesc_sh,
                             const ShaderResource &tese_sh, const ShaderResource &frag_sh) {
    GLCall( m_id = glCreateProgram() );
    GLCall( glAttachShader(m_id, vert_sh.m_id) );
    GLCall( glAttachShader(m_id, tesc_sh.m_id) );
    GLCall( glAttachShader(m_id, tese_sh.m_id) );
    GLCall( glAttachShader(m_id, frag_sh.m_id) );
    link_program(m_id);
}

ShaderProgram::~ShaderProgram() {
    if (m_id > 0) {
        GLCall( glDeleteProgram(m_id) );
    }
}

void ShaderProgram::bind() const {
    GLCall( glUseProgram(m_id) );
}

void ShaderProgram::unbind() {
    GLCall( glUseProgram(0) );
}

GLint ShaderProgram::get_uniform_location(std::string_view name) {
    if (m_uniform_location_cache.find(name.data()) != m_uniform_location_cache.end())
        return m_uniform_location_cache[name.data()];

    GLCall( int location = glGetUniformLocation(m_id, name.data()) );
    if (location == -1)
        std::cerr << "[OpenGL] Warning: No active uniform variable with name " << name << " found" << std::endl;

    m_uniform_location_cache[name.data()] = location;

    return location;
}

void ShaderProgram::set_uniform_1b(std::string_view name, bool value) {
    this->bind();
    GLCall( glUniform1i(get_uniform_location(name.data()), value) );

}

void ShaderProgram::set_uniform_1i(std::string_view name, int value) {
    this->bind();
    GLCall( glUniform1i(get_uniform_location(name.data()), value) );
}

void ShaderProgram::set_uniform_2i(std::string_view name, int v1, int v2) {
    this->bind();
    GLCall( glUniform2i(get_uniform_location(name.data()), v1, v2) );
}

void ShaderProgram::set_uniform_1f(std::string_view name, float value) {
    this->bind();
    GLCall( glUniform1f(get_uniform_location(name.data()), value) );
}

void ShaderProgram::set_uniform_2f(std::string_view name, float f0, float f1) {
    this->bind();
    GLCall( glUniform2f(get_uniform_location(name.data()), f0, f1) );
}

void ShaderProgram::set_uniform_3f(std::string_view name, float f0, float f1, float f2) {
    this->bind();
    GLCall( glUniform3f(get_uniform_location(name.data()), f0, f1, f2) );
}

void ShaderProgram::set_uniform_3f(std::string_view name, glm::vec3 vec) {
    this->bind();
    GLCall( glUniform3f(get_uniform_location(name.data()), vec.x, vec.y, vec.z) );
}

void ShaderProgram::set_uniform_4f(std::string_view name, float f0, float f1, float f2, float f3) {
    this->bind();
    GLCall( glUniform4f(get_uniform_location(name.data()), f0, f1, f2, f3) );
}

void ShaderProgram::set_uniform_4f(std::string_view name, glm::vec4 vec) {
    this->bind();
    GLCall( glUniform4f(get_uniform_location(name.data()), vec.x, vec.y, vec.z, vec.w) );
}

void ShaderProgram::set_uniform_mat3f(std::string_view name, const glm::mat3& matrix) {
    this->bind();
    GLCall(glUniformMatrix3fv(get_uniform_location(name.data()), 1, GL_FALSE, &matrix[0][0]));
}

void ShaderProgram::set_uniform_mat4f(std::string_view name, const glm::mat4& matrix) {
    this->bind();
    GLCall( glUniformMatrix4fv(get_uniform_location(name.data()), 1, GL_FALSE, &matrix[0][0]) );
}

GLuint ShaderProgram::link_program(GLuint prog_id) {
    GLCall( glLinkProgram(prog_id) );

    GLint program_linked;

    GLCall( glGetProgramiv(prog_id, GL_LINK_STATUS, &program_linked) );
    std::cout << "[Shader Linker]: Program link status: " << (program_linked ? "Success" : "Failure") << std::endl;
    if (program_linked != GL_TRUE)
    {
        GLsizei log_length = 0;
        GLchar message[1024];
        GLCall( glGetProgramInfoLog(prog_id, 1024, &log_length, message) );
        std::cerr << "[Shader Linker]: Failed to link program" << std::endl;
        std::cerr << message << std::endl;
        std::terminate();
    }

    GLCall( glValidateProgram(prog_id) );
    return prog_id;
}

ShaderProgram::ShaderProgram(ShaderProgram &&other) noexcept
: m_id(other.m_id) {
    other.m_id = 0;
}

