# PR-HW3-05: Assimp Model Loading And Material Textures

- Proposed title: `feat(model): implement Assimp mesh conversion and texture extraction`
- Status: Draft

## Goal

Implement the remaining `Model` TODOs in `src/model.h` so OBJ assets loaded
through Assimp can be converted into one or more `Mesh` instances and can carry
their material textures into the render path.

## Preconditions

- `PR-HW3-00` is complete
- `PR-HW3-04` is complete

## Execution Contract (Canonical Inputs)

| Type | Reference | Relation to this PR |
|------|------|------|
| skeleton | `src/model.h` | Contains `processMesh()` and `loadMaterialTextures()` TODOs |
| dependency | `src/mesh.h` | Receives converted vertices, indices, and textures |
| runtime usage | `src/main.cpp` | Loads `Model` instances such as lamp and robot |
| assets | `assets/models/**` | Provide OBJ, MTL, and texture files used by Assimp |

## Scope

In scope:
- implement `processMesh()`
- implement `loadMaterialTextures()`
- extract positions, normals, UVs, and indices from `aiMesh`
- extract diffuse, specular, and emissive textures from `aiMaterial`
- use the provided texture-cache strategy keyed by resolved full paths

Out of scope:
- changing the existing Assimp import flags in `load()`
- changing the provided recursive `processNode()` traversal
- scene placement or world transforms

## Design Constraints

- Preserve the current `Model` public API and loading flow.
- Keep `load()` and `processNode()` as the authoritative traversal path.
- Build full texture paths from the current model directory plus the filename extracted from Assimp.
- Reuse `m_texCache` to avoid duplicate OpenGL texture uploads.
- Use the provided fallback-texture behavior if a texture path is missing or unreadable.

## Task Breakdown

| Task | Lane | Description | Target | Depends on |
|------|------|------|------|------|
| T1 | impl | Convert one `aiMesh` into vertex, index, and texture vectors | `src/model.h` | PR-HW3-04 |
| T2 | impl | Implement material texture extraction with filename normalization and cache reuse | `src/model.h` | T1 |

## Planned File Changes

- `[edit]` `src/model.h`

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

Expected signals:
- model-loading log lines appear without Assimp load failure
- multi-mesh assets still report as loaded
- missing textures fall back gracefully instead of crashing

## Risks

| Risk | Why it matters | Mitigation |
|------|------|------|
| Assimp texture paths include stale directories | Textures fail to resolve even though the files exist locally | Normalize to filename-only before rebuilding the path under `m_directory` |
| Duplicate textures are uploaded repeatedly | Wasteful and can complicate debugging | Use `m_texCache` consistently on the resolved full path |
| UVs or normals are assumed to exist unconditionally | Some assets may load incorrectly or crash | Guard with `HasNormals()` and `mTextureCoords[0]` checks |

## Acceptance Criteria

- [ ] All `Model` TODOs in `src/model.h` are implemented
- [ ] Positions, normals, UVs, and indices are transferred correctly from Assimp data
- [ ] Diffuse/specular/emissive textures are extracted through the provided material-loading path
- [ ] Multi-mesh OBJ models load through `Model::draw()` without changing the existing public interface
