# PR-HW3-06: drawModel, Terrain Rendering & Scene Placement

- Proposed title: `feat(main): implement drawModel, terrain draw, and scene object placement`
- Status: Draft

## Goal

实现 `drawModel()` 函数上传变换矩阵 uniform，在渲染循环中绘制地面和所有场景物体，完成几何渲染管线的闭环。

前置条件：PR-HW3-01 (shader), PR-HW3-02 (camera), PR-HW3-03 (input/renderloop), PR-HW3-04 (mesh), PR-HW3-05 (model)

## Execution Contract (Canonical Inputs)

| 类型 | 引用 | 与本 PR 的关系 |
|------|------|---------------|
| skeleton | `src/main.cpp` L209-216 | `drawModel()` TODO |
| skeleton | `src/main.cpp` L276-327 | 模型加载 + 渲染循环中 draw 调用 |
| spec | `src/main.cpp` L317-324 注释 | 物体坐标/旋转/缩放的指定值 |
| assets | `assets/models/*/` | 7 个 OBJ 模型 |

## Scope

In scope:
- `drawModel()` — 上传 model/view/projection/normalMatrix uniform，调用 `model.draw()`
- 取消注释全部 Model 加载行，补充缺失模型
- 渲染循环中绘制地面 VAO
- 渲染循环中按指定坐标放置并绘制全部场景物体

Out of scope:
- setLightUniforms（Stage 8）
- 地面纹理加载（Stage 9）
- 模型缩放调优（Stage 9）

## Design

### drawModel()

```cpp
static void drawModel(Model& model, Shader& sh,
                      const glm::mat4& modelMat,
                      const glm::mat4& view,
                      const glm::mat4& proj)
{
    sh.setMat4("model", modelMat);
    sh.setMat4("view", view);
    sh.setMat4("projection", proj);

    glm::mat3 nm = glm::transpose(glm::inverse(glm::mat3(modelMat)));
    sh.setMat4("normalMatrix", glm::mat4(nm));

    model.draw(sh);
}
```

### 取消注释模型加载

```cpp
Model mFarmhouse("./assets/models/farmhouse/Farm_house.obj");
Model mBarrel   ("./assets/models/barrel/Barrel_OBJ.obj");
Model mTree     ("./assets/models/pinetree/Pine_Tree.obj");
Model mBench    ("./assets/models/bench/wooden_bench.obj");
Model mLamp     ("./assets/models/lamp/objLamp.obj");
Model mRobot    ("./assets/models/robot/Robot.obj");
Model mFarmer   ("./assets/models/farmer/farmer.obj");
```

### 地面绘制

```cpp
{
    glm::mat4 terrainM(1.0f);
    glm::mat3 terrainNM = glm::transpose(glm::inverse(glm::mat3(terrainM)));

    objShader.setMat4("model", terrainM);
    objShader.setMat4("view", view);
    objShader.setMat4("projection", proj);
    objShader.setMat4("normalMatrix", glm::mat4(terrainNM));

    objShader.setInt("material.hasDiffuse", 0);
    objShader.setInt("material.hasSpecular", 0);
    objShader.setInt("material.hasEmissive", 0);

    glBindVertexArray(g_terrainVAO);
    glDrawElements(GL_TRIANGLES, g_terrainIndexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
```

### 场景物体放置

使用 lambda 构建 model 矩阵：

```cpp
auto makeModel = [](glm::vec3 pos, float rotDeg, glm::vec3 rotAxis, glm::vec3 scl) {
    glm::mat4 M = glm::translate(glm::mat4(1.f), pos);
    M = glm::rotate(M, glm::radians(rotDeg), rotAxis);
    M = glm::scale(M, scl);
    return M;
};
glm::vec3 Y(0, 1, 0);
```

| 物体 | 位置 | 旋转 | 缩放 | 备注 |
|------|------|------|------|------|
| Farmhouse | (0, 0, -10) | 0° | (1, 1, 1) | 主建筑 |
| Barrel 1 | (3.5, 0, -8) | 0° | (1, 1, 1) | |
| Barrel 2 | (4.5, 0, -8) | 15° | (1, 1, 1) | 旋转 ✓ |
| Tree ×4 | (±12, 0, ±12) | 10°/25°/35°/45° | (1, 1, 1) | 旋转 ✓×4 |
| Bench | (-6, 0, 2) | -30° | (1, 1, 1) | 旋转 ✓ |
| Lamp ×2 | (5,0,0) / (-5,0,5) | 0° | (0.5, 0.5, 0.5) | |
| Robot | (-3, 0, -4) | 180° | (1, 1, 1) | 旋转 ✓ |
| Farmer | (2, 0, 0) | 90° | (1, 1, 1) | 旋转 ✓ |

旋转物体数量: 8 ≥ 3 ✓

**注意**: 缩放值为初始占位值，Stage 9 会视觉调优。非均匀缩放也在 Stage 9 确认。

## Task Breakdown

| Task | Lane | 内容 | 文件 | 估算 | 依赖 |
|------|------|------|------|------|------|
| T1 | impl | 实现 `drawModel()` | `src/main.cpp` | 3min | PR-HW3-01,04,05 |
| T2 | impl | 取消注释模型加载 + 补充 farmer | `src/main.cpp` | 2min | PR-HW3-05 |
| T3 | impl | 地面绘制代码 | `src/main.cpp` | 3min | T1 |
| T4 | impl | 场景物体 drawModel 调用 | `src/main.cpp` | 5min | T1, T2 |

## Planned File Changes

- `[edit]` `src/main.cpp` (实现 drawModel + 取消注释模型加载 + 渲染循环中绘制地面和物体)

## Verification

### CI gates

```bash
cd f:/Learn/ECE6122_hmk3
cmake -B build -S . && cmake --build build 2>&1 | tail -5
```

### Structural verification

```bash
./build/bin/Hmk3_Skeleton.exe
# 预期行为:
# 1. 所有模型加载日志出现
# 2. 地面可见（灰色平面）
# 3. 7 个模型在不同位置渲染
# 4. WASD + 右键拖拽可自由浏览场景
# 5. 滚轮缩放 FOV
# 6. R 重置视角
# 7. ESC 退出
# 8. 物体可能为灰色/无光照（预期 — 光照是 Stage 7+8）
```

## Risk

| 风险 | 严重度 | 缓解 |
|------|--------|------|
| 模型缩放差异巨大，部分物体看不见或占满屏幕 | MEDIUM | 先用默认 scale 1.0 观察，Stage 9 再调优 |
| normalMatrix 打包到 mat4 时丢失数据 | LOW | `glm::mat4(nm)` 自动将 mat3 放在左上角 3×3，正确 |
| 模型加载时间过长导致窗口长时间无响应 | LOW | 所有加载在渲染循环前完成，属正常行为 |

## Acceptance Criteria

- [ ] `drawModel()` 正确上传 model/view/projection/normalMatrix 四个 uniform
- [ ] 7 个模型全部加载成功（控制台日志确认）
- [ ] 地面平面可见
- [ ] 所有模型在窗口中可见，位于不同位置
- [ ] WASD 移动 + 右键拖拽旋转 + 滚轮缩放 + ESC 退出 + R 重置 均正常工作
- [ ] 编译零错误
