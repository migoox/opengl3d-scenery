#ifndef SHADER_OBJECT_HPP
#define SHADER_OBJECT_HPP
#include <string>
#include "../meshes.hpp"
#include "../lighting.hpp"
#include "../camera.hpp"
#include "shader_program.hpp"

namespace renderer {
    // Renderer represents a pipeline
    class IShaderObject {
    public:
        virtual void new_frame() = 0;
        virtual void set_camera(const Camera& camera) = 0;

        void add_light(const Light& light);
        virtual void add_mesh(const MeshInstance& mesh) = 0;
        virtual void add_drawable_obj(const IDrawable& obj, const Material& mat) = 0;

        virtual void add_directional_light(const DirectionalLight& light) = 0;
        virtual void add_point_light(const PointLight& light) = 0;
        virtual void add_spotlight(const Spotlight& light) = 0;
        virtual ShaderProgram& get_prog() = 0;
    };

    enum class ShadingType {
        Phong = 0,
        Gouraud = 1,
        Const = 2
    };

    class MaterialShaderObject : public IShaderObject {
    public:
        MaterialShaderObject() = default;

        void set_type(ShadingType type);
        ShadingType get_type() const {return m_shading_type; }

        void new_frame() override;
        void set_camera(const Camera& camera) override;

        void add_mesh(const MeshInstance& mesh) override;
        void add_drawable_obj(const IDrawable& obj, const Material& mat) override;

        void add_directional_light(const DirectionalLight& light) override;
        void add_point_light(const PointLight& light) override;
        void add_spotlight(const Spotlight& light) override;

        ShaderProgram& get_prog() override { return m_sh_prog[static_cast<int>(m_shading_type)]; }

    protected:
        ShaderProgram m_sh_prog[3] = {
                ShaderProgram(ShaderResource("res/mesh.vert"), ShaderResource("res/material.frag")),
                ShaderProgram(ShaderResource("res/gouraud_mesh.vert"), ShaderResource("res/gouraud_material.frag")),
                ShaderProgram(ShaderResource("res/const_mesh.vert"), ShaderResource("res/const_material.frag"))
        };

    private:
        ShadingType m_shading_type = ShadingType::Phong;

        uint32_t m_dir_light_count = 0;
        uint32_t m_fin_point_light_count = 0;
        uint32_t m_inf_point_light_count = 0;
        uint32_t m_spotlight_count = 0;
    };

};

#endif
