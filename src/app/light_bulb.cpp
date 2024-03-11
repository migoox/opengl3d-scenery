#include "light_bulb.hpp"

app::LightBulb::LightBulb(const std::shared_ptr<renderer::SphereMeshResource>& sphere_res) : mesh(sphere_res) {
    mesh.set_scale(glm::vec3(0.1f));
    mesh.set_origin(glm::vec3(0.1f/2.f));
    light.set_parent(mesh);
}

void app::LightBulb::draw(renderer::IShaderObject& sh_obj) {
    if (show_sphere) {
        mesh.get_material().set_albedo(light.color);
        mesh.draw(sh_obj);
    }
}
