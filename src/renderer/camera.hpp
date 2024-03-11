#ifndef OPENGL_3D_SCENERY_CAMERA_HPP
#define OPENGL_3D_SCENERY_CAMERA_HPP

#include "glm/glm.hpp"
#include "GLFW/glfw3.h"
#include "transform3d.hpp"

namespace renderer {
    static glm::vec3 global_up() { return glm::vec3(0.f, 1.f, 0.f); }

    enum class Projection {
        Orthographic,
        Perspective
    };

    class Camera : public Transform3D {
    public:
        Camera() = delete;
        Camera(float width, float height, float fov, float znear, float zfar);

        void set_proj_type(Projection type);
        void resize_screen(float width, float height);
        void set_fov(float fov);
        void set_znear(float znear);
        void set_zfar(float zfar);

        float get_fov() const { return m_fov; }
        float get_znear() const { return m_znear; }
        float get_zfar() const { return m_zfar; }

        glm::mat4 get_view() const;
        glm::mat4 get_inv_view() const;
        const glm::mat4& get_proj() const;
        const glm::mat4& get_inv_proj() const;

    private:

        void recalculate_proj() const;

    private:
        Projection m_proj_type = Projection::Perspective;

        mutable bool m_proj_dirty = true;
        mutable glm::mat4 m_inv_proj = glm::mat4(1.f);
        mutable glm::mat4 m_proj = glm::mat4(1.f);

        float m_width, m_height, m_fov, m_znear, m_zfar;
    };
}

#endif
