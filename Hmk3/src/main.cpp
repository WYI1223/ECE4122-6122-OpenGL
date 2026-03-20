// ─────────────────────────────────────────────────────────────────────────────
// main.cpp  –  HW3: 3D World Scene with Assimp Model Loading  [SKELETON]
//
// ECE 4122 / 6122 – Advanced Programming Techniques
//
// OpenGL loader: GLEW  (no GLAD required)
//   GL/glew.h must be included before GLFW/glfw3.h.
//   Call glewInit() immediately after glfwMakeContextCurrent() – this is
//   already done for you in the provided initialization block below.
//
// What is provided:
//   • buildTerrain() – flat 100×100 ground quad as a raw VAO
//   • Frame timing
//   • Render-loop skeleton with swap/poll
//
// What you must implement (search for TODO):
//   1. processInput()        – keyboard camera movement + ESC + R
//   2. mouseButtonCallback() – track right-mouse-button state
//   3. cursorPosCallback()   – forward deltas to camera.processMouseMovement()
//   4. scrollCallback()      – forward to camera.processScroll()
//   5. setLightUniforms()    – upload directional + 2 point light uniforms
//   6. drawModel()           – set MVP + normalMatrix uniforms, call model.draw()
//   7. Render loop body      – clear, build matrices, draw all scene objects
//                              at the positions specified in the assignment
// ─────────────────────────────────────────────────────────────────────────────

// GL/glew.h MUST come before GLFW/glfw3.h
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <vector>

#include "camera.h"
#include "model.h"
#include "shader.h"

// ── Window dimensions ─────────────────────────────────────────────────────────
static const int SCR_W = 1280;
static const int SCR_H = 720;

// ── Global camera ─────────────────────────────────────────────────────────────
Camera g_camera({ 0.f, 5.f, 15.f });

// ── Mouse tracking state ──────────────────────────────────────────────────────
static float g_lastX      = SCR_W / 2.f;
static float g_lastY      = SCR_H / 2.f;
static bool  g_rmb        = false;   // true while right mouse button is held
static bool  g_firstMouse = true;    // prevents view jump on first RMB press

// ── Frame timing ──────────────────────────────────────────────────────────────
static float g_deltaTime = 0.f;
static float g_lastFrame = 0.f;

// ─────────────────────────────────────────────────────────────────────────────
// framebufferSizeCallback (provided)
// ─────────────────────────────────────────────────────────────────────────────
static void framebufferSizeCallback(GLFWwindow*, int w, int h)
{
    glViewport(0, 0, w, h);
}

// ─────────────────────────────────────────────────────────────────────────────
// mouseButtonCallback
// Set g_rmb = true when GLFW_MOUSE_BUTTON_RIGHT is pressed,
//         = false when it is released.
// Reset g_firstMouse = true on press so the view does not jump.
// TODO: Implement.
// ─────────────────────────────────────────────────────────────────────────────
static void mouseButtonCallback(GLFWwindow*, int button, int action, int /*mods*/)
{
    // TODO
    (void)button; (void)action;
}

// ─────────────────────────────────────────────────────────────────────────────
// cursorPosCallback
// Only process movement when g_rmb is true.
// On the first call after RMB press (g_firstMouse) record the position and
//   return without moving the camera.
// Compute dx = xpos - g_lastX and dy = g_lastY - ypos  (Y is inverted).
// Update g_lastX / g_lastY then call g_camera.processMouseMovement(dx, dy).
// TODO: Implement.
// ─────────────────────────────────────────────────────────────────────────────
static void cursorPosCallback(GLFWwindow*, double xpos, double ypos)
{
    // TODO
    (void)xpos; (void)ypos;
}

// ─────────────────────────────────────────────────────────────────────────────
// scrollCallback
// Forward the yOffset to g_camera.processScroll().
// TODO: Implement.
// ─────────────────────────────────────────────────────────────────────────────
static void scrollCallback(GLFWwindow*, double /*xo*/, double yo)
{
    // TODO
    (void)yo;
}

// ─────────────────────────────────────────────────────────────────────────────
// processInput  (called once per frame)
// Poll keys and drive the camera:
//   ESC        → glfwSetWindowShouldClose(window, true)
//   R          → g_camera.reset()
//   W/S/A/D    → CameraDir::FORWARD/BACKWARD/LEFT/RIGHT
//   Q/E        → CameraDir::DOWN/UP
// TODO: Implement.
// ─────────────────────────────────────────────────────────────────────────────
static void processInput(GLFWwindow* window)
{
    // TODO
    (void)window;
}

// ─────────────────────────────────────────────────────────────────────────────
// buildTerrain (provided)
// Creates a 100×100 flat quad at Y = -0.001 as a raw VAO.
// ─────────────────────────────────────────────────────────────────────────────
static GLuint g_terrainVAO { 0 };
static GLuint g_terrainVBO { 0 };
static GLuint g_terrainEBO { 0 };
static int    g_terrainIndexCount { 0 };

static void buildTerrain()
{
    const float Y    = -0.001f;
    const float HALF = 50.f;
    const float TILE = 20.f;

    float verts[] = {
        -HALF, Y, -HALF,  0.f, 1.f, 0.f,  0.f,  TILE,
         HALF, Y, -HALF,  0.f, 1.f, 0.f,  TILE, TILE,
         HALF, Y,  HALF,  0.f, 1.f, 0.f,  TILE, 0.f,
        -HALF, Y,  HALF,  0.f, 1.f, 0.f,  0.f,  0.f,
    };
    unsigned idx[] = { 0, 1, 2,  2, 3, 0 };
    g_terrainIndexCount = 6;

    glGenVertexArrays(1, &g_terrainVAO);
    glGenBuffers(1, &g_terrainVBO);
    glGenBuffers(1, &g_terrainEBO);
    glBindVertexArray(g_terrainVAO);

    glBindBuffer(GL_ARRAY_BUFFER, g_terrainVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_terrainEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

    const int stride = 8 * sizeof(float);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

    glBindVertexArray(0);
}

// ─────────────────────────────────────────────────────────────────────────────
// setLightUniforms
// Upload all light parameters to the currently-bound shader.
//
// Directional light (late-afternoon sun):
//   direction: (-0.5, -1.0, -0.5)     diffuse: (1.0, 0.8, 0.6)  warm orange-yellow
//   Set sensible ambient (≈ 30% of diffuse) and specular values.
//
// Point light 0 – street lamp at world pos (5, 0, 0), lamp head ≈ Y=4:
//   position: (5, 4, 0)   diffuse: (0.9, 0.9, 1.0)  cool white
//   Attenuation: constant=1.0, linear=0.09, quadratic=0.032
//
// Point light 1 – street lamp at world pos (-5, 0, 5):
//   position: (-5, 4, 5)  diffuse: (0.9, 0.9, 1.0)  cool white
//   Same attenuation as point light 0.
//
// Uniform naming convention used by the fragment shader:
//   dirLight.direction / .ambient / .diffuse / .specular
//   pointLights[i].position / .ambient / .diffuse / .specular
//   pointLights[i].constant / .linear / .quadratic
//
// TODO: Implement.
// ─────────────────────────────────────────────────────────────────────────────
static void setLightUniforms(Shader& sh)
{
    // TODO – directional light

    // TODO – point light 0

    // TODO – point light 1

    (void)sh;
}

// ─────────────────────────────────────────────────────────────────────────────
// drawModel
// Upload the model, view, and projection matrices and the normal matrix to
// the shader, then call model.draw(shader).
//
// Normal matrix = transpose(inverse(mat3(model)))
// Pack it into a mat4 for the mat4 uniform in the vertex shader.
//
// TODO: Implement.
// ─────────────────────────────────────────────────────────────────────────────
static void drawModel(Model& model, Shader& sh,
                      const glm::mat4& modelMat,
                      const glm::mat4& view,
                      const glm::mat4& proj)
{
    // TODO
    (void)model; (void)sh; (void)modelMat; (void)view; (void)proj;
}

// ─────────────────────────────────────────────────────────────────────────────
// main
// ─────────────────────────────────────────────────────────────────────────────
int main()
{
    // ── GLFW init (provided) ──────────────────────────────────────────────────
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(SCR_W, SCR_H,
                                          "HW3 – 3D Rural Scene", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // ── GLEW init (provided) ──────────────────────────────────────────────────
    // glewExperimental enables core-profile extension pointers (VAOs, etc.).
    // Must be called after glfwMakeContextCurrent().
    // glGetError() clears the benign GL_INVALID_ENUM that glewInit may emit.
    glewExperimental = GL_TRUE;
    GLenum glewErr = glewInit();
    if (glewErr != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: "
                  << glewGetErrorString(glewErr) << "\n";
        glfwTerminate();
        return -1;
    }
    glGetError(); // clear benign error

    std::cout << "OpenGL " << glGetString(GL_VERSION)
              << "  Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout << "GLEW "   << glewGetString(GLEW_VERSION)  << "\n";

    // ── Register callbacks (provided – calls your implementations above) ──────
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    // ── Shaders ───────────────────────────────────────────────────────────────
    Shader objShader("shaders/object.vert", "shaders/object.frag");

    // ── Model loading ─────────────────────────────────────────────────────────
    // Place each OBJ (with .mtl and textures) in assets/models/<n>/

//    Model mFarmhouse("./assets/models/farmhouse/Farm_house.obj");
//    Model mBarrel   ("./assets/models/barrel/Barrel_OBJ.obj");
//    Model mTree     ("./assets/models/pine_tree/Pine_Tree.obj");
//    Model mBench    ("./assets/models/bench/wooden_bench.obj");
    Model mLamp     ("./assets/models/lamp/objLamp.obj");
    Model mRobot    ("./assets/models/robot/Robot.obj");

    // ── Terrain quad ──────────────────────────────────────────────────────────
    buildTerrain();

    // ── Render loop ───────────────────────────────────────────────────────────
    while (!glfwWindowShouldClose(window))
    {
        // Delta time (provided)
        float now   = (float)glfwGetTime();
        g_deltaTime = now - g_lastFrame;
        g_lastFrame = now;

        // TODO: call processInput(window)

        // ── Clear
        // TODO: Set a dusk sky clear color and clear COLOR + DEPTH buffers

        // ── Build view + projection matrices
        // TODO: glm::mat4 view = g_camera.getViewMatrix();
        // TODO: glm::mat4 proj = glm::perspective(…)  near=0.1, far=500

        // ── Bind shader, upload per-frame constants
        // TODO: objShader.use();
        // TODO: upload viewPos and material.shininess
        // TODO: call setLightUniforms(objShader)

        // ── Draw terrain
        // TODO: bind g_terrainVAO and draw the 6 terrain indices

        // ── Draw scene objects
        // Use drawModel() with glm::translate / glm::rotate / glm::scale
        // to place each object at its required world-space position.
        //
        // Required placements (from assignment spec):
        //   Farmhouse        (0,  0, -10)   rot 0° y,    scale 1
        //   Barrel 1         (3.5,0, -8)    no rotation
        //   Barrel 2         (4.5,0, -8)    rot 15° y
        //   Pine Tree × 4   (±12,0, ±12)   random 0–45° y each
        //   Park Bench       (-6, 0,  2)    rot -30° y
        //   Street Lamp × 2  (5,0,0) (-5,0,5)  scale 0.5
        //   Robot            (-3,0, -4)     rot 180° y (facing camera)

        // TODO: add drawModel calls for each object above

        // ── Swap + poll (provided)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ── Cleanup (provided) ────────────────────────────────────────────────────
    glDeleteVertexArrays(1, &g_terrainVAO);
    glDeleteBuffers(1, &g_terrainVBO);
    glDeleteBuffers(1, &g_terrainEBO);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
