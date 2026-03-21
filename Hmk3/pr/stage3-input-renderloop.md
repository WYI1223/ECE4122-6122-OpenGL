# PR-HW3-03: Input Wiring And Render-Loop Skeleton

- Proposed title: `feat(main): wire GLFW input and complete the render-loop skeleton`
- Status: Draft

## Goal

Connect the camera to GLFW callbacks and fill in the non-scene-specific parts
of the render loop in `src/main.cpp`.

After this PR, the window should be interactive and should update view and
projection state every frame, but full scene composition and final lighting are
still handled by later PRs.

## Preconditions

- `PR-HW3-00` is complete
- `PR-HW3-01` is complete
- `PR-HW3-02` is complete

## Execution Contract (Canonical Inputs)

| Type | Reference | Relation to this PR |
|------|------|------|
| skeleton | `src/main.cpp` | Contains the callback, input, and render-loop TODOs for this PR |
| dependency | `src/camera.h` | Receives movement, mouse, scroll, and reset updates |
| dependency | `src/shader.h` | Supplies the linked object shader used in the render loop |

## Scope

In scope:
- implement `mouseButtonCallback()`
- implement `cursorPosCallback()`
- implement `scrollCallback()`
- implement `processInput()`
- call `processInput(window)` every frame
- clear the screen and depth buffer with the intended dusk-sky color
- build `view` and `projection` matrices each frame
- bind `objShader` and upload `viewPos` and `material.shininess`

Out of scope:
- implementing `setLightUniforms()`
- implementing `drawModel()`
- placing scene objects
- final terrain or lighting polish

## Design Constraints

- Preserve the current callback registration already present in `main.cpp`.
- Only rotate the camera while the right mouse button is held.
- Reset `g_firstMouse` on RMB press to avoid a view jump.
- Keep `Q/E` vertical motion and `R` reset because the skeleton already exposes them.
- Use the current projection contract: perspective, near `0.1`, far `500`.

## Task Breakdown

| Task | Lane | Description | Target | Depends on |
|------|------|------|------|------|
| T1 | impl | Implement RMB tracking, cursor delta forwarding, and scroll forwarding | `src/main.cpp` | PR-HW3-02 |
| T2 | impl | Implement keyboard polling for camera movement, reset, and exit | `src/main.cpp` | PR-HW3-02 |
| T3 | impl | Fill the per-frame render-loop skeleton through shader bind and per-frame uniform upload | `src/main.cpp` | PR-HW3-01 |

## Planned File Changes

- `[edit]` `src/main.cpp`

## Verification

### Build verification

```bash
cd F:/Learn/ECE4122-6122-OpenGL
cmake --build build_hmk3_stage0 --config Debug --target Hmk3_Skeleton
```

### Runtime verification

```bash
cd F:/Learn/ECE4122-6122-OpenGL/build/bin
./Hmk3_Skeleton.exe
```

Expected result:
- the window opens and clears to the intended dusk-sky color
- `ESC` closes the app
- RMB drag changes view direction
- scroll changes zoom/FOV
- `R` resets the camera without crashing

## Risks

| Risk | Why it matters | Mitigation |
|------|------|------|
| `g_firstMouse` is not reset on RMB press | The first drag frame produces a large jump | Reset it when RMB becomes active |
| Y delta is not inverted | Looking up/down feels reversed | Use `dy = lastY - ypos` as documented in the skeleton |
| Render loop mixes in later-stage responsibilities too early | Debugging gets harder | Stop at camera/input/per-frame state; leave lights and model drawing to later PRs |

## Acceptance Criteria

- [ ] All callback and input TODOs for this PR in `src/main.cpp` are implemented
- [ ] The per-frame clear/view/projection/shader-use skeleton is implemented
- [ ] The window responds to keyboard, mouse, and scroll input
- [ ] No scene-placement or light-upload responsibilities were pulled forward from later PRs
