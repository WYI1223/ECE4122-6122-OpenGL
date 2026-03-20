#pragma once
// ─────────────────────────────────────────────────────────────────────────────
// mesh.h  –  Single GPU-resident mesh (VAO / VBO / EBO)
//
// Uses GL/glew.h (GLEW) as the OpenGL function loader.
//
// The Vertex struct, Texture struct, and class interface are fully defined.
// You must implement:
//   • setupGPU() – allocate and populate the VAO, VBO, and EBO
//   • draw()     – bind textures, upload their uniform indices, draw
//
// TODO sections are marked below.
// ─────────────────────────────────────────────────────────────────────────────
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "shader.h"

// ── Vertex layout ─────────────────────────────────────────────────────────────
// Interleaved attributes sent to the vertex shader at locations 0, 1, 2.
struct Vertex {
    glm::vec3 position;    // location 0
    glm::vec3 normal;      // location 1
    glm::vec2 texCoords;   // location 2
};

// ── Texture descriptor ────────────────────────────────────────────────────────
struct Texture {
    GLuint      id   { 0 };
    std::string type;   // "texture_diffuse" | "texture_specular" | "texture_emissive"
    std::string path;   // used as deduplication key in Model
};

// ── Mesh class ────────────────────────────────────────────────────────────────
class Mesh
{
public:
    std::vector<Vertex>  vertices;
    std::vector<GLuint>  indices;
    std::vector<Texture> textures;

    // Constructor: store data and call setupGPU()
    Mesh(std::vector<Vertex>  v,
         std::vector<GLuint>  i,
         std::vector<Texture> t)
        : vertices(std::move(v))
        , indices (std::move(i))
        , textures(std::move(t))
    {
        setupGPU();
    }

    // Disable copy; allow move so std::vector<Mesh> is safe
    Mesh(const Mesh&)            = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&&)                 = default;
    Mesh& operator=(Mesh&&)      = default;

    ~Mesh()
    {
        // Provided: clean up GPU resources
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ebo);
    }

    // ── draw ──────────────────────────────────────────────────────────────────
    // For each texture in 'textures':
    //   1. Activate GL_TEXTURE0 + i.
    //   2. Build the uniform name:
    //        "texture_diffuse"  → "material.diffuse0",  "material.diffuse1", …
    //        "texture_specular" → "material.specular0", …
    //        "texture_emissive" → "material.emissive0", …
    //      (use separate counters for each type)
    //   3. Call shader.setInt(name, i) and glBindTexture(GL_TEXTURE_2D, id).
    // Then:
    //   4. Set the has-flags:
    //        shader.setInt("material.hasDiffuse",  diffuseCount  > 0 ? 1 : 0);
    //        shader.setInt("material.hasSpecular", specularCount > 0 ? 1 : 0);
    //        shader.setInt("material.hasEmissive", emissiveCount > 0 ? 1 : 0);
    //   5. Bind m_vao, call glDrawElements(GL_TRIANGLES, …), unbind.
    //   6. Reset to GL_TEXTURE0 to avoid state leakage.
    // TODO: Implement.
    void draw(Shader& shader) const
    {
        // TODO
        (void)shader;
    }

private:
    GLuint m_vao { 0 };
    GLuint m_vbo { 0 };
    GLuint m_ebo { 0 };

    // ── setupGPU ──────────────────────────────────────────────────────────────
    // 1. Generate one VAO, one VBO, one EBO (glGenVertexArrays / glGenBuffers).
    // 2. Bind the VAO.
    // 3. Upload vertices to the VBO with GL_STATIC_DRAW.
    //      Size = vertices.size() * sizeof(Vertex)
    // 4. Upload indices to the EBO with GL_STATIC_DRAW.
    //      Size = indices.size()  * sizeof(GLuint)
    // 5. Describe the three vertex attributes via glVertexAttribPointer:
    //      Location 0 – position  (3 floats, offset = offsetof(Vertex, position))
    //      Location 1 – normal    (3 floats, offset = offsetof(Vertex, normal))
    //      Location 2 – texCoords (2 floats, offset = offsetof(Vertex, texCoords))
    //    Stride = sizeof(Vertex) for all three.
    // 6. Enable each attribute (glEnableVertexAttribArray).
    // 7. Unbind the VAO.
    // TODO: Implement.
    void setupGPU()
    {
        // TODO
    }
};
