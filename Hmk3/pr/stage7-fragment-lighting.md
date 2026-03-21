# PR-HW3-07: Fragment Lighting Functions

- Proposed title: `feat(lighting): implement directional and point-light shading in the fragment shader`
- Status: Draft

## Goal

Implement the missing lighting math in `shaders/object.frag` so the fragment
shader can compute:
- directional-light ambient, diffuse, and specular response
- point-light ambient, diffuse, and specular response
- attenuation for point lights

This PR only owns shader-side lighting math. It does not upload light uniforms.

## Preconditions

- `PR-HW3-00` is complete
- `PR-HW3-01` is complete

## Execution Contract (Canonical Inputs)

| Type | Reference | Relation to this PR |
|------|------|------|
| skeleton | `shaders/object.frag` | Contains `calcDirLight()` and `calcPointLight()` TODOs |
| shader interface | `shaders/object.vert` | Supplies `FragPos`, `Normal`, and `TexCoords` |
| runtime uploader | `src/main.cpp` | Later uploads `dirLight`, `pointLights[i]`, and `viewPos` |

## Scope

In scope:
- implement `calcDirLight()`
- implement `calcPointLight()`
- use the existing `getDiffuseColor()` and `getSpecularColor()` helpers
- preserve the current material and light struct layout

Out of scope:
- adding or renaming uniforms
- changing light colors or positions in C++
- emissive contribution logic already present in `main()`

## Design Constraints

- Match the current shader contract exactly: `dirLight`, `pointLights[2]`, and `material.*`.
- Use the documented Blinn-Phong half-vector approach already described in the shader comments.
- Apply point-light attenuation to ambient, diffuse, and specular contributions.
- Do not change the provided `main()` accumulation structure.

## Task Breakdown

| Task | Lane | Description | Target | Depends on |
|------|------|------|------|------|
| T1 | impl | Implement directional-light ambient, diffuse, and specular response | `shaders/object.frag` | PR-HW3-01 |
| T2 | impl | Implement point-light response and attenuation | `shaders/object.frag` | T1 |

## Planned File Changes

- `[edit]` `shaders/object.frag`

## Verification

### Build verification

```bash
cd F:/Learn/ECE4122-6122-OpenGL
cmake --build build --config Debug --target Hmk3_Skeleton
```

### Runtime verification

This PR's visible effect is confirmed most clearly once `PR-HW3-08` is also in
place. After both land, expected behavior is:
- surfaces respond to the directional light across the whole scene
- point lights affect nearby objects more strongly than distant ones
- specular highlights shift when the camera moves

## Risks

| Risk | Why it matters | Mitigation |
|------|------|------|
| Using a reflected-vector Phong variant instead of the documented half-vector path | Results differ from the intended rubric expectation | Follow the shader comments and use Blinn-Phong |
| Forgetting attenuation on one or more point-light terms | Point lights look physically inconsistent | Apply the same attenuation factor to ambient, diffuse, and specular |
| Renaming uniforms or structs | Breaks the fixed skeleton contract with C++ | Keep the existing struct names and fields unchanged |

## Acceptance Criteria

- [ ] `calcDirLight()` is implemented
- [ ] `calcPointLight()` is implemented
- [ ] The fragment shader still compiles under the current shader program setup
- [ ] The shader-side light math is ready for C++ light uniform upload in `PR-HW3-08`
