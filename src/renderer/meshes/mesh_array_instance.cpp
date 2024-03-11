#include "mesh_array_instance.hpp"
#include "../shaders/shader_object.hpp"
using namespace renderer;

MeshArrayInstance::MeshArrayInstance(const std::shared_ptr<MeshArrayResource> &array_mesh_res)
: m_mesh_array(array_mesh_res) {
    init_materials();
}

void MeshArrayInstance::init_materials() {
    for (auto& mat_res : m_mesh_array->get_material_resources()) {
        m_materials.emplace_back(mat_res);
    }
}

void MeshArrayInstance::draw(IShaderObject &sh_obj) const {
    for (auto& m : m_mesh_array->get_mesh_mat_id_pairs()) {
        sh_obj.add_drawable_obj(*this, m_materials[m.second]);
        m.first->draw_gl();
    }
}

void MeshArrayInstance::draw_one(IShaderObject &sh_obj, uint32_t pair_index) const {
    sh_obj.add_drawable_obj(*this, m_materials[m_mesh_array->get_mesh_mat_id_pairs()[pair_index].second]);
    m_mesh_array->get_mesh_mat_id_pairs()[pair_index].first->draw_gl();
}

