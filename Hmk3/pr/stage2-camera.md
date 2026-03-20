# PR-HW3-02: Camera Implementation

- Proposed title: `feat(camera): implement free-fly FPS camera`
- Status: Draft

## Goal

实现 FPS 风格自由飞行摄像机的全部方法：向量更新、视图矩阵、键盘移动、鼠标旋转、滚轮缩放、重置。

前置条件：无（不依赖 Stage 1，可并行开发）

## Execution Contract (Canonical Inputs)

| 类型 | 引用 | 与本 PR 的关系 |
|------|------|---------------|
| skeleton | `src/camera.h` | 包含 7 个 TODO 函数需要实现 |

## Scope

In scope:
- `updateVectors()` — 从 yaw/pitch 计算 front/right/up
- 构造函数体 — 调用 `updateVectors()`
- `getViewMatrix()` — 返回 `glm::lookAt`
- `processKeyboard()` — 6 方向移动
- `processMouseMovement()` — yaw/pitch 更新 + pitch clamp
- `processScroll()` — FOV 缩放
- `reset()` — 恢复初始状态

Out of scope:
- main.cpp 中的回调函数（Stage 3）
- Camera 类接口变更

## Design

```cpp
void updateVectors()
{
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    right = glm::normalize(glm::cross(front, worldUp));
    up    = glm::normalize(glm::cross(right, front));
}

Camera(glm::vec3 pos = { 0.f, 5.f, 15.f })
    : position(pos)
{
    updateVectors();
}

glm::mat4 getViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

void processKeyboard(CameraDir dir, float dt)
{
    float dist = speed * dt;
    switch (dir) {
        case CameraDir::FORWARD:  position += front   * dist; break;
        case CameraDir::BACKWARD: position -= front   * dist; break;
        case CameraDir::LEFT:     position -= right   * dist; break;
        case CameraDir::RIGHT:    position += right   * dist; break;
        case CameraDir::UP:       position += worldUp * dist; break;
        case CameraDir::DOWN:     position -= worldUp * dist; break;
    }
}

void processMouseMovement(float dx, float dy, bool constrainPitch)
{
    yaw   += dx * mouseSens;
    pitch += dy * mouseSens;
    if (constrainPitch)
        pitch = std::clamp(pitch, -89.f, 89.f);
    updateVectors();
}

void processScroll(float yOffset)
{
    fov -= yOffset;
    fov = std::clamp(fov, 1.f, 90.f);
}

void reset()
{
    position = { 0.f, 5.f, 15.f };
    yaw   = -90.f;
    pitch = 0.f;
    fov   = 45.f;
    updateVectors();
}
```

## Task Breakdown

| Task | Lane | 内容 | 文件 | 估算 | 依赖 |
|------|------|------|------|------|------|
| T1 | impl | 实现全部 7 个 TODO 方法 | `src/camera.h` | 10min | 无 |

## Planned File Changes

- `[edit]` `src/camera.h` (填充 7 个 TODO 函数体)

## Verification

### CI gates

```bash
cd f:/Learn/ECE6122_hmk3
cmake -B build -S . && cmake --build build 2>&1 | tail -5
```

### Structural verification

```bash
# 数学验证: yaw=-90, pitch=0 时 front 应为 (0, 0, -1)
# cos(-90°)*cos(0°) = 0,  sin(0°) = 0,  sin(-90°)*cos(0°) = -1
# 结果: front = (0, 0, -1) ✓
```

## Risk

| 风险 | 严重度 | 缓解 |
|------|--------|------|
| pitch 未 clamp 导致万向锁翻转 | LOW | `std::clamp(pitch, -89.f, 89.f)` 已在设计中 |

## Acceptance Criteria

- [ ] `src/camera.h` 中 7 处 TODO 均已实现
- [ ] 编译零错误
- [ ] 初始 `front` 向量为 (0, 0, -1)（yaw=-90°, pitch=0° 的数学结果）
- [ ] `getViewMatrix()` 不再返回单位矩阵
