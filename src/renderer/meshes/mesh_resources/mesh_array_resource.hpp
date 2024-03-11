#ifndef MESH_ARRAY_RESOURCE_H
#define MESH_ARRAY_RESOURCE_H
#include <iostream>
#include <memory>
#include <vector>
#include "assimp/scene.h"
#include "mesh_resource.hpp"
#include "../../lighting/material.hpp"

namespace renderer {
    class MeshArrayResource {
    public:
        static std::shared_ptr<MeshArrayResource> from_path(std::string_view path);
        static std::shared_ptr<MeshArrayResource> from_path(std::string_view path, glm::mat4 vert_multiplier);

        const std::vector<MaterialResource>& get_material_resources() const { return m_materials; }

        const std::vector<std::pair<std::shared_ptr<const MeshResource>, uint32_t>>& get_mesh_mat_id_pairs() const {
            return m_meshes;
        }

        void reorder_meshes(std::vector<uint32_t> indices) {
            if (indices.size() != m_meshes.size()) {
                std::cerr << "[Mesh Array Resource]: Reordering failed, indices vector size must match the pairs count.\n";
            }

            std::vector<std::pair<std::shared_ptr<const MeshResource>, uint32_t>> new_meshes;
            new_meshes.reserve(indices.size());
            for (int i = 0; i < indices.size(); ++i) {
                new_meshes.push_back(m_meshes[indices[i]]);
            }

            m_meshes = new_meshes;
        }

    private:
        void process_node(aiNode *node, const aiScene *scene);
        void add_mesh(aiMesh *mesh, const aiNode *curr_node);
        void load_materials(const aiScene *scene);
        void load_model(std::string_view path);

    private:
        explicit MeshArrayResource(std::string_view path);
        explicit MeshArrayResource(std::string_view path, glm::mat4 vert_multiplier);

        // Pairs: MeshResource and MaterialResource index
        std::vector<std::pair<std::shared_ptr<const MeshResource>, uint32_t>> m_meshes;

        std::vector<MaterialResource> m_materials;

        std::optional<std::string> m_directory = std::nullopt;

        bool m_multiply = false;
        glm::mat4 m_vert_multiplier = glm::mat4(1.f);
        glm::mat3 m_vert_multiplier_inv = glm::mat4(1.f);
    };
}

#endif
