# PR-HW3-03: Input Callbacks & Render Loop Framework

- Proposed title: `feat(main): implement input handling and render loop skeleton`
- Status: Draft

## Goal

连接摄像机到 GLFW 输入回调，搭建渲染循环的基本框架（clear、矩阵构建、shader 绑定），使窗口可交互。

前置条件：PR-HW3-01 (shader), PR-HW3-02 (camera)

## Execution Contract (Canonical Inputs)

| 类型 | 引用 | 与本 PR 的关系 |
|------|------|---------------|
| skeleton | `src/main.cpp` | 4 个回调 TODO + 渲染循环 TODO |
| dependency | `src/camera.h` | 回调函数调用 Camera 方法 |
| dependency | `src/shader.h` | 渲染循环中 `objShader.use()` |

## Scope

In scope:
- `mouseButtonCallback()` — 右键按下/释放跟踪
- `cursorPosCallback()` — 鼠标移动 → 摄像机旋转
- `scrollCallback()` — 滚轮 → FOV 缩放
- `processInput()` — 键盘轮询 → 摄像机移动 + ESC/R
- 渲染循环：`processInput` 调用、`glClearColor`/`glClear`、view/projection 矩阵构建、`objShader.use()` + viewPos uniform

Out of scope:
- drawModel（Stage 6）
- setLightUniforms（Stage 8）
- 地面绘制和模型绘制（Stage 6）

## Design

### mouseButtonCallback

```cpp
static void mouseButtonCallback(GLFWwindow*, int button, int action, int)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            g_rmb = true;
            g_firstMouse = true;
        } else if (action == GLFW_RELEASE) {
            g_rmb = false;
        }
    }
}
```

### cursorPosCallback

```cpp
static void cursorPosCallback(GLFWwindow*, double xpos, double ypos)
{
    if (!g_rmb) return;

    if (g_firstMouse) {
        g_lastX = (float)xpos;
        g_lastY = (float)ypos;
        g_firstMouse = false;
        return;
    }

    float dx = (float)xpos - g_lastX;
    float dy = g_lastY - (float)ypos;
    g_lastX = (float)xpos;
    g_lastY = (float)ypos;

    g_camera.processMouseMovement(dx, dy);
}
```

### scrollCallback

```cpp
static void scrollCallback(GLFWwindow*, double, double yo)
{
    g_camera.processScroll((float)yo);
}
```

### processInput

```cpp
static void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        g_camera.reset();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        g_camera.processKeyboard(CameraDir::FORWARD, g_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        g_camera.processKeyboard(CameraDir::BACKWARD, g_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        g_camera.processKeyboard(CameraDir::LEFT, g_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        g_camera.processKeyboard(CameraDir::RIGHT, g_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        g_camera.processKeyboard(CameraDir::DOWN, g_deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        g_camera.processKeyboard(CameraDir::UP, g_deltaTime);
}
```

### 渲染循环框架

在 delta time 计算之后、`glfwSwapBuffers` 之前：

```cpp
processInput(window);

glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

glm::mat4 view = g_camera.getViewMatrix();
glm::mat4 proj = glm::perspective(
    glm::radians(g_camera.fov),
    (float)SCR_W / (float)SCR_H,
    0.1f, 500.f);

objShader.use();
objShader.setVec3("viewPos", g_camera.position);
objShader.setFloat("material.shininess", 32.f);
```

## Task Breakdown

| Task | Lane | 内容 | 文件 | 估算 | 依赖 |
|------|------|------|------|------|------|
| T1 | impl | 实现 4 个回调函数 | `src/main.cpp` | 5min | PR-HW3-02 |
| T2 | impl | 填写渲染循环框架 | `src/main.cpp` | 5min | PR-HW3-01 |

## Planned File Changes

- `[edit]` `src/main.cpp` (填充 4 个回调 TODO + 渲染循环 TODO)

## Verification

### Structural verification

```bash
# 编译并运行
cd f:/Learn/ECE6122_hmk3
cmake -B build -S . && cmake --build build
./build/bin/Hmk3_Skeleton.exe
```

```
预期行为:
1. 窗口打开，背景为深蓝/紫色 (dusk sky)
2. 按 ESC → 窗口关闭
3. 按 R → 不崩溃（摄像机重置，但无几何体看不出效果）
4. 无几何体渲染（预期 — drawModel 未实现）
```

## Risk

| 风险 | 严重度 | 缓解 |
|------|--------|------|
| g_firstMouse 未重置导致首次右键拖拽视角跳跃 | LOW | 在 mouseButtonCallback PRESS 中设置 `g_firstMouse = true` |

## Acceptance Criteria

- [ ] 4 个回调函数（mouseButton, cursorPos, scroll, processInput）均已实现
- [ ] 渲染循环中调用了 `processInput()`、`glClear`、构建了 view/projection 矩阵
- [ ] 编译零错误
- [ ] 窗口背景颜色为深蓝色（非黑色）
- [ ] ESC 键正常关闭窗口
