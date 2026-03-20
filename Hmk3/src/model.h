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

        // TODO: fill vertices, indices, textures

        (void)mesh; (void)scene; // remove when implemented
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

        // TODO
        (void)mat; (void)type; (void)typeName;
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
