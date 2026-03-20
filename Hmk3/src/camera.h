#pragma once
// ─────────────────────────────────────────────────────────────────────────────
// camera.h  –  Free-look FPS camera
//
// TODO (students): Implement all methods marked TODO below.
//
// Controls to support:
//   W / A / S / D        – forward / left / backward / right
//   Q / E                – down / up along world Y
//   Right Mouse + drag   – yaw / pitch (FPS style)
//   Scroll wheel         – zoom FOV (clamp to 1°–90°)
//   R                    – reset to position (0, 5, 15) looking at origin
// ─────────────────────────────────────────────────────────────────────────────
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

// Direction enum used by processKeyboard()
enum class CameraDir { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

class Camera
{
public:
    // ── Public state (read by main.cpp for projection + light upload) ─────────
    glm::vec3 position;
    glm::vec3 worldUp { 0.f, 1.f, 0.f };

    float yaw    { -90.f };   // degrees; -90 points toward -Z on startup
    float pitch  {   0.f };   // degrees

    float speed     { 5.f };   // world units per second
    float mouseSens { 0.08f };
    float fov       { 45.f };  // vertical FOV in degrees

    // Derived basis vectors – kept up-to-date by updateVectors()
    glm::vec3 front { 0.f, 0.f, -1.f };
    glm::vec3 right { 1.f, 0.f,  0.f };
    glm::vec3 up    { 0.f, 1.f,  0.f };

    // ── Constructor ───────────────────────────────────────────────────────────
    // TODO: Initialize position from parameter, then call updateVectors().
    Camera(glm::vec3 pos = { 0.f, 5.f, 15.f })
        : position(pos)
    {
        // TODO: call updateVectors() here
    }

    // ── View matrix ──────────────────────────────────────────────────────────
    // TODO: Return glm::lookAt(position, position + front, up)
    glm::mat4 getViewMatrix() const
    {
        return glm::mat4(1.f); // placeholder
    }

    // ── Keyboard movement ─────────────────────────────────────────────────────
    // Move the camera in the requested direction by (speed * dt) world units.
    // FORWARD/BACKWARD move along 'front'; LEFT/RIGHT along 'right';
    // UP/DOWN along 'worldUp'.
    // TODO: Implement the six movement directions.
    void processKeyboard(CameraDir dir, float dt)
    {
        float dist = speed * dt;
        // TODO: switch on dir and update position
        (void)dist; // remove when implemented
    }

    // ── Mouse look ────────────────────────────────────────────────────────────
    // dx: horizontal pixel delta (positive = look right)
    // dy: vertical pixel delta   (positive = look up, caller already inverts Y)
    // Clamp pitch to [-89°, 89°] to prevent gimbal flip.
    // TODO: scale deltas by mouseSens, update yaw/pitch, clamp pitch,
    //       then call updateVectors().
    void processMouseMovement(float dx, float dy, bool constrainPitch = true)
    {
        // TODO
        (void)dx; (void)dy; (void)constrainPitch;
    }

    // ── Scroll zoom ───────────────────────────────────────────────────────────
    // yOffset is positive for scroll-up (zoom in) and negative for scroll-down.
    // Subtract from fov and clamp to [1°, 90°].
    // TODO: Implement.
    void processScroll(float yOffset)
    {
        // TODO
        (void)yOffset;
    }

    // ── Reset ─────────────────────────────────────────────────────────────────
    // Restore the camera to its initial state: position (0,5,15), yaw -90°,
    // pitch 0°, fov 45°.  Call updateVectors() at the end.
    // TODO: Implement.
    void reset()
    {
        // TODO
    }

private:
    // ── updateVectors ─────────────────────────────────────────────────────────
    // Recompute 'front', 'right', and 'up' from current yaw and pitch.
    //
    // Hint – front vector components:
    //   front.x = cos(radians(yaw)) * cos(radians(pitch))
    //   front.y = sin(radians(pitch))
    //   front.z = sin(radians(yaw)) * cos(radians(pitch))
    // Normalize front, then derive right = cross(front, worldUp),
    //   and up = cross(right, front).  Normalize both.
    // TODO: Implement.
    void updateVectors()
    {
        // TODO
    }
};
