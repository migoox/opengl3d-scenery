#include "imgui_ext.hpp"
#include "../vendor/imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

int imgui_ext::UniqueID::current_id = 0;

void imgui_ext::position_sliders(glm::vec3 &pos) {
    std::string unique = UniqueID::get();

    ImGui::PushID(unique.c_str());
    ImGui::SliderFloat("X", &pos.x, -50.f, 50.f);
    ImGui::SliderFloat("Y", &pos.y, -50.f, 50.f);
    ImGui::SliderFloat("Z", &pos.z, -50.f, 50.f);
    ImGui::PopID();
}

void imgui_ext::point_light_opts(renderer::PointLight &point_light) {
    std::string unique = UniqueID::get();

    ImGui::PushID(unique.c_str());
    ImGui::ColorEdit3("Color", glm::value_ptr(point_light.color));
    ImGui::ColorEdit3("Ambient", glm::value_ptr(point_light.ambient));
    ImGui::SliderFloat("Radius", &point_light.radius, 0.1f, 40.f);
    ImGui::SliderFloat("Linear", &point_light.linear, 0.2f, 10.f);
    ImGui::SliderFloat("Quadratic", &point_light.quadratic, 0.2f, 10.f);
    ImGui::PopID();
}

void imgui_ext::spotlight_opts(renderer::Spotlight &spotlight) {
    std::string unique = UniqueID::get();

    ImGui::PushID(unique.c_str());
    ImGui::ColorEdit3("Color", glm::value_ptr(spotlight.color));
    ImGui::ColorEdit3("Ambient", glm::value_ptr(spotlight.ambient));
    ImGui::SliderAngle("Outer Angle", &spotlight.outer_angle, 0.f, 120.f);
    ImGui::SliderAngle("Inner Angle", &spotlight.inner_angle, 0.f, 110.f);
    ImGui::PopID();
}

void imgui_ext::dir_light_opts(renderer::DirectionalLight &dir_light) {
    static float polar = glm::radians(-32.f), azim = glm::radians(45.f);
    std::string unique = UniqueID::get();

    ImGui::PushID(unique.c_str());
    ImGui::ColorEdit3("Color", glm::value_ptr(dir_light.color));
    ImGui::ColorEdit3("Ambient", glm::value_ptr(dir_light.ambient));
    ImGui::SliderAngle("Azimuth Angle", &azim, -180.f, 180.f);
    ImGui::SliderAngle("Polar Angle", &polar, -180.f, 0.f);
    ImGui::PopID();

    dir_light.set_rot(glm::quat(1.f, 0.f, 0.f, 0.f));
    dir_light.rotate_around(glm::vec3(0.f, 1.f, 0.f), azim);
    dir_light.rotate_around_local(glm::vec3(1.f, 0.f, 0.f), polar);
}

std::string imgui_ext::UniqueID::get() {
    return "imgui_ext" + std::to_string(current_id++);
}

void imgui_ext::UniqueID::reset() {
    current_id = 0;
}

void imgui_ext::begin_frame() {
    UniqueID::reset();
}
