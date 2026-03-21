# HW3 - 3D Rural Scene

This homework builds a small rural scene in OpenGL 3.3 Core using the course
`ECE4122-6122-OpenGL` repository, `GLEW`, `GLFW`, `GLM`, and `Assimp`.

## Build

From the top-level course repo:

```powershell
cd F:\Learn\ECE4122-6122-OpenGL
& 'D:\Application\VisualStudioIDE\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' -B build -S . -G "Visual Studio 17 2022" -A x64
& 'D:\Application\VisualStudioIDE\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe' --build build --config Debug --target Hmk3_Skeleton
```

## Run

Run from the homework directory so relative shader and asset paths resolve:

```powershell
cd F:\Learn\ECE4122-6122-OpenGL\Hmk3
..\build\bin\Hmk3_Skeleton.exe
```

## Controls

- `W / A / S / D`: move forward / left / backward / right
- `Q / E`: move down / up
- `Right Mouse + Drag`: yaw / pitch camera look
- `Mouse Wheel`: zoom
- `R`: reset camera
- `ESC`: quit

## Scene Contents

The scene currently uses these distinct models:

- farmhouse
- barrel
- pine tree
- bench
- street lamp
- robot

The ground uses the existing `grass.jpg` texture already included under the
homework assets and is tiled across the terrain quad.

## Notes

- The homework is integrated into the course repo as the `Hmk3` subdirectory.
- The course skeleton uses `GLEW`; the PDF mention of `GLAD` was treated as a
  course typo per instructor clarification.
- Assets were kept in-place under `assets/` and were not modified.
