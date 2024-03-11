#ifndef OPENGL_3D_SCENERY_TRANSFORM3D_HPP
#define OPENGL_3D_SCENERY_TRANSFORM3D_HPP
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include <list>
#include <optional>

namespace renderer {
    class Transform3D {
    public:
        Transform3D() = default;
        ~Transform3D();

        const glm::quat& get_rot() const { return m_rot; }
        void set_rot(const glm::quat& rotation);

        void rotate_around(const glm::vec3& axis, float radians);
        void rotate_around_local(const glm::vec3& axis, float radians);
        void rotate(const glm::quat& rotation);
        void rotate_local(const glm::quat& rotation);
        void look_at(const glm::vec3& point, const glm::vec3& up);

        const glm::vec3& get_pos() const { return m_pos; }
        glm::vec3 get_world_pos() const;
        void set_pos(const glm::vec3& pos);

        void move_local(const glm::vec3& delta);
        void move(const glm::vec3& delta);

        const glm::vec3& get_origin() const { return m_origin; }
        void set_origin(const glm::vec3& origin);

        const glm::vec3& get_scale() const { return m_scale; }
        void set_scale(glm::vec3 scale);

        glm::mat4 get_world_model_mat() const;
        glm::mat4 get_world_inv_model_mat() const;

        const glm::mat4& get_object_model_mat() const;
        const glm::mat4& get_object_inv_model_mat() const;

        glm::mat3 get_object_normal_mat() const;
        glm::mat3 get_world_normal_mat() const;

        const glm::vec3& get_right() const;
        const glm::vec3& get_up() const;
        const glm::vec3& get_front() const;

        glm::vec3 get_world_right() const;
        glm::vec3 get_world_up() const;
        glm::vec3 get_world_front() const;

        void set_parent(Transform3D& parent);
        void remove_parent();

        void clear_model_mat();

    private:
        void recalculate_model() const;
        void recalculate_axises() const;

    private:
        glm::vec3 m_pos = glm::vec3(0.f);
        glm::vec3 m_origin = glm::vec3(0.f);
        glm::vec3 m_scale = glm::vec3(1.f);
        glm::quat m_rot = glm::quat(1.f, 0.f, 0.f, 0.f);

        mutable bool m_rot_dirty = true;
        mutable glm::vec3 m_right = glm::vec3(1.f, 0.f, 0.f);
        mutable glm::vec3 m_up = glm::vec3(0.f, 1.f, 0.f);
        mutable glm::vec3 m_front = glm::vec3(0.f, 0.f, 1.f);

        mutable bool m_model_dirty = true;
        mutable glm::mat4 m_model = glm::mat4(1.f);
        mutable glm::mat4 m_inv_model = glm::mat4(1.f);

        std::list<std::reference_wrapper<Transform3D>> m_children;
        std::optional<std::reference_wrapper<Transform3D>> m_parent;
    };
}

#endif //OPENGL_3D_SCENERY_TRANSFORM3D_HPP
