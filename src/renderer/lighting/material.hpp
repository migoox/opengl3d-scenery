#ifndef MATERIAL_H
#define MATERIAL_H
#include <glm/glm.hpp>
#include "../textures/texture.hpp"
#include "material_resource.hpp"

namespace renderer {
    class Material {
    public:
        Material() = default;
        explicit Material(const MaterialResource& mat_res);

        // Material can always be safely destructed
        ~Material() noexcept = default;

        bool use_fragment_discarding = false;
        bool use_albedo_mapping = false;
        bool use_specular_mapping = false;
        bool use_normal_mapping = false;
        bool add_normals = false;

        void set_albedo(const glm::vec3& albedo);
        const glm::vec3& get_albedo() const { return m_albedo; };

        void set_diffuse(const glm::vec3& diffuse);
        const glm::vec3& get_diffuse() const { return m_diffuse; };

        void set_specular(const glm::vec3& specular);
        const glm::vec3& get_specular() const { return m_specular; };

        void set_shininnes(float shininess);
        float get_shininnes() const { return m_shininess; };

        void set_discard_threshold(float discard_threshold);
        float get_discard_threshold() const { return m_discard_threshold; };

        void push_albedo_map(const std::shared_ptr<const TextureResource>& albedo_map);
        void push_specular_map(const std::shared_ptr<const TextureResource>& specular_map);
        void push_normal_map(const std::shared_ptr<const TextureResource>& normal_map);

        const std::vector<Texture>& get_albedo_maps() const { return m_albedo_maps; }
        const std::vector<Texture>& get_specular_maps() const { return m_specular_maps; }
        const std::vector<Texture>& get_normal_maps() const { return m_normal_maps; }

        const std::string& get_name() const { return m_name; }

    private:
        glm::vec3 m_albedo = glm::vec3(1.f);
        glm::vec3 m_diffuse = glm::vec3(0.8f);
        glm::vec3 m_specular = glm::vec3(0.9f);
        float m_shininess = 128.f;

        float m_discard_threshold = 0.1f;

        std::vector<Texture> m_albedo_maps;
        std::vector<Texture> m_specular_maps;
        std::vector<Texture> m_normal_maps;

        std::string m_name;
    };
}

#endif
