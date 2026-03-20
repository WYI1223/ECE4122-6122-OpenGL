# PR-HW3-00: Environment Setup & Baseline Verification

- Proposed title: `chore(env): verify toolchain, dependency layout, and runtime baseline`
- Status: Draft

## Goal

Establish a known-good local environment before any feature TODOs are implemented.

This PR exists to answer one question first:

"Can the provided skeleton be configured, built, and launched in this workspace without changing its architecture?"

Per the roadmap rules:
- the assignment PDF remains the SSOT for requirements
- the provided skeleton is a fixed reference implementation
- this PR must not redesign, rename, or restructure the skeleton

## Preconditions

None.

This is the required baseline PR before `PR-HW3-01` through `PR-HW3-09`.

## Execution Contract (Canonical Inputs)

| Type | Reference | Relation to this PR |
|------|------|------|
| requirements SSOT | `ECE4122_6122_HW3_OpenGL_Assimp-1.pdf` | Defines required platform behavior and final grading intent |
| top-level build system | `../CMakeLists.txt` | Instructor-approved parent entry point that builds `external` and `Hmk3` together |
| local build system | `CMakeLists.txt` | Defines the fixed homework target and asset-copy behavior inside `Hmk3` |
| runtime entry | `src/main.cpp` | Defines window creation and executable startup path |
| shader assets | `shaders/object.vert`, `shaders/object.frag` | Must be reachable at runtime |
| model assets | `assets/models/**` | Must be present and copyable to the runtime output folder |
| third-party deps | `../external/glfw-3.1.2`, `../external/glm-0.9.7.1`, `../external/glew-1.13.0`, `../external/assimp-3.0.1270` | Required by the current skeleton and `CMakeLists.txt` |

## Scope

In scope:
- derive the minimum environment checklist directly from the PDF requirements
- verify local availability of `cmake`
- verify presence of a C++ toolchain usable by CMake
- verify existence of the `../external` dependency layout expected by `CMakeLists.txt`
- verify canonical configure/build commands for this workspace
- verify runtime assumptions for shader paths and copied assets
- record current blockers that must be resolved before feature PRs start

Out of scope:
- implementing any functional TODO in `src/` or `shaders/`
- changing shader contracts, file ownership, public interfaces, or project structure
- redesigning `CMakeLists.txt` to use a different dependency strategy
- writing the final submission `README.md`

## Design

### 0.1 Build Baseline Policy

This PR treats the current repository layout as fixed.

That means:
- we validate the environment against the provided skeleton
- we do not rewrite the skeleton to fit the machine
- if a dependency is missing, the fix is to install/provide it in the expected place, not to redesign the project

### 0.2 Stage 0 Must-Follow Points

Stage 0 should only enforce the environment points that are necessary to satisfy the PDF and make later feature PRs possible.

#### A. PDF-level requirements

These come directly from the assignment handout and are the real acceptance targets:

- OpenGL 3.3 Core Profile must be available
- GLSL 330 core shaders must compile and run
- GLFW must be available for windowing and input
- Assimp must be available for OBJ model loading
- the project must build through the provided `CMakeLists.txt`
- the project must support a clean build from a fresh build directory

Course clarification to apply during Stage 0:

- the PDF line saying `GLAD` is required is treated as a course typo based on the professor's 3/12 lecture clarification
- therefore, for this repository, the OpenGL function loader requirement is satisfied by the provided `GLEW`-based skeleton rather than by introducing `GLAD`

#### B. Repo bootstrap requirements

These are not separate homework requirements. They are the minimum local conditions needed to realize the PDF requirements using this fixed skeleton:

- `cmake` must be invokable in a stable, repeatable way
- a compatible C++ toolchain must be available to CMake
- the third-party dependency tree expected by `CMakeLists.txt` must exist
- the `GLEW` headers/libs expected by the fixed skeleton must be provisioned locally
- shader files and asset files must be reachable at runtime
- no skeleton source or interface changes are allowed during environment bring-up

### 0.3 Environment Bring-Up Order

We should fix the environment in this order:

1. Lock down the canonical `cmake` entry point
2. Restore or provide the expected third-party dependency tree
3. Place the homework under the course repository as `Hmk3/`
4. Add `add_subdirectory(Hmk3)` to the top-level `CMakeLists.txt` after `add_subdirectory(external)`
5. Re-run `cmake -B build_hmk3_stage0 -S .`
6. Re-run `cmake --build build_hmk3_stage0 --target Hmk3_Skeleton`
7. Launch the executable and verify runtime paths
8. Only then move to Stage 1 feature work

### 0.4 Required Checks

The following checks define the environment baseline:

1. Tooling check
   `cmake` must be invokable from the shell used for development.
2. Compiler check
   A usable C++ compiler/toolchain must be available to CMake.
3. Dependency layout check
   The external directories referenced by `CMakeLists.txt` must exist at the expected relative paths.
4. Configure check
   `cmake -B build -S .` must succeed.
5. Build check
   `cmake --build build` must succeed.
6. Runtime layout check
   The executable must be able to resolve shaders and copied assets using the skeleton's path assumptions.

### 0.5 Canonical Commands

The canonical commands for baseline verification are:

```bash
cd F:/Learn/ECE4122-6122-OpenGL
cmake -B build_hmk3_stage0 -S .
cmake --build build_hmk3_stage0 --target Hmk3_Skeleton
./build/bin/Hmk3_Skeleton.exe
```

If a different generator is needed on the local machine, that generator must still be driven through `cmake`; this PR does not introduce a non-CMake build path.

Current fallback discovered in this workspace:

```powershell
cd F:\Learn\ECE4122-6122-OpenGL

& 'D:\Application\VisualStudioIDE\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' `
  -B build_hmk3_stage0 -S . -G "Visual Studio 17 2022" -A x64

& 'D:\Application\VisualStudioIDE\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' `
  --build build_hmk3_stage0 --config Debug --target Hmk3_Skeleton
```

### 0.6 Current Environment Snapshot

Based on direct inspection of the current workspace:

- `cmake` is not currently available on `PATH` in the active shell
- however, a usable Visual Studio bundled `cmake.exe` exists at:
  `D:\Application\VisualStudioIDE\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe`
- Visual Studio 2022 Community and MSVC 14.32 are installed and usable
- the course OpenGL repository has been cloned and now provides the expected parent layout:
  `external/`, tutorials, and `Hmk3/`
- the homework directory has been renamed to `Hmk3` to match the instructor's recommended layout and avoid Linux case-sensitivity issues
- the top-level `CMakeLists.txt` now includes:
  `add_subdirectory (Hmk3)`
- top-level configure succeeds with generator `Visual Studio 17 2022`
- top-level build succeeds for target `Hmk3_Skeleton` and produces:
  `F:\Learn\ECE4122-6122-OpenGL\build\bin\Hmk3_Skeleton.exe`
- runtime asset copying succeeds into:
  `F:\Learn\ECE4122-6122-OpenGL\build\bin\assets`
- the executable can be launched successfully from the top-level build output directory

This means the Stage 0 build baseline is now established through the instructor-approved top-level integration flow.

### 0.7 Exit Criteria

This PR is complete only when all baseline blockers are removed and later PRs can assume a stable configure/build/run environment.

## Task Breakdown

| Task | Lane | Description | Target | Estimate | Depends on |
|------|------|------|------|------|------|
| T1 | verify | Lock down the canonical `cmake` command path | local toolchain | 2min | none |
| T2 | verify | Confirm a usable compiler/toolchain is available to CMake | local toolchain | 2min | T1 |
| T3 | verify | Confirm required `../external` dependency folders exist | workspace layout | 3min | none |
| T4 | integrate | Place the homework under the course repository as `Hmk3/` | repo layout | 3min | T1-T3 |
| T5 | integrate | Add `add_subdirectory(Hmk3)` to the top-level `CMakeLists.txt` | `../CMakeLists.txt` | 2min | T4 |
| T6 | verify | Re-run configure/build from the top-level repo and confirm binary output | `build/bin` | 3min | T5 |
| T7 | verify | Launch app once to validate runtime asset/shader assumptions | `src/main.cpp` runtime path | 2min | T6 |
| T8 | document | Record the final stage0 baseline and commands | PR notes / README later | 3min | T1-T7 |

## Planned File Changes

- `[rename]` `../hwk3` -> `../Hmk3`
- `[edit]` `../CMakeLists.txt` (add `add_subdirectory(Hmk3)` after `add_subdirectory(external)`)
- `[none expected]` `src/*.h`, `src/main.cpp`, `shaders/*`

This PR is still validation-first. The only source-controlled changes allowed in Stage 0 are the repo-structure and top-level build-entry changes needed to match the instructor-approved integration flow.

## Verification

### Baseline commands

```bash
cd F:/Learn/ECE4122-6122-OpenGL
cmake -B build_hmk3_stage0 -S .
cmake --build build_hmk3_stage0 --target Hmk3_Skeleton
./build/bin/Hmk3_Skeleton.exe
```

### Directory checks

```bash
dir F:/Learn/ECE4122-6122-OpenGL/external
dir F:/Learn/ECE4122-6122-OpenGL/external/glfw-3.1.2
dir F:/Learn/ECE4122-6122-OpenGL/external/glm-0.9.7.1
dir F:/Learn/ECE4122-6122-OpenGL/external/glew-1.13.0
dir F:/Learn/ECE4122-6122-OpenGL/external/assimp-3.0.1270
```

### Runtime checks

- The executable launches and opens a GLFW window.
- No startup-time file-not-found errors occur for shaders or copied assets.
- `build/bin/assets` exists after build and contains the mirrored asset tree.

## Risks

| Risk | Severity | Mitigation |
|------|--------|------|
| `cmake` not available from the default shell | MEDIUM | Either add the Visual Studio bundled `cmake` to `PATH`, use a wrapper script, or install standalone CMake |
| Required compiler/generator missing | HIGH | Install a supported toolchain that CMake can discover |
| `../external` dependency tree missing | HIGH | Restore the exact external layout expected by `CMakeLists.txt` rather than changing the build script |
| Runtime path mismatch for shaders/assets | MEDIUM | Run the executable from the expected working layout and verify asset copy behavior early |
| Team starts feature work before environment is stable | HIGH | Treat `PR-HW3-00` as a hard gate for `PR-HW3-01+` |

## Acceptance Criteria

- [ ] OpenGL 3.3 Core capable development environment is available
- [ ] `cmake` is callable from the active development shell, or an agreed canonical wrapper path is documented and used consistently
- [ ] a C++ toolchain discoverable by CMake is installed
- [ ] GLFW and Assimp dependencies required by the fixed skeleton are provisioned locally
- [ ] all required third-party dependency directories under `../external` exist
- [ ] the homework exists under the course repo as `Hmk3/`
- [ ] the top-level `CMakeLists.txt` includes `add_subdirectory(Hmk3)` after `add_subdirectory(external)`
- [ ] `cmake -B build_hmk3_stage0 -S .` succeeds from the top-level repo
- [ ] `cmake --build build_hmk3_stage0 --target Hmk3_Skeleton` succeeds from the top-level repo
- [ ] the executable starts and opens a window
- [ ] asset copy behavior to `build/bin/assets` is verified
- [ ] no skeleton source/interface changes were needed to satisfy the environment baseline
