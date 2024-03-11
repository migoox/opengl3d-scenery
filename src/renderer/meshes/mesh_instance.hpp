#ifndef MESH_INSTANCE_H
#define MESH_INSTANCE_H
#include <memory>
#include "drawable.hpp"
#include "../lighting/material.hpp"
#include "mesh_resources/mesh_resource.hpp"

namespace renderer {
    class MeshInstance : public IDrawable {
    public:
        MeshInstance() = delete;
        explicit MeshInstance(const std::shared_ptr<const MeshResource>& mesh_res);

        const std::shared_ptr<const MeshResource>& get_mesh_res() const { return m_mesh; }

        Material& get_material() { return m_material; }
        const Material& get_material() const { return m_material; }

        void set_height_map(const std::shared_ptr<TextureResource>& texture_res);
        const std::optional<Texture>& get_height_map() const { return m_height_map; };

        void draw(IShaderObject& sh_obj) const override;
        // void draw_instanced(const ShaderProgram &program, size_t elems) const;

    private:
        std::shared_ptr<const MeshResource> m_mesh;
        Material m_material;
        std::optional<Texture> m_height_map = std::nullopt;
    };
}

#endif
