# PR-HW3-06: Scene Composition And World-Space Draw Path

- Proposed title: `feat(scene): implement drawModel helper and place the full world scene`
- Status: Draft

## Goal

Implement the scene-composition layer in `src/main.cpp` so the homework can:
- upload per-object transform matrices through `drawModel()`
- pack the normal matrix into the vertex shader's expected `mat4` uniform
- draw the terrain quad
- place at least five distinct world-space objects in one coherent scene

This PR owns placement and transform wiring. Final lighting polish still lands in
`PR-HW3-08`.

## Preconditions

- `PR-HW3-00` is complete
- `PR-HW3-03` is complete
- `PR-HW3-05` is complete

## Execution Contract (Canonical Inputs)

| Type | Reference | Relation to this PR |
|------|------|------|
| skeleton | `src/main.cpp` | Contains `drawModel()`, terrain draw, and scene-placement TODOs |
| shader contract | `shaders/object.vert` | Expects `model`, `view`, `projection`, and `normalMatrix` uniforms |
| asset pool | `assets/models/**` | Supplies the default farmhouse, tree, lamp, bench, barrel, farmer, and robot models |
| roadmap | `overall-pr-spec.md` | Defines the minimum scene-composition target and stage exit condition |

## Scope

In scope:
- implement `drawModel()`
- upload `model`, `view`, `projection`, and packed `normalMatrix`
- draw the terrain VAO from the existing terrain buffers
- instantiate and place the required scene objects in world space
- use rotation and non-uniform scale where the homework requires visible variety

Out of scope:
- implementing fragment-lighting functions
- implementing `setLightUniforms()`
- final README and submission packaging

## Design Constraints

- Preserve the current `main.cpp` structure and helper ownership.
- Keep `normalMatrix` aligned with the current shader contract: compute `transpose(inverse(mat3(model)))`, then pack into a `mat4`.
- Follow the placement notes already embedded in `main.cpp` unless later asset constraints force a small adjustment.
- Keep the scene built from the current local asset inventory rather than introducing new dependencies.

## Task Breakdown

| Task | Lane | Description | Target | Depends on |
|------|------|------|------|------|
| T1 | impl | Implement `drawModel()` with per-object transform uploads and `model.draw(shader)` | `src/main.cpp` | PR-HW3-05 |
| T2 | impl | Draw the provided terrain geometry in the main render loop | `src/main.cpp` | PR-HW3-03 |
| T3 | impl | Add the world-space draw calls for the chosen farm-scene objects | `src/main.cpp` | T1-T2 |

## Planned File Changes

- `[edit]` `src/main.cpp`

## Verification

### Build verification

```bash
cd F:/Learn/ECE4122-6122-OpenGL
cmake --build build --config Debug --target Hmk3_Skeleton
```

### Runtime verification

```bash
cd F:/Learn/ECE4122-6122-OpenGL/Hmk3
../build/bin/Hmk3_Skeleton.exe
```

Expected result once this PR is combined with the lighting path:
- the terrain is drawn
- at least five distinct scene objects are placed in one shared world
- at least three objects visibly use non-zero rotation
- at least two objects visibly use non-uniform scale

## Risks

| Risk | Why it matters | Mitigation |
|------|------|------|
| Wrong normal-matrix upload | Lighting later looks incorrect under non-uniform scale | Keep the packed normal-matrix path explicit in `drawModel()` |
| All objects are clustered near the origin | The scene misses a core rubric requirement | Use the placement plan in `main.cpp` as the baseline, then tune only as needed |
| Too much scene logic lands in one helper | Debugging transforms becomes harder | Keep `drawModel()` limited to per-object uniform upload plus `model.draw()` |

## Acceptance Criteria

- [ ] `drawModel()` is implemented in `src/main.cpp`
- [ ] The terrain quad is drawn from the provided terrain buffers
- [ ] The scene contains at least five distinct placed models
- [ ] The placement plan includes visible rotation and non-uniform scale variety
