#include "terrain.hpp"
#include "../renderer/shaders.hpp"

using namespace renderer;

app::Terrain::Terrain() : MeshArrayInstance(MeshArrayResource::from_path("res/terrain_dristibute_gn/scene.gltf")) {
    // Reordering is required, since the terrain should be drawn first (bushes alpha blending)
    int n = this->get_mesh_array_res()->get_mesh_mat_id_pairs().size();
    std::vector<uint32_t> indices;
    indices.reserve(n);
    for (uint32_t i = 0; i < n; ++i) {
        indices.push_back((i + n - 1) % n);
    }
    this->get_mesh_array_res()->reorder_meshes(indices);
}

void app::Terrain::draw(IShaderObject &sh_obj) const {
    uint32_t i = 0;
    for (auto& m : m_mesh_array->get_mesh_mat_id_pairs()) {
        if (this->get_material(m.second).get_name() == "Bush_Mediteranean_Frond_Mat") {
            sh_obj.get_prog().set_uniform_1b("u_deform", true);
        } else {
            sh_obj.get_prog().set_uniform_1b("u_deform", false);
        }
        draw_one(sh_obj, i);
        ++i;
    }


}
