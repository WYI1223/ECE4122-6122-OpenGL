# PR-HW3-08: Light Uniform Upload And Lit Scene Path

- Proposed title: `feat(main): upload scene lights and complete the lit render path`
- Status: Draft

## Goal

Implement the C++ side of the lighting pipeline in `src/main.cpp` so the scene
actually receives the directional light and both point lights expected by the
shader.

This PR closes the loop between:
- the fragment-lighting math in `PR-HW3-07`
- the scene placement and draw path in `PR-HW3-06`

## Preconditions

- `PR-HW3-00` is complete
- `PR-HW3-03` is complete
- `PR-HW3-06` is complete
- `PR-HW3-07` is complete

## Execution Contract (Canonical Inputs)

| Type | Reference | Relation to this PR |
|------|------|------|
| skeleton | `src/main.cpp` | Contains `setLightUniforms()` and the remaining lit-render TODOs |
| shader contract | `shaders/object.frag` | Defines `dirLight`, `pointLights[i]`, `material.shininess`, and `viewPos` |
| scene path | `src/main.cpp` | Already owns `objShader.use()`, terrain draw, and object draw calls |

## Scope

In scope:
- implement `setLightUniforms()`
- upload all directional-light fields
- upload all point-light fields for both lamps
- ensure the render loop uses the lit shader path consistently
- confirm the scene shows visible diffuse/specular response

Out of scope:
- rewriting the light struct layout
- adding extra-credit lighting features such as shadows or fog
- final submission README and packaging

## Design Constraints

- Preserve the current uniform names exactly: `dirLight.*`, `pointLights[i].*`, `material.shininess`, and `viewPos`.
- Follow the existing scene notes in `main.cpp` for light positions and colors:
  - one warm late-afternoon directional light
  - two cool white lamp point lights
- Keep the attenuation values aligned with the skeleton comments.
- Do not rename shader-side structs or repurpose light slots.

## Task Breakdown

| Task | Lane | Description | Target | Depends on |
|------|------|------|------|------|
| T1 | impl | Implement `setLightUniforms()` for the directional light and both point lights | `src/main.cpp` | PR-HW3-07 |
| T2 | impl | Ensure the render loop uploads `viewPos`, `material.shininess`, and light uniforms before drawing | `src/main.cpp` | PR-HW3-03, PR-HW3-06 |

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
- the scene is visibly lit instead of flat black
- the directional light affects the whole scene
- both lamp point lights brighten nearby geometry
- specular highlights move as the camera moves

## Risks

| Risk | Why it matters | Mitigation |
|------|------|------|
| Uniform names do not match the shader exactly | The scene renders dark even though the code compiles | Treat the current shader names as the source of truth |
| Point-light positions do not match the placed lamp models | The lighting looks disconnected from the scene | Keep lamp world transforms and point-light positions aligned |
| `viewPos` is not uploaded every frame | Specular highlights break or appear frozen | Upload camera position as part of the per-frame path |

## Acceptance Criteria

- [ ] `setLightUniforms()` is implemented in `src/main.cpp`
- [ ] The render loop uploads light uniforms before drawing terrain and models
- [ ] The scene shows visible diffuse and specular lighting response
- [ ] Both point lights and the directional light are active at runtime
