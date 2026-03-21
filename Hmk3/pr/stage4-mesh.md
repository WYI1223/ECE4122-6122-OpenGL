# PR-HW3-04: Mesh GPU Upload And Draw Path

- Proposed title: `feat(mesh): implement VAO/VBO/EBO upload and texture-aware draw path`
- Status: Draft

## Goal

Implement the `Mesh` GPU path in `src/mesh.h` so a mesh can:
- upload interleaved vertex data plus indices to the GPU
- describe vertex attributes at locations `0/1/2`
- bind material textures using the current shader contract
- issue `glDrawElements()`

This PR is about one mesh at a time. Full Assimp model loading is the next PR.

## Preconditions

`PR-HW3-00` is complete.

## Execution Contract (Canonical Inputs)

| Type | Reference | Relation to this PR |
|------|------|------|
| skeleton | `src/mesh.h` | Contains `setupGPU()` and `draw()` TODOs |
| shader contract | `shaders/object.frag` | Defines `material.diffuse0`, `material.specular0`, `material.emissive0`, and has-flags |
| upstream consumer | `src/model.h` | Builds `Mesh` instances from Assimp data |

## Scope

In scope:
- implement `setupGPU()`
- implement `draw()`
- configure vertex attributes for position, normal, and UV
- bind textures and upload the expected material uniform names
- set `material.hasDiffuse`, `material.hasSpecular`, and `material.hasEmissive`

Out of scope:
- Assimp scene traversal
- texture file loading
- scene placement in `main.cpp`

## Design Constraints

- Preserve the existing `Vertex`, `Texture`, and `Mesh` interfaces.
- Use the existing attribute-location contract:
  - location `0`: position
  - location `1`: normal
  - location `2`: texCoords
- Use separate counters for diffuse/specular/emissive samplers.
- Reset the active texture unit to `GL_TEXTURE0` after drawing to reduce state leakage.

## Task Breakdown

| Task | Lane | Description | Target | Depends on |
|------|------|------|------|------|
| T1 | impl | Implement VAO/VBO/EBO creation, buffer uploads, and attribute wiring | `src/mesh.h` | none |
| T2 | impl | Implement the texture-binding draw path and material has-flags | `src/mesh.h` | T1 |

## Planned File Changes

- `[edit]` `src/mesh.h`

## Verification

### Build verification

```bash
cd F:/Learn/ECE4122-6122-OpenGL
cmake --build build --config Debug --target Hmk3_Skeleton
```

### Structural verification

Expected post-implementation properties:
- `setupGPU()` creates and fills one VAO, one VBO, and one EBO
- the mesh stride is `sizeof(Vertex)`
- `draw()` binds textures using `material.diffuseN`, `material.specularN`, and `material.emissiveN`
- `draw()` uses `glDrawElements(GL_TRIANGLES, ...)`

Runtime visibility for this path is confirmed again in `PR-HW3-05` and `PR-HW3-06`.

## Risks

| Risk | Why it matters | Mitigation |
|------|------|------|
| Wrong attribute offsets | Geometry renders as garbage or lighting breaks | Use `offsetof(Vertex, ...)` and `sizeof(Vertex)` |
| Wrong material uniform names | Textures silently fail to show up | Match the exact `material.*` names in `object.frag` |
| Texture state leakage | Later draws pick up the wrong texture unit or binding | Reset to `GL_TEXTURE0` after the mesh draw |

## Acceptance Criteria

- [ ] All `Mesh` TODOs in `src/mesh.h` are implemented
- [ ] Vertex attributes are wired to locations `0/1/2`
- [ ] Material texture uniforms and has-flags match the current fragment-shader contract
- [ ] The mesh draw path is ready for `Model::draw()` to use
