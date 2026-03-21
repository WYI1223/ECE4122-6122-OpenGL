# PR-HW3-01: Shader Compile/Link Foundation

- Proposed title: `feat(shader): implement compile/link foundation for object shader`
- Status: Draft

## Goal

Implement the shader-program foundation in `src/shader.h` so the homework can
read, compile, and link `shaders/object.vert` and `shaders/object.frag`.

This PR follows the fixed skeleton as-is:
- keep the existing `Shader` class interface
- keep the existing uniform setter API
- use the provided `GLEW`-based loader path
- do not introduce `GLAD` here; the PDF mention was a course typo

## Preconditions

`PR-HW3-00` is complete.

## Execution Contract (Canonical Inputs)

| Type | Reference | Relation to this PR |
|------|------|------|
| skeleton | `src/shader.h` | Contains the constructor plus `compile()` and `checkLink()` TODOs |
| shader source | `shaders/object.vert` | Vertex shader source loaded by the constructor |
| shader source | `shaders/object.frag` | Fragment shader source loaded by the constructor |
| runtime usage | `src/main.cpp` | Instantiates `Shader objShader("shaders/object.vert", "shaders/object.frag")` |

## Scope

In scope:
- implement `Shader::compile()`
- implement `Shader::checkLink()`
- implement the constructor flow: read, compile, attach, link, cleanup
- preserve error logging so shader failures are visible in the console

Out of scope:
- changing uniform setter behavior
- editing shader source logic
- changing the shader file paths used by `main.cpp`

## Design Constraints

- Preserve the current public API of `Shader`.
- Use `glCreateShader`, `glShaderSource`, `glCompileShader`, and shader-info logs for compile failures.
- Use `glCreateProgram`, `glAttachShader`, `glLinkProgram`, and program-info logs for link failures.
- Delete intermediate shader objects after linking.
- Treat relative shader paths in the current runtime layout as part of the contract.

## Task Breakdown

| Task | Lane | Description | Target | Depends on |
|------|------|------|------|------|
| T1 | impl | Implement `compile()` with compile-status checks and info-log printing | `src/shader.h` | none |
| T2 | impl | Implement `checkLink()` with link-status checks and info-log printing | `src/shader.h` | T1 |
| T3 | impl | Implement the constructor that reads both files, compiles both stages, links the program, and deletes temporary shader handles | `src/shader.h` | T1-T2 |

## Planned File Changes

- `[edit]` `src/shader.h`

## Verification

### Build verification

```bash
cd F:/Learn/ECE4122-6122-OpenGL
cmake --build build_hmk3_stage0 --config Debug --target Hmk3_Skeleton
```

If `cmake` is not on `PATH`, use the Stage 0 documented Visual Studio bundled
`cmake.exe` path.

### Runtime verification

```bash
cd F:/Learn/ECE4122-6122-OpenGL/build/bin
./Hmk3_Skeleton.exe
```

Expected result:
- the app launches and opens a window
- no `[Shader] compile error` message appears
- no `[Shader] link error` message appears

## Risks

| Risk | Why it matters | Mitigation |
|------|------|------|
| Shader paths resolve relative to the executable working directory | The constructor may fail even if the code is correct | Continue launching from the Stage 0 verified runtime layout |
| Compile errors are swallowed | Later stages become harder to debug | Print shader and program info logs immediately on failure |

## Acceptance Criteria

- [ ] All TODOs in `src/shader.h` covered by this PR are implemented
- [ ] `Hmk3_Skeleton` builds successfully from the top-level repo
- [ ] The program launches without shader compile or link errors
- [ ] No `GLAD`-specific work or interface changes were introduced
