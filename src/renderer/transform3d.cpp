#include "transform3d.hpp"
using namespace renderer;

void Transform3D::set_rot(const glm::quat &rotation) {
    m_model_dirty = true;
    m_rot_dirty = true;
    m_rot = rotation;
}

void Transform3D::rotate(const glm::quat &rotation) {
    m_model_dirty = true;
    m_rot_dirty = true;
    m_rot = glm::normalize(rotation * m_rot);
}

void Transform3D::rotate_around(const glm::vec3 &axis, float radians) {
    m_model_dirty = true;
    m_rot_dirty = true;
    m_rot = glm::normalize(glm::rotate(glm::quat(1.f, 0.f, 0.f, 0.f), radians, axis) * m_rot);
}

void Transform3D::rotate_around_local(const glm::vec3 &axis, float radians) {
    m_model_dirty = true;
    m_rot_dirty = true;
    m_rot = glm::normalize(m_rot * glm::rotate(glm::quat(1.f, 0.f, 0.f, 0.f), radians, axis));
}

void Transform3D::rotate_local(const glm::quat &rotation) {
    m_model_dirty = true;
    m_rot_dirty = true;
    m_rot = glm::normalize(m_rot * rotation);
}

void Transform3D::look_at(const glm::vec3 &point, const glm::vec3 &up) {
    m_model_dirty = true;
    m_rot_dirty = true;
    m_rot = glm::quatLookAtRH(glm::normalize(point - m_pos), up);
}

void Transform3D::set_pos(const glm::vec3 &pos) {
    m_model_dirty = true;
    m_pos = pos;
}

void Transform3D::move_local(const glm::vec3 &delta) {
    m_model_dirty = true;
    recalculate_axises();
    m_pos += m_right * delta.x + m_up * delta.y + m_front * delta.z;
}

void Transform3D::move(const glm::vec3 &delta) {
    m_model_dirty = true;
    m_pos += delta;
}

void Transform3D::set_origin(const glm::vec3 &origin) {
    m_model_dirty = true;
    m_origin = origin;
}

void Transform3D::set_scale(glm::vec3 scale) {
    m_model_dirty = true;
    m_scale = scale;
}

void Transform3D::recalculate_model() const {
    if (!m_model_dirty) {
        return;
    }
    m_model_dirty = false;
    m_model = glm::translate(glm::mat4(1.f), m_pos)
              * mat4_cast(m_rot)
              * glm::scale(glm::mat4(1.f), m_scale)
              * glm::translate(glm::mat4(1.f), -m_origin);
    m_inv_model = glm::inverse(m_model);
}

void Transform3D::recalculate_axises() const {
    if (!m_rot_dirty) {
        return;
    }
    m_rot_dirty = false;

    glm::mat3 m = mat3_cast(m_rot);

    m_right = glm::vec3(m[0][0], m[0][1], m[0][2]);
    m_up = glm::vec3(m[1][0], m[1][1], m[1][2]);
    m_front = glm::vec3(m[2][0], m[2][1], m[2][2]);
}

glm::mat4 Transform3D::get_world_model_mat() const {
    this->recalculate_model();
    if (m_parent.has_value()) {
        return m_parent->get().get_world_model_mat() * m_model;
    }
    return m_model;
}

glm::mat4 Transform3D::get_world_inv_model_mat() const {
    this->recalculate_model();
    if (m_parent.has_value()) {
        return m_inv_model * m_parent->get().get_world_inv_model_mat();
    }
    return m_inv_model;
}

const glm::mat4 &Transform3D::get_object_model_mat() const {
    this->recalculate_model();
    return m_model;
}

const glm::mat4 &Transform3D::get_object_inv_model_mat() const {
    this->recalculate_model();
    return m_inv_model;
}

glm::mat3 Transform3D::get_object_normal_mat() const {
    return glm::mat3(glm::transpose(this->get_object_inv_model_mat()));
}

glm::mat3 Transform3D::get_world_normal_mat() const {
    return glm::mat3(glm::transpose(this->get_world_inv_model_mat()));
}

const glm::vec3 &Transform3D::get_right() const {
    this->recalculate_axises();
    return m_right;
}

const glm::vec3 &Transform3D::get_up() const {
    this->recalculate_axises();
    return m_up;
}

const glm::vec3 &Transform3D::get_front() const {
    this->recalculate_axises();
    return m_front;
}

glm::vec3 Transform3D::get_world_right() const {
    return glm::vec3(m_parent->get().get_world_model_mat() * glm::vec4(get_right(), 0.f));
}

glm::vec3 Transform3D::get_world_up() const {
    return glm::vec3(m_parent->get().get_world_model_mat() * glm::vec4(get_up(), 0.f));
}

glm::vec3 Transform3D::get_world_front() const {
    return glm::vec3(m_parent->get().get_world_model_mat() * glm::vec4(get_front(), 0.f));
}


Transform3D::~Transform3D() {
    if (m_parent.has_value()) {
        // Remove from parent's children
        if (m_parent.has_value()) {
            m_parent->get().m_children.remove_if([&](std::reference_wrapper<Transform3D> wrapper) {
                return &wrapper.get() == this;
            });
        }

        // Move current's children to parrent
        auto parent_ref = m_parent->get();
        for (auto it = m_children.begin(); it != m_children.end(); ++it) {
            auto& child = it->get();
            // 1. Remove current from the child's parent
            child.m_parent = std::nullopt;
            // 2. Set new parent
            child.set_parent(parent_ref);
        }
    } else {
        for (auto& child : m_children) {
            child.get().m_parent = std::nullopt;
        }
    }

    m_children.clear();
    m_parent = std::nullopt;
}

void Transform3D::set_parent(Transform3D &parent) {
    this->remove_parent();
    parent.m_children.push_back(std::ref(*this));
    m_parent = std::ref(parent);
}

void Transform3D::remove_parent() {
    if (m_parent.has_value()) {
        m_parent->get().m_children.remove_if([&](std::reference_wrapper<Transform3D> wrapper) {
            return &wrapper.get() == this;
        });
    }

    m_parent = std::nullopt;
}

glm::vec3 Transform3D::get_world_pos() const {
    if (m_parent.has_value()) {
        return glm::vec3(m_parent->get().get_world_model_mat() * glm::vec4(m_pos, 1.f));
    }
    return m_pos;
}

void Transform3D::clear_model_mat() {
    m_pos = glm::vec3(0.f);
    m_origin = glm::vec3(0.f);
    m_scale = glm::vec3(1.f);
    m_rot = glm::quat(1.f, 0.f, 0.f, 0.f);

    m_rot_dirty = true;
    m_right = glm::vec3(1.f, 0.f, 0.f);
    m_up = glm::vec3(0.f, 1.f, 0.f);
    m_front = glm::vec3(0.f, 0.f, 1.f);

    m_model_dirty = true;
    m_model = glm::mat4(1.f);
    m_inv_model = glm::mat4(1.f);
}
