#include <iostream>
#include "mesh_array_resource.hpp"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

using namespace renderer;

void add_textures(aiMaterial *mat, std::vector<std::shared_ptr<const TextureResource>>& textures, const std::string& dir, aiTextureType ai_type) {
    for (uint32_t i = 0; i < mat->GetTextureCount(ai_type); ++i) {
        aiString str;
        mat->GetTexture(ai_type, i, &str);
        std::string path = dir + "/" + str.C_Str();
        textures.push_back(TextureResource::from_path(path));
    }
}

void MeshArrayResource::load_materials(const aiScene *scene) {
    if (!m_directory.has_value()) {
        std::cerr << "[Assimp]: Couldn't load textures, directory is not initialized.\n";
    }

    for (uint32_t i = 0; i < scene->mNumMaterials; ++i) {
        // Create new material
        MaterialResource material;

        // Get material handle
        aiMaterial *mat = scene->mMaterials[i];

        add_textures(mat, material.albedo_maps, *m_directory, aiTextureType_DIFFUSE);
        add_textures(mat, material.specular_maps, *m_directory, aiTextureType_SPECULAR);
        add_textures(mat, material.normal_maps, *m_directory, aiTextureType_NORMALS);

        material.name = std::string(mat->GetName().C_Str());
        m_materials.push_back(material);
    }
}

MeshArrayResource::MeshArrayResource(std::string_view path, glm::mat4 vert_multiplier)
: m_vert_multiplier(vert_multiplier), m_multiply(true) {
    m_vert_multiplier_inv = glm::inverse(glm::mat3(vert_multiplier));
    this->load_model(path);
}

std::shared_ptr<MeshArrayResource> MeshArrayResource::from_path(std::string_view path, glm::mat4 vert_multiplier) {
    auto* raw = new MeshArrayResource(path, vert_multiplier);

    return std::shared_ptr<MeshArrayResource>(raw);
}

MeshArrayResource::MeshArrayResource(std::string_view path) {
    this->load_model(path);
}

std::shared_ptr<MeshArrayResource> MeshArrayResource::from_path(std::string_view path) {
    auto* raw = new MeshArrayResource(path);
    return std::shared_ptr<MeshArrayResource>(raw);
}

void MeshArrayResource::process_node(aiNode *node, const aiScene *scene) {
    for (uint32_t i = 0; i < node->mNumMeshes; ++i) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        add_mesh(mesh, node);
    }

    for (uint32_t i = 0; i < node->mNumChildren; ++i) {
        process_node(node->mChildren[i], scene);
    }
}

void MeshArrayResource::add_mesh(aiMesh *mesh, const aiNode *curr_node) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    aiMatrix4x4 transform = curr_node->mTransformation;
    aiMatrix3x3 normal_mat = aiMatrix3x3(transform).Inverse().Transpose();

    // process vertices
    for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
        aiVector3D tvert = transform * mesh->mVertices[i];
        aiVector3D tnorm = normal_mat * mesh->mNormals[i];
        aiVector3D ttan = normal_mat * mesh->mTangents[i];
        aiVector3D tbitan = normal_mat * mesh->mBitangents[i];
        Vertex vertex(
            glm::vec3(tvert.x, tvert.y, tvert.z),
            glm::normalize(glm::vec3(ttan.x, ttan.y, ttan.z)),
            glm::normalize(glm::vec3(tbitan.x, tbitan.y, tbitan.z)),
            glm::normalize(glm::vec3(tnorm.x, tnorm.y, tnorm.z)),
            glm::vec2(0.f, 0.f)
        );

        if (mesh->mTextureCoords[0]) {
            vertex.tex_coords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }

        if (m_multiply) {
            vertex.position = glm::vec3(m_vert_multiplier * glm::vec4(vertex.position, 1.f));
            vertex.tangent = glm::vec3(glm::transpose(m_vert_multiplier_inv) * glm::vec4(vertex.tangent, 1.f));
            vertex.bitangent = glm::vec3(glm::transpose(m_vert_multiplier) * glm::vec4(vertex.bitangent, 1.f));
            vertex.normal = glm::vec3(glm::transpose(m_vert_multiplier) * glm::vec4(vertex.normal, 1.f));
        }

        vertices.push_back(vertex);
    }

    // process indices
    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    std::string name = curr_node->mName.C_Str();
    m_meshes.emplace_back(MeshResource::create(vertices, indices, name), mesh->mMaterialIndex);
}

void MeshArrayResource::load_model(std::string_view path) {
    Assimp::Importer importer;

    unsigned int flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace;

    size_t dotPos = path.find_last_of('.');
    if (dotPos != std::string_view::npos) {
        std::string_view extension = path.substr(dotPos + 1);

        if (extension == "obj") {
            flags |= aiProcess_FlipUVs;
        } else if (extension != "gltf") {
            std::cerr << "[Assimp] Error for path " << path << " -- File extension ." << extension << " is not supported." << std::endl;
        }
    } else {
        std::cerr << "[Assimp] Error for path " << path << " -- No file extension found." << std::endl;
    }

    const aiScene *scene = importer.ReadFile(
            path.data(),
            flags
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "[Assimp] Couldn't load " << path << ". Error: " << importer.GetErrorString() << std::endl;
        return;
    }

    m_directory.emplace(path.substr(0, path.find_last_of('/')));

    process_node(scene->mRootNode, scene);
    load_materials(scene);
}
