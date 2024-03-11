#include "camera.hpp"
#include <cmath>
#include "glm/ext/matrix_clip_space.hpp"

using namespace renderer;

Camera::Camera(float width, float height, float fov, float znear, float zfar)
: m_width(width), m_height(height), m_fov(fov), m_znear(znear), m_zfar(zfar) { }

void Camera::resize_screen(float width, float height) {
    m_proj_dirty = true;
    m_width = width;
    m_height = height;
}

void Camera::set_fov(float fov) {
    m_proj_dirty = true;
    m_fov = fov;
}

void Camera::set_znear(float znear) {
    m_proj_dirty = true;
    m_znear = znear;
}

void Camera::set_zfar(float zfar) {
    m_proj_dirty = true;
    m_zfar = zfar;
}

glm::mat4 Camera::get_view() const {
    return this->get_world_inv_model_mat();
}

glm::mat4 Camera::get_inv_view() const {
    return this->get_world_model_mat();
}

const glm::mat4 &Camera::get_proj() const {
    this->recalculate_proj();
    return m_proj;
}

const glm::mat4 &Camera::get_inv_proj() const {
    this->recalculate_proj();
    return m_inv_proj;
}

void Camera::recalculate_proj() const {
    if (!m_proj_dirty) {
        return;
    }
    m_proj_dirty = false;

    if (m_proj_type == Projection::Orthographic) {
        // TODO
//        m_inv_proj = glm::mat4(0.f);
//        m_inv_proj[0][0] = 2.f / m_width;
//		m_inv_proj[1][1] = 2.f / m_height;
//		m_inv_proj[2][2] = - 2.f / (m_zfar - m_znear);
//		m_inv_proj[0][3] = - 1.f;
//		m_inv_proj[1][3] = - 1.f;
//		m_inv_proj[2][3] = - (m_zfar + m_znear) / (m_zfar - m_znear);
//
//        m_proj = glm::ortho(0.f, m_width, 0.f, m_height, m_znear, m_zfar);
    } else {
        float tan_half_fov = glm::tan(m_fov / 2.f);
        float aspect = m_width / m_height;
        m_inv_proj = glm::mat4(0.f);
        m_inv_proj[0][0] = aspect * tan_half_fov;
        m_inv_proj[1][1] = tan_half_fov;
        m_inv_proj[2][3] = (m_znear - m_zfar) / (2.f * m_zfar * m_znear);
        m_inv_proj[3][2] = -1.f;
        m_inv_proj[3][3] = (m_znear + m_zfar) / (2.f * m_zfar * m_znear);

        m_proj = glm::perspective(m_fov, m_width / m_height, m_znear, m_zfar);
    }
}

void Camera::set_proj_type(Projection type) {
    m_proj_dirty = true;
    m_proj_type = type;
}
