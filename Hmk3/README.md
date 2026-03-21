# HW3 - 3D Rural Scene

This assignment implements a small rural 3D scene in OpenGL 3.3 Core using the
course `ECE4122-6122-OpenGL` repository, `GLEW`, `GLFW`, `GLM`, `Assimp`, and
`stb_image`.

The homework lives in the `Hmk3/` subdirectory of the course repo and is built
through the top-level `CMakeLists.txt`.

## What Is Implemented

- recursive OBJ loading through Assimp
- VAO/VBO/EBO mesh upload and indexed drawing
- directional sunlight plus two point lights
- textured ground plane with visible tiling
- FPS-style camera with keyboard, mouse, reset, and exit controls
- a composed scene with multiple placed models

## Build

Build from the top-level course repo:

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
- `Mouse Wheel`: zoom field of view
- `R`: reset camera
- `ESC`: quit

## Assets Used

The scene uses these homework assets:

- `assets/models/farmhouse/Farm_house.obj`
- `assets/models/barrel/Barrel_OBJ.obj`
- `assets/models/pinetree/Pine_Tree.obj`
- `assets/models/bench/wooden_bench.obj`
- `assets/models/lamp/objLamp.obj`
- `assets/models/robot/Robot.obj`

Ground texture:

- `assets/models/pinetree/Texture/grass.jpg`

## Scene Notes

- The scene places at least five distinct OBJ models in a shared world space.
- Multiple objects use non-zero Y-axis rotations.
- The ground plane is tiled and textured.
- The robot is oriented to face the default camera view.
- Asset files under `assets/` were kept in-place and were not edited.

## Environment Notes

- The homework is integrated into the course repo as the `Hmk3` subdirectory.
- The course skeleton uses `GLEW`; the PDF mention of `GLAD` was treated as a
  course typo per instructor clarification.
- Runtime assumes shader and asset paths are resolved relative to `Hmk3/`.

## Submission Notes

- Include `src/`, `shaders/`, `assets/`, `CMakeLists.txt`, and this `README.md`.
- Exclude generated directories such as `build/`.

## Walkthrough Video

YouTube link:

- https://youtu.be/R-tP1IJDwVk
