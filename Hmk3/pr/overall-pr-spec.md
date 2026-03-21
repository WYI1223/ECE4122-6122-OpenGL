# HW3 Overall Delivery Roadmap

- Proposed title: `docs(hw3): define end-to-end implementation roadmap`
- Status: Draft

## Goal

This document defines the overall implementation plan for HW3 as a sequence of small PR-sized stages.

This file is the roadmap document for the whole homework. It is not itself an implementation PR.

It merges three sources into one execution plan:
- the assignment PDF as the requirements SSOT
- the current project skeleton under `src/` and `shaders/` as the fixed reference implementation
- the already-downloaded assets under `assets/models/`

This spec is intentionally high level. It does not expand the detailed design for each stage doc yet.

## Non-Goals

Out of scope for this document:
- filling in `pr/stage1-9` with detailed code-level instructions
- implementing any code changes
- changing the provided project skeleton, public interfaces, shader contracts, or file boundaries
- adding extra-credit features such as skybox, fog, or shadows

## Source Of Truth

The assignment PDF is the SSOT for requirements, grading intent, and acceptance criteria.

The current project skeleton is secondary and plays a different role:

- it is the fixed reference implementation and interface contract
- it is not to be redesigned, renamed, or restructured
- stage planning must fit the PDF requirements into the existing skeleton rather than changing the skeleton to match our preferred design

Decision priority:

1. Assignment PDF for what the homework must do and how it will be judged.
2. Current code skeleton for how that behavior must be implemented in this repository.
3. Current asset inventory for which local models and textures to use by default.

## Repo-Specific Notes

The current repository is not a byte-for-byte match with the PDF starter notes. Some of these differences are fixed implementation constraints of this workspace, and at least one is explained by an official course clarification:

- The PDF line requiring `GLAD` appears to be a course typo per the professor's 3/12 lecture clarification; the provided skeleton uses `GLEW`, and that is the loader we should follow here.
- The vertex shader expects `normalMatrix` as a `mat4`, then casts to `mat3` in shader code.
- The fragment shader uses `dirLight` plus `pointLights[2]`, not the single `sun/lantern` naming from the PDF example.
- The camera skeleton supports `Q/E` vertical motion and `R` reset in addition to the PDF's minimum controls.
- Terrain geometry is already scaffolded in `src/main.cpp` via `buildTerrain()`.
- Texture loading fallback logic already exists in `src/model.h`.

Implementation planning must respect these existing contracts:

- do not rename uniforms or struct layouts already used by the shaders and C++ code
- do not move responsibilities across files unless the skeleton already assigns them there
- prefer filling TODOs and provided extension points over introducing new architecture
- if the PDF and skeleton differ in wording, preserve the skeleton interface while still meeting the PDF-visible behavior

## Target Outcome

The final submission should deliver:

- a compilable OpenGL 3.3 application
- at least five distinct OBJ-based scene objects placed meaningfully in world space
- a textured ground plane with visible tiling
- working Assimp-based mesh and material loading
- a free-look camera with keyboard, mouse, and scroll interaction
- visible Blinn-Phong style lighting with one directional light and two point lights
- a clean `CMake` build and a `README.md` suitable for submission

## Planned Scene Baseline

Use the current local assets as the default scene pool:

- `farmhouse`
- `pinetree`
- `lamp`
- `bench`
- `barrel`
- `farmer`
- `robot`

Minimum requirement is five distinct models. The likely default scene should be a farmstead or yard composition centered around the farmhouse, trees, lamps, and small props.

## File Ownership Map

Primary ownership by stage should follow the current skeleton:

| Area | Primary file(s) |
|------|------------------|
| Environment and build baseline | `CMakeLists.txt`, runtime layout, third-party dependency paths |
| Shader program wrapper | `src/shader.h` |
| Camera math and controls | `src/camera.h` |
| Input wiring and render loop | `src/main.cpp` |
| Mesh GPU upload and draw | `src/mesh.h` |
| Assimp model loading and texture extraction | `src/model.h` |
| Fragment lighting math | `shaders/object.frag` |
| Scene placement, uniform upload, polish | `src/main.cpp`, `README.md` |

## Stage Roadmap

| Stage | PR ID | Focus | Primary files | Depends on | Exit condition |
|------|------|------|------|------|------|
| 0 | `PR-HW3-00` | Environment setup and baseline verification | `CMakeLists.txt`, dependency layout, runtime asset/shader paths | none | Toolchain, external deps, configure/build commands, and runtime layout are validated |
| 1 | `PR-HW3-01` | Shader compile/link foundation | `src/shader.h` | 0 | App can create and use a linked shader program |
| 2 | `PR-HW3-02` | Camera math and movement methods | `src/camera.h` | 0 | Camera basis vectors, movement, mouse look, zoom, reset all work logically |
| 3 | `PR-HW3-03` | GLFW input callbacks and render-loop skeleton | `src/main.cpp` | 0, 1, 2 | Window is interactive, view/projection update each frame |
| 4 | `PR-HW3-04` | Mesh VAO/VBO/EBO upload and draw path | `src/mesh.h` | 0 | A `Mesh` instance can bind textures and issue `glDrawElements` |
| 5 | `PR-HW3-05` | Assimp model loading and material texture extraction | `src/model.h` | 0, 4 | Multi-mesh OBJ models load and draw through `Model::draw()` |
| 6 | `PR-HW3-06` | Scene composition and model drawing in world space | `src/main.cpp` | 0, 3, 5 | At least five scene objects plus ground are visible in one world |
| 7 | `PR-HW3-07` | Fragment lighting functions | `shaders/object.frag` | 0, 1 | Directional and point light math is implemented in shader |
| 8 | `PR-HW3-08` | Upload light uniforms and complete lit render path | `src/main.cpp` | 0, 3, 6, 7 | Scene shows visible diffuse/specular response from all lights |
| 9 | `PR-HW3-09` | Ground texture, scale tuning, README, submission checks | `src/main.cpp`, `README.md` | 0, 8 | Rubric checklist is satisfied and build is submission-ready |

## Dependency Strategy

Critical path:

`0 -> 1 -> 3 -> 6 -> 8 -> 9`

Supporting path:

`0 -> 2 -> 3`

Geometry path:

`0 -> 4 -> 5 -> 6`

Lighting path:

`0 -> 1 -> 7 -> 8`

Parallelization opportunities:

- Stage 0 must complete first because it establishes the executable build baseline.
- Stage 2 and Stage 4 can proceed independently after Stage 0.
- Stage 7 can be prepared in parallel with Stage 4 and Stage 5 once shader interfaces are confirmed.
- Stage 9 should wait until rendering, lighting, and placement are already stable.

## Recommended Delivery Order

The implementation order should optimize for fast visual feedback:

1. Verify the environment, dependency layout, and configure/build/run baseline.
2. Make shader creation and camera math stable.
3. Make the window interactive with a running render loop.
4. Make one mesh and one model draw correctly.
5. Expand that draw path to a full multi-object scene.
6. Turn on lighting and confirm the scene reacts to camera motion.
7. Finish rubric-driven polish, scale tuning, texturing checks, and submission assets.

This order keeps the project debuggable at every step instead of postponing all visual confirmation until the end.

## Acceptance Criteria By Milestone

### Milestone 0: Environment Ready

Satisfied when:

- `cmake` is available from the working shell
- at least one supported compiler/toolchain is installed and callable by CMake
- the external dependency layout expected by `CMakeLists.txt` exists
- configure and build commands are known and reproducible
- runtime assumptions for shaders and copied assets are understood before feature work begins

### Milestone A: Engine Skeleton Alive

Satisfied when:

- shaders compile and link
- camera math returns a valid `glm::lookAt` view matrix
- input callbacks update camera state
- the app clears and renders every frame without crashing

### Milestone B: Geometry And Assets Alive

Satisfied when:

- `Mesh` uploads position, normal, and UV data to attribute locations `0/1/2`
- `Model` recursively traverses Assimp nodes
- multi-mesh models draw successfully
- diffuse textures are loaded or gracefully replaced by fallback textures

### Milestone C: Scene Alive

Satisfied when:

- at least five distinct models are placed at different world positions
- at least three models visibly use non-zero rotation
- at least two models visibly use non-uniform scale
- the ground plane is present and textured with repeat tiling

### Milestone D: Lighting Alive

Satisfied when:

- directional light affects the whole scene
- both point lights affect nearby surfaces
- specular highlights move when the camera moves
- normals are transformed correctly under non-uniform scale

### Milestone E: Submission Ready

Satisfied when:

- the project builds from a clean build directory
- `README.md` documents build steps, controls, and asset choices
- the scene visually satisfies the rubric, not just the compile step
- the submission package excludes generated build artifacts

## Rubric Mapping

| Rubric area | What must be visibly true in the final result |
|------|------|
| Scene composition | Five or more distinct OBJ models are arranged logically, not clustered at the origin |
| Rotation and scale variety | Three or more objects are rotated, two or more use non-uniform scale |
| Ground plane | Terrain quad is rendered and uses a tiled texture |
| Assimp mesh loading | Positions, normals, UVs, indices, and material textures flow from Assimp into GPU buffers |
| Recursive traversal | Models with multiple child nodes or sub-meshes still render fully |
| Lighting | Directional plus point lights produce diffuse and specular response |
| Camera | Keyboard, mouse, scroll, and exit controls all work interactively |
| Texturing | At least three scene objects show textures correctly |
| Submission quality | CMake build works cleanly and README is present |

## Known Risks

| Risk | Why it matters | Mitigation |
|------|------|------|
| Texture path mismatch from `.mtl` files | Models may render gray or black | Normalize paths through filename extraction and cache resolved full paths |
| Asset scale mismatch | Scene may look broken even if code is correct | Add explicit per-object scale control and tune late in Stage 9 |
| Wrong light uniform names | Scene may render dark even though shader math is correct | Treat current shader names as authoritative: `dirLight`, `pointLights[i]`, `material.*` |
| Incorrect normal transform | Specular and diffuse response will look wrong under non-uniform scale | Always compute `transpose(inverse(model))` and upload as the expected matrix form |
| Overloading `main.cpp` too early | Debugging becomes harder when too many concerns land at once | Keep `main.cpp` changes staged across input, scene draw, and light upload PRs |
| Build-only success without visual verification | Can still lose many rubric points | Require runtime checks after each major milestone, not just compile checks |

## Verification Strategy

Every stage should include both build verification and runtime verification.

Build verification:

```bash
cd F:/Learn/ECE4122-6122-OpenGL
cmake --build build --config Debug --target Hmk3_Skeleton
```

If `cmake` is not on `PATH` on the local machine, use the Stage 0 documented
Visual Studio bundled `cmake.exe` path instead of the bare `cmake` command.

Runtime verification should be milestone-specific:

- after Stage 0: confirm configure/build prerequisites and runtime path assumptions are satisfied
- after Stage 3: confirm camera motion, mouse look, and projection updates
- after Stage 5: confirm at least one textured model renders completely
- after Stage 6: confirm five-plus objects and ground are visible together
- after Stage 8: confirm light response changes as camera moves
- after Stage 9: confirm homework README and submission packaging checklist are complete

## Definition Of Done

This overall plan is considered successfully delivered when all of the following are true:

- all planned stage PRs can be derived cleanly from this document
- the final implementation satisfies the PDF rubric while staying within the provided skeleton contracts
- no required TODO remains in the allowed implementation areas of the skeleton
- the build succeeds from a fresh build directory
- the scene is visually coherent enough to defend during grading or walkthrough

## Follow-Up

The next step after approving this file is to execute `PR-HW3-01` onward using
the aligned stage documents, with `PR-HW3-00` already serving as the baseline
gate.
