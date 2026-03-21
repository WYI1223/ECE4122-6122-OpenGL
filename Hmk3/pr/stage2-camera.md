# PR-HW3-02: Camera Math And Movement

- Proposed title: `feat(camera): implement free-look camera math and controls`
- Status: Draft

## Goal

Implement the camera methods in `src/camera.h` so the skeleton has a working
FPS-style camera model before GLFW callbacks are wired in `main.cpp`.

This PR is intentionally limited to camera math and state transitions. It does
not own GLFW callback plumbing.

## Preconditions

`PR-HW3-00` is complete.

## Execution Contract (Canonical Inputs)

| Type | Reference | Relation to this PR |
|------|------|------|
| skeleton | `src/camera.h` | Contains all camera TODOs for this PR |
| consumer | `src/main.cpp` | Uses `g_camera`, `getViewMatrix()`, and movement methods |
| math contract | PDF camera requirements + current skeleton | Requires keyboard motion, mouse look, scroll zoom, and reset behavior |

## Scope

In scope:
- implement `updateVectors()`
- implement the constructor initialization path
- implement `getViewMatrix()`
- implement `processKeyboard()`
- implement `processMouseMovement()`
- implement `processScroll()`
- implement `reset()`

Out of scope:
- GLFW callback registration and input polling in `main.cpp`
- any change to public camera fields used by the rest of the skeleton

## Design Constraints

- Preserve the existing `Camera` data layout and method signatures.
- Keep the startup camera at `(0, 5, 15)` with `yaw = -90`, `pitch = 0`, `fov = 45`.
- Support the skeleton's full control set: `W/A/S/D`, `Q/E`, RMB drag, scroll, and `R`.
- Clamp pitch to avoid camera flips.
- Clamp FOV to a safe range for later projection-matrix creation.

## Task Breakdown

| Task | Lane | Description | Target | Depends on |
|------|------|------|------|------|
| T1 | impl | Implement vector recomputation from yaw/pitch and normalize the basis vectors | `src/camera.h` | none |
| T2 | impl | Implement view matrix, keyboard motion, mouse look, scroll zoom, and reset | `src/camera.h` | T1 |

## Planned File Changes

- `[edit]` `src/camera.h`

## Verification

### Build verification

```bash
cd F:/Learn/ECE4122-6122-OpenGL
cmake --build build --config Debug --target Hmk3_Skeleton
```

### Structural verification

Expected logical results after implementation:
- default `front` is approximately `(0, 0, -1)`
- `getViewMatrix()` no longer returns the identity matrix
- `reset()` restores `(0, 5, 15)`, `yaw = -90`, `pitch = 0`, `fov = 45`
- `processScroll()` keeps `fov` within the intended clamp range

## Risks

| Risk | Why it matters | Mitigation |
|------|------|------|
| Wrong front-vector math | All later movement and lighting-view behavior becomes confusing | Match the documented yaw/pitch equations exactly |
| Missing pitch clamp | Mouse look can flip or gimbal unexpectedly | Clamp pitch before recomputing vectors |
| Moving along `up` instead of `worldUp` | Vertical motion becomes camera-tilt dependent | Keep `Q/E` mapped to `worldUp` as the skeleton expects |

## Acceptance Criteria

- [ ] All camera TODOs in `src/camera.h` are implemented
- [ ] The camera basis vectors update correctly from yaw/pitch
- [ ] View-matrix generation uses `glm::lookAt`
- [ ] Movement, zoom, and reset behavior match the current skeleton contract
