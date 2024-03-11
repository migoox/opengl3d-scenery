#ifndef MESH_ARRAY_INSTANCE_H
#define MESH_ARRAY_INSTANCE_H
#include <memory>
#include <vector>
#include "mesh_resources/mesh_array_resource.hpp"
#include "drawable.hpp"

namespace renderer {
    class MeshArrayInstance : public IDrawable {
    public:
        MeshArrayInstance() = delete;
        explicit MeshArrayInstance(const std::shared_ptr<MeshArrayResource>& array_mesh_res);

        const std::shared_ptr<MeshArrayResource>& get_mesh_array_res() { return m_mesh_array; }

        Material& get_material(uint32_t index) { return m_materials[index]; }
        const Material& get_material(uint32_t index) const { return m_materials[index]; }
        uint32_t get_materials_count() const { return m_materials.size(); }

        void draw(IShaderObject& sh_obj) const override;

        virtual void draw_one(IShaderObject& sh_obj, uint32_t pair_index) const;

    private:
        void init_materials();

    protected:
        std::shared_ptr<MeshArrayResource> m_mesh_array;
        std::vector<Material> m_materials;
    };

}

#endif
