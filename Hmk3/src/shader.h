#pragma once
// ─────────────────────────────────────────────────────────────────────────────
// shader.h  –  GLSL program wrapper
//
// Uses GL/glew.h (GLEW) as the OpenGL function loader.
// GL/glew.h must be included before any other OpenGL header.
// glewInit() is called in main.cpp after the GLFW context is made current.
//
// TODO (students): Implement the constructor (compile + link) and the two
//   private helper methods: compile() and checkLink().
//   The uniform setters and file reader are provided for you.
// ─────────────────────────────────────────────────────────────────────────────
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class Shader
{
public:
    GLuint id { 0 };

    // ── Constructor ───────────────────────────────────────────────────────────
    // 1. Read vertex shader source from vertPath (use readFile below).
    // 2. Read fragment shader source from fragPath.
    // 3. Compile each source with compile() → returns a shader object handle.
    // 4. Create a program with glCreateProgram().
    // 5. Attach both shader objects (glAttachShader).
    // 6. Link the program (glLinkProgram) and check for errors (checkLink).
    // 7. Delete the individual shader objects (glDeleteShader) – they are no
    //    longer needed once linked into the program.
    // TODO: Implement.
    Shader(const char* vertPath, const char* fragPath)
    {
        // TODO
        (void)vertPath; (void)fragPath;
    }

    void use() const { glUseProgram(id); }

    // ── Uniform setters (provided – do not modify) ────────────────────────────
    void setBool (const std::string& n, bool  v) const { glUniform1i (loc(n), (int)v); }
    void setInt  (const std::string& n, int   v) const { glUniform1i (loc(n), v); }
    void setFloat(const std::string& n, float v) const { glUniform1f (loc(n), v); }

    void setVec3(const std::string& n, const glm::vec3& v) const
        { glUniform3fv(loc(n), 1, glm::value_ptr(v)); }

    void setVec3(const std::string& n, float x, float y, float z) const
        { glUniform3f(loc(n), x, y, z); }

    void setMat4(const std::string& n, const glm::mat4& m) const
        { glUniformMatrix4fv(loc(n), 1, GL_FALSE, glm::value_ptr(m)); }

private:
    GLint loc(const std::string& n) const
        { return glGetUniformLocation(id, n.c_str()); }

    // ── readFile (provided) ───────────────────────────────────────────────────
    static std::string readFile(const char* path)
    {
        std::ifstream f(path);
        if (!f.is_open()) {
            std::cerr << "[Shader] Cannot open: " << path << "\n";
            return {};
        }
        std::ostringstream ss;
        ss << f.rdbuf();
        return ss.str();
    }

    // ── compile ───────────────────────────────────────────────────────────────
    // Create a shader of the given 'type' (GL_VERTEX_SHADER or
    // GL_FRAGMENT_SHADER), upload 'src', compile it, and check for errors
    // via glGetShaderiv(GL_COMPILE_STATUS).  Print any info log on failure.
    // Return the shader object handle.
    // TODO: Implement.
    static GLuint compile(GLenum type, const char* src, const char* label)
    {
        // TODO
        (void)type; (void)src; (void)label;
        return 0;
    }

    // ── checkLink ─────────────────────────────────────────────────────────────
    // Query GL_LINK_STATUS on 'prog'.  On failure, retrieve and print the
    // info log via glGetProgramInfoLog.
    // TODO: Implement.
    static void checkLink(GLuint prog)
    {
        // TODO
        (void)prog;
    }
};
