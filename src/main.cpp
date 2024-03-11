#include <GL/glew.h>
#include <iostream>
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/backend/imgui_impl_glfw.h"
#include "vendor/imgui/backend/imgui_impl_opengl3.h"


#include <GLFW/glfw3.h>

#include <filesystem>
#include "renderer/camera.hpp"
#include "renderer/meshes.hpp"
#include "renderer/shaders/shader_resource.hpp"
#include "renderer/window.hpp"
#include "renderer/textures.hpp"
#include "renderer/lighting/spotlight.hpp"
#include "app/tess_sphere.hpp"

#include "app/terrain.hpp"
#include "app/fp_camera.hpp"
#include "app/light_bulb.hpp"

#include "renderer/gl_debug.h"
#include "renderer/shaders.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "app/imgui_ext.hpp"
#include "app/bspline.h"
#include "imgui_internal.h"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

using namespace renderer;

// Main code
int main(int, char**)
{
    // Init GLFW
    Backend::init_glfw();
    {
        // Create a window
        Window window(1280, 720, "OpenGL 3D scenery");
        glfwWindowHint(GLFW_SAMPLES, 4);

        // Init GLEW and ImGui
        Backend::init_glew();
        Backend::init_imgui(window);

        glfwWindowHint(GLFW_SAMPLES, 4);
        glEnable(GL_MULTISAMPLE);

        // Application state
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        std::string executable_dir = std::filesystem::path(__FILE__).parent_path().string();

        // Mesh resources
        auto box_res = BoxMeshResource::create();
        auto sphere_res = SphereMeshResource::create(80);

        glm::mat4 rot = glm::rotate(glm::mat4(1.f), glm::radians(180.f), glm::vec3(1.f, 0.f, 0.f));
        rot = glm::rotate(rot, glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
        rot = glm::rotate(rot, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
        auto plane_res= MeshArrayResource::from_path("res/napier-heston_type_5_racer/scene.gltf", rot);

        SkyBox skybox = SkyBox(
                {
                        "res/skybox/right.jpg",
                        "res/skybox/left.jpg",
                        "res/skybox/bottom.jpg",
                        "res/skybox/top.jpg",
                        "res/skybox/front.jpg",
                        "res/skybox/back.jpg"
                }
        );
        app::Terrain terr = app::Terrain();

        app::TessSphere sphere;
        sphere.set_parent(terr);
        sphere.set_pos(glm::vec3(2.3f, 16.1f, 3.f));
        // ROCK
        terr.get_material(0).set_diffuse(glm::vec3(0.8f));
        terr.get_material(0).set_specular(glm::vec3(0.15f));
        terr.get_material(0).set_shininnes(4.f);

        // BUSH
        terr.get_material(1).set_diffuse(glm::vec3(0.661f, 0.577f, 0.675f));
        terr.get_material(1).set_specular(glm::vec3(0.789, 0.386, 0.469));
        terr.get_material(1).set_shininnes(8.f);

        // TERRAIN
        terr.get_material(2).set_diffuse(glm::vec3(0.758f, 0.8, 0.458));
        terr.get_material(2).set_specular(glm::vec3(0.417, 0.444, 0.442));
        terr.get_material(2).set_shininnes(4.f);

        MeshArrayInstance plane(plane_res);
        plane.set_parent(terr);

        app::CameraOperator cam_op(1280.f, 720.f, glm::radians(90.f), 0.1f, 200.f);
        cam_op.get_cam().set_parent(terr);
        const glm::vec3 def_cam_pos = glm::vec3(13.3f, 14.1f, 2.f);
        cam_op.get_cam().set_pos(def_cam_pos);

        app::LightBulb light_bulb(sphere_res);
        light_bulb.get_transform().set_parent(terr);
        light_bulb.get_transform().set_pos(glm::vec3(-5.f, 5.5f, 6.6f));
        light_bulb.light.color = glm::vec3(120.f, 231.f, 255.f) / 255.f;
        light_bulb.light.ambient = glm::vec3(26.f) / 255.f;
        light_bulb.light.radius = 14.f;

        app::LightBulb light_bulb2(sphere_res);
        light_bulb2.get_transform().set_parent(terr);
        light_bulb2.get_transform().set_pos(glm::vec3(11.8f, 3.9f, 3.86f));
        light_bulb2.light.color = glm::vec3(255.f, 222.f, 143.f) / 255.f;
        light_bulb2.light.ambient = glm::vec3(26.f) / 255.f;
        light_bulb2.light.radius = 14.f;

        DirectionalLight sun;
        sun.color = glm::vec3(255.f, 252.f, 241.f) / 255.f;
        sun.ambient = glm::vec3(83.f) / 255.f;
        sun.rotate_around(glm::vec3(0.f, 1.f, 0.f), glm::radians(45.f));
        sun.rotate_around_local(glm::vec3(1.f, 0.f, 0.f), glm::radians(-32.f));
        sun.set_parent(terr);

        Spotlight spotlight;
        spotlight.set_parent(plane);

        std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
        std::chrono::steady_clock::time_point previous_time = current_time;

        auto sh_obj = MaterialShaderObject();
        float distance = 50.f;
        bool show_dist_fog = false;
        glm::vec3 fog_color = glm::vec3(165.f, 198.f, 195.f) / 255.f;

        GLCall( glEnable(GL_DEPTH_TEST) );
        GLCall( glEnable(GL_BLEND) );
        GLCall( glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );
        GLCall (glEnable(GL_MULTISAMPLE) );

        plane.set_pos(glm::vec3(6.26f, 4.f, 6.6f));
        std::vector<glm::vec3> control_points = {
                glm::vec3(5.156f, 4.f, 9.37f),
                glm::vec3(5.8f, 5.5f, 26.2f),
                glm::vec3(-8.6f, 8.2f, 35.2f),
                glm::vec3(-23.f, 9.8f, 29.3f),
                glm::vec3(-26.f, 12.f, 11.72f),
                glm::vec3(-26.f, 11.7f, -16.0f),
                glm::vec3(-12.f, 6.641f, -23.437f)
        };

        float anim_duration = 14.f;
        float anim_moment = 0.f;

        bool show_anim_boxes = false;
        bool show_anim = true;
        std::vector<MeshInstance> boxes;
        boxes.reserve(control_points.size());

        for (const auto & point : control_points) {
            boxes.emplace_back(box_res);
            boxes[boxes.size() - 1].set_pos(point);
            boxes[boxes.size() - 1].set_parent(terr);
        }

        BSplineCurve spline_curve(control_points, 3, true);

        // Main loop
        while (!window.should_close()) {
            glfwPollEvents();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            {
                ImGui::Begin("Controls");
                imgui_ext::begin_frame();

                if (ImGui::CollapsingHeader("Lighting")) {

                    if (ImGui::TreeNode("Sun")) {
                        imgui_ext::dir_light_opts(sun);

                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNode("Point Light 1")) {

                        auto pos = light_bulb.get_transform().get_pos();
                        imgui_ext::position_sliders(pos);
                        light_bulb.get_transform().set_pos(pos);
                        imgui_ext::point_light_opts(light_bulb.light);

                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNode("Point Light 2")) {
                        auto pos = light_bulb2.get_transform().get_pos();
                        imgui_ext::position_sliders(pos);
                        light_bulb2.get_transform().set_pos(pos);
                        imgui_ext::point_light_opts(light_bulb2.light);

                        ImGui::TreePop();
                    }
                }

                if (ImGui::CollapsingHeader("Plane aircraft")) {
                    if (ImGui::TreeNode("Animation")) {
                        ImGui::Checkbox("Show anim boxes", &show_anim_boxes);
                        if (ImGui::Checkbox("Show animation", &show_anim)) {
                            anim_moment = 0.f;
                        }
                        if (show_anim){
                            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                        }
                        ImGui::SliderFloat("Animation duration", &anim_duration, 2.f, 30.f);
                        if (show_anim) {
                            ImGui::PopItemFlag();
                        }

                        ImGui::TreePop();
                    }

                    if (ImGui::TreeNode("Spotlight")) {
                        static float polar = 0.f, azim = 0.f;
                        ImGui::SliderAngle("Polar", &polar, -180.f, 180.f);
                        ImGui::SliderAngle("Azim", &azim, -180.f, 180.f);
                        imgui_ext::spotlight_opts(spotlight);

                        spotlight.set_rot(glm::quat(1.f, 0.f, 0.f, 0.f));
                        spotlight.rotate_around(glm::vec3(0.f, 1.f, 0.f), azim);
                        spotlight.rotate_around_local(glm::vec3(1.f, 0.f, 0.f), polar);

                        ImGui::TreePop();
                    }
                }

                if (ImGui::CollapsingHeader("Camera")) {
                    static app::CamType selected = app::CamType::Free;
                    static float zfar = 200.f;

                    if (ImGui::SliderFloat("ZFar", &zfar, 0.f, 500.f)) {
                        cam_op.get_cam().set_zfar(zfar);
                    }

                    if (ImGui::RadioButton("Free", selected == app::CamType::Free)) {
                        selected = app::CamType::Free;
                        cam_op.get_cam().set_parent(terr);
                    }

                    if (ImGui::RadioButton("Follow Static", selected == app::CamType::FollowStatic)) {
                        selected = app::CamType::FollowStatic;
                        cam_op.get_cam().set_parent(terr);
                        cam_op.get_cam().set_pos(def_cam_pos);
                    }

                    if (ImGui::RadioButton("Follow", selected == app::CamType::Follow)) {
                        selected = app::CamType::Follow;
                        cam_op.get_cam().clear_model_mat();
                        cam_op.get_cam().set_parent(plane);
                        cam_op.get_cam().set_pos(glm::vec3(0.f, 2.5f, 1.5f));
                    }
                    cam_op.type = selected;
                }

                if (ImGui::CollapsingHeader("Shading")) {
                    static ShadingType selected = ShadingType::Phong;

                    if (ImGui::RadioButton("Phong", selected == ShadingType::Phong)) {
                        selected = ShadingType::Phong;
                        sh_obj.set_type(ShadingType::Phong);
                        sphere.set_type(ShadingType::Phong);
                    }

                    if (ImGui::RadioButton("Gouraud", selected == ShadingType::Gouraud)) {
                        selected = ShadingType::Gouraud;
                        sh_obj.set_type(ShadingType::Gouraud);
                        sphere.set_type(ShadingType::Gouraud);
                    }

                    if (ImGui::RadioButton("Const", selected == ShadingType::Const)) {
                        selected = ShadingType::Const;
                        sh_obj.set_type(ShadingType::Const);
                        sphere.set_type(ShadingType::Const);
                    }
                }

                if (ImGui::CollapsingHeader("Distance fog (linear)")) {
                    ImGui::ColorEdit3("Color##fog", glm::value_ptr(fog_color));
                    sh_obj.get_prog().set_uniform_3f("u_fog_color", fog_color);

                    ImGui::SliderFloat("View range", &distance, 0.f, 500.f);
                    sh_obj.get_prog().set_uniform_1f("u_view_range", distance);

                    ImGui::Checkbox("Show", &show_dist_fog);
                }

                if (ImGui::CollapsingHeader("Sphere")) {
                    ImGui::Checkbox("Show", &sphere.show);

                    int lvl = sphere.get_tess_level();
                    if (ImGui::SliderInt("Tesselation level", &lvl, 4, 32)) {
                        sphere.set_tess_level(lvl);
                    }

                    auto scale = sphere.get_scale();
                    if (ImGui::SliderFloat3("Scale", glm::value_ptr(scale), 0.5f, 4.f)) {
                        sphere.set_scale(scale);
                    }
                    ImGui::Checkbox("Wireframe", &sphere.wireframe);
                }

                if (ImGui::CollapsingHeader("Terrain")) {
                    for (int i = 0; i < terr.get_materials_count(); ++i) {
                        auto& mat = terr.get_material(i);
                        if (mat.get_name() == "") {
                           continue;
                        }

                        ImGui::PushID(i);
                        if (ImGui::TreeNode(mat.get_name().c_str())) {
                            glm::vec3 diffuse = mat.get_diffuse();
                            if (ImGui::SliderFloat3("Diffuse", glm::value_ptr(diffuse), 0.f, 1.f)) {
                                mat.set_diffuse(diffuse);
                            }

                            glm::vec3 specular = mat.get_specular();
                            if (ImGui::SliderFloat3("Specular", glm::value_ptr(specular), 0.f, 1.f)) {
                                mat.set_specular(specular);
                            }

                            float shininnes = mat.get_shininnes();
                            if (ImGui::SliderFloat("Shininnes", &shininnes, 0.f, 512.f)) {
                                mat.set_shininnes(shininnes);
                            }

                            ImGui::TreePop();
                        }
                        ImGui::PopID();
                    }
                }

                ImGui::End();
            }

            ImGui::Render();

            if (window.resized()) {
                cam_op.get_cam().resize_screen(window.get_size().x, window.get_size().y);
            }

            // Calculate delta time
            current_time = std::chrono::steady_clock::now();
            std::chrono::duration<float> delta_time = std::chrono::duration_cast<std::chrono::duration<float>>(current_time - previous_time);
            previous_time = current_time;

            // Get the delta time in seconds
            float dt_as_seconds = delta_time.count();

            // Plane animation
            if (show_anim) {
                anim_moment += dt_as_seconds;
                if (anim_moment > anim_duration) {
                    anim_moment = 0.f;
                }
                plane.set_pos(spline_curve.calc(anim_moment/anim_duration));
                plane.look_at(plane.get_world_pos() + spline_curve.calc_tangent(anim_moment/anim_duration), glm::vec3(0.f, 1.f, 0.f));
            }

            cam_op.update(window, dt_as_seconds);
            if (cam_op.type == app::CamType::FollowStatic) {
                cam_op.get_cam().look_at(plane.get_pos(), glm::vec3(0.f, 1.f, 0.f));
            }

            // Update opengl viewport
            auto fb_size = window.get_framebuffer_size();
            glViewport(0, 0, fb_size.x, fb_size.y);

            // Clear framebuffer
            glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (show_dist_fog) {
                skybox.draw(cam_op.get_cam().get_proj(), cam_op.get_cam().get_view(), fog_color);
            } else {
                skybox.draw(cam_op.get_cam().get_proj(), cam_op.get_cam().get_view());
            }

            sphere.get_sh().set_uniform_mat4f("u_proj", cam_op.get_cam().get_proj());
            sphere.get_sh().set_uniform_mat4f("u_view", cam_op.get_cam().get_view());
            sphere.get_sh().set_uniform_3f("u_camera_pos", cam_op.get_cam().get_world_pos());
            sphere.add_lights({&spotlight}, {&sun}, {&light_bulb.light, &light_bulb2.light});

            sphere.draw();

            sh_obj.new_frame();
            sh_obj.get_prog().set_uniform_1b("u_show_distance_fog", show_dist_fog);
            sh_obj.get_prog().set_uniform_1f("u_view_range", distance);
            sh_obj.get_prog().set_uniform_3f("u_fog_color", fog_color);

            sh_obj.set_camera(cam_op.get_cam());

            sh_obj.add_light(spotlight);
            sh_obj.add_light(light_bulb.light);
            sh_obj.add_light(light_bulb2.light);

            sh_obj.add_light(sun);

            sh_obj.get_prog().set_uniform_1b("u_deform", false);
            sh_obj.get_prog().set_uniform_1f("u_time", std::chrono::duration_cast<std::chrono::duration<float>>(current_time.time_since_epoch()).count());
            terr.draw(sh_obj);
            sh_obj.get_prog().set_uniform_1b("u_deform", false);

            if (show_anim_boxes) {
                for (const auto& box : boxes) {
                    box.draw(sh_obj);
                }
            }

            light_bulb.draw(sh_obj);
            light_bulb2.draw(sh_obj);
            plane.draw(sh_obj);

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            window.swap_buffers();
        }
        Backend::terminate_imgui();
        // glfw window terminates here
    }
    Backend::terminate_glfw();

    return 0;
}
