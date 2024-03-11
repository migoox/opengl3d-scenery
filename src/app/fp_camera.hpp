#ifndef OPENGL_3D_SCENERY_FP_CAMERA_HPP
#define OPENGL_3D_SCENERY_FP_CAMERA_HPP
#include "../renderer/window.hpp"
#include "../renderer/camera.hpp"

namespace app {
    enum class CamType {
        Free,
        FollowStatic,
        Follow
    };
    class CameraOperator {
    public:
        CameraOperator() = delete;
        CameraOperator(float width, float height, float fov, float znear, float zfar);

        renderer::Camera& get_cam() { return m_cam; }
        bool update(const renderer::Window& window, float dt);

        CamType type = CamType::Free;

    private:
        glm::vec2 last_mouse_pos = glm::vec2(0.f, 0.f);
        renderer::Camera m_cam;
    };
}



#endif //OPENGL_3D_SCENERY_FP_CAMERA_HPP
