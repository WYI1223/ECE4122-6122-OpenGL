#pragma once
// ─────────────────────────────────────────────────────────────────────────────
// model.h  –  Assimp-based model loader
//
// Uses GL/glew.h (GLEW) as the OpenGL function loader.
// stb_image.h must be present in src/ for texture decoding.
//
// The top-level loading logic (load, processNode) and texture upload helpers
// (loadTextureFromFile, createFallbackTexture) are provided.
//
// You must implement:
//   • processMesh()            – convert an aiMesh to our Mesh class
//   • loadMaterialTextures()   – extract texture paths from an aiMaterial
//                                and call loadTextureFromFile()
//
// TODO sections are marked below.
// ─────────────────────────────────────────────────────────────────────────────
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// STB_IMAGE_IMPLEMENTATION must be defined in exactly one translation unit.
// model.h is included only from main.cpp, so this is safe here.
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "mesh.h"
#include "shader.h"

namespace fs = std::filesystem;

class Model
{
public:
    // ── Constructor (provided) ────────────────────────────────────────────────
    explicit Model(const std::string& path)
    {
        m_directory = fs::path(path).parent_path().string();
        load(path);
    }

    // ── draw (provided) ───────────────────────────────────────────────────────
    void draw(Shader& shader) const
    {
        for (const auto& mesh : m_meshes)
            mesh.draw(shader);
    }

    bool isLoaded() const { return m_loaded; }

private:
    std::vector<Mesh>                        m_meshes;
    std::string                              m_directory;
    std::unordered_map<std::string, Texture> m_texCache;  // path → Texture
    bool                                     m_loaded { false };

    // ── load (provided) ───────────────────────────────────────────────────────
    // Sets up Assimp import flags and calls processNode on the root.
    void load(const std::string& path)
    {
        Assimp::Importer importer;

        // Required import flags (per assignment spec):
        //   aiProcess_Triangulate     – all faces become triangles
        //   aiProcess_FlipUVs         – flip UV.y for OpenGL bottom-left origin
        //   aiProcess_CalcTangentSpace – tangent vectors for future normal maps
        //   aiProcess_GenNormals      – auto-generate normals if not present
        unsigned flags = aiProcess_Triangulate
                       | aiProcess_FlipUVs
                       | aiProcess_CalcTangentSpace
                       | aiProcess_GenNormals
                       | aiProcess_JoinIdenticalVertices;

        const aiScene* scene = importer.ReadFile(path, flags);

        if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode) {
            std::cerr << "[Model] Assimp error loading \"" << path
                      << "\": " << importer.GetErrorString() << "\n";
            return;
        }

        processNode(scene->mRootNode, scene);
        m_loaded = true;
        std::cout << "[Model] Loaded: " << path
                  << "  (" << m_meshes.size() << " meshes)\n";
    }

    // ── processNode (provided) ────────────────────────────────────────────────
    // Recursively walks the scene graph; converts each mesh node.
    void processNode(aiNode* node, const aiScene* scene)
    {
        for (unsigned i = 0; i < node->mNumMeshes; ++i)
            m_meshes.push_back(processMesh(scene->mMeshes[node->mMeshes[i]], scene));

        for (unsigned i = 0; i < node->mNumChildren; ++i)
            processNode(node->mChildren[i], scene);
    }

    // ── processMesh ───────────────────────────────────────────────────────────
    // Convert an aiMesh to our Mesh type.  Steps:
    //
    // Vertices – loop mesh->mNumVertices:
    //   v.position  = { mesh->mVertices[i].x/y/z }
    //   v.normal    = { mesh->mNormals[i].x/y/z }  (check mesh->HasNormals())
    //   v.texCoords = { mesh->mTextureCoords[0][i].x/y }
    //                 if mesh->mTextureCoords[0] is non-null, else vec2(0,0)
    //
    // Indices – loop mesh->mNumFaces; for each face loop face.mNumIndices:
    //   push each face.mIndices[j] into the indices vector.
    //   (aiProcess_Triangulate guarantees every face has exactly 3 indices.)
    //
    // Textures – if mesh->mMaterialIndex >= 0:
    //   aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
    //   call loadMaterialTextures for aiTextureType_DIFFUSE  → "texture_diffuse"
    //   call loadMaterialTextures for aiTextureType_SPECULAR → "texture_specular"
    //   call loadMaterialTextures for aiTextureType_EMISSIVE → "texture_emissive"
    //   append all three result vectors into textures.
    //
    // Return Mesh(vertices, indices, textures).
    // TODO: Implement.
    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex>  vertices;
        std::vector<GLuint>  indices;
        std::vector<Texture> textures;

        vertices.reserve(mesh->mNumVertices);
        for (unsigned i = 0; i < mesh->mNumVertices; ++i) {
            Vertex v {};
            v.position = {
                mesh->mVertices[i].x,
                mesh->mVertices[i].y,
                mesh->mVertices[i].z
            };

            if (mesh->HasNormals() && mesh->mNormals) {
                v.normal = {
                    mesh->mNormals[i].x,
                    mesh->mNormals[i].y,
                    mesh->mNormals[i].z
                };
            } else {
                v.normal = glm::vec3(0.0f);
            }

            if (mesh->mTextureCoords[0]) {
                v.texCoords = {
                    mesh->mTextureCoords[0][i].x,
                    mesh->mTextureCoords[0][i].y
                };
            } else {
                v.texCoords = glm::vec2(0.0f);
            }

            vertices.push_back(v);
        }

        for (unsigned i = 0; i < mesh->mNumFaces; ++i) {
            const aiFace& face = mesh->mFaces[i];
            for (unsigned j = 0; j < face.mNumIndices; ++j)
                indices.push_back(face.mIndices[j]);
        }

        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

            auto diffuseMaps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            auto specularMaps = loadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

            auto emissiveMaps = loadMaterialTextures(mat, aiTextureType_EMISSIVE, "texture_emissive");
            textures.insert(textures.end(), emissiveMaps.begin(), emissiveMaps.end());
        }

        return Mesh(std::move(vertices), std::move(indices), std::move(textures));
    }

    // ── loadMaterialTextures ──────────────────────────────────────────────────
    // Retrieve all textures of 'type' from 'mat'.  For each one:
    //   1. Call mat->GetTexture(type, i, &aiPath) to get the path string.
    //   2. Extract just the filename (strip any directory prefix in the path)
    //      and build fullPath = m_directory + "/" + filename.
    //      Hint: fs::path(aiPath.C_Str()).filename().string()
    //   3. Check m_texCache for fullPath:
    //        – hit:  push the cached Texture and continue.
    //        – miss: call loadTextureFromFile(fullPath) to get a GL texture ID,
    //                build a Texture{id, typeName, fullPath}, store in cache,
    //                then push it.
    // Return the vector of Texture objects.
    // TODO: Implement.
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat,
                                              aiTextureType type,
                                              const std::string& typeName)
    {
        std::vector<Texture> out;

        const unsigned texCount = mat->GetTextureCount(type);
        out.reserve(texCount);

        for (unsigned i = 0; i < texCount; ++i) {
            aiString aiPath;
            if (mat->GetTexture(type, i, &aiPath) != AI_SUCCESS)
                continue;

            const fs::path materialPath = fs::path(aiPath.C_Str()).lexically_normal();
            fs::path resolvedPath = (fs::path(m_directory) / materialPath).lexically_normal();

            // Some exported MTL files keep a useful relative subdirectory
            // (for example "textures/black.jpg"), while others may contain
            // stale directory prefixes. Prefer the full relative path first,
            // then fall back to filename-only normalization.
            if (!fs::exists(resolvedPath))
                resolvedPath = (fs::path(m_directory) / materialPath.filename()).lexically_normal();

            // Some Assimp importers collapse the relative directory and leave
            // only the filename. Search under the model root as a last resort.
            if (!fs::exists(resolvedPath) && fs::exists(m_directory)) {
                for (const auto& entry : fs::recursive_directory_iterator(m_directory)) {
                    if (!entry.is_regular_file())
                        continue;
                    if (entry.path().filename() == materialPath.filename()) {
                        resolvedPath = entry.path().lexically_normal();
                        break;
                    }
                }
            }

            const std::string fullPath = resolvedPath.string();

            auto it = m_texCache.find(fullPath);
            if (it != m_texCache.end()) {
                out.push_back(it->second);
                continue;
            }

            Texture tex;
            tex.id = loadTextureFromFile(fullPath);
            tex.type = typeName;
            tex.path = fullPath;

            m_texCache[fullPath] = tex;
            out.push_back(tex);
        }

        return out;
    }

    // ── loadTextureFromFile (provided) ────────────────────────────────────────
    // Uploads one image to the GPU and returns its GL texture ID.
    // Falls back to a 1×1 gray texture if the file is not found.
    static GLuint loadTextureFromFile(const std::string& path)
    {
        if (!fs::exists(path)) {
            std::cerr << "[Model] Texture not found: " << path
                      << "  (using gray fallback)\n";
            return createFallbackTexture();
        }

        int w, h, channels;
        stbi_set_flip_vertically_on_load(false); // FlipUVs already handled by Assimp
        unsigned char* data = stbi_load(path.c_str(), &w, &h, &channels, 0);

        if (!data) {
            std::cerr << "[Model] stb_image failed: " << path << "\n";
            return createFallbackTexture();
        }

        GLenum fmt = GL_RGB;
        if      (channels == 1) fmt = GL_RED;
        else if (channels == 4) fmt = GL_RGBA;

        GLuint id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, (GLint)fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        return id;
    }

    // ── createFallbackTexture (provided) ─────────────────────────────────────
    static GLuint createFallbackTexture()
    {
        GLuint id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        unsigned char gray[3] = { 200, 200, 200 };
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, gray);
        glGenerateMipmap(GL_TEXTURE_2D);
        return id;
    }
};
