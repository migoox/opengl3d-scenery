#include "mesh_instance.hpp"
#include "../shaders/shader_object.hpp"
using namespace renderer;


MeshInstance::MeshInstance(const std::shared_ptr<const MeshResource> &mesh)
: m_mesh(mesh) { }

void MeshInstance::draw(IShaderObject &sh_obj) const {
    sh_obj.add_mesh(*this);
    m_mesh->draw_gl();
}

void MeshInstance::set_height_map(const std::shared_ptr<TextureResource>& texture_res) {
    m_height_map.emplace(texture_res);
}
