# PR-HW3-08: Light Uniform Upload

- Proposed title: `feat(main): implement setLightUniforms for directional and point lights`
- Status: Draft

## Goal

在 `setLightUniforms()` 中上传方向光（太阳）和 2 个点光源的全部参数到着色器 uniform，完成光照系统的最后一环。

前置条件：PR-HW3-07 (fragment lighting), PR-HW3-06 (drawModel/scene)

## Execution Contract (Canonical Inputs)

| 类型 | 引用 | 与本 PR 的关系 |
|------|------|---------------|
| skeleton | `src/main.cpp` L188-197 | `setLightUniforms()` TODO |
| spec | `src/main.cpp` L169-184 注释 | 光源参数的具体数值 |
| dependency | `shaders/object.frag` L43-62 | uniform 名称必须匹配 shader 中的声明 |

## Scope

In scope:
- 实现 `setLightUniforms()` — 上传 1 个方向光 + 2 个点光源的全部 uniform
- 在渲染循环中调用 `setLightUniforms(objShader)`

Out of scope:
- 光照计算逻辑（已在 Stage 7 完成）
- 光源参数的动态调节/GUI

## Design

### setLightUniforms()

```cpp
static void setLightUniforms(Shader& sh)
{
    // --- 方向光 (late-afternoon sun) ---
    sh.setVec3("dirLight.direction", -0.5f, -1.0f, -0.5f);
    sh.setVec3("dirLight.ambient",    0.3f,  0.24f, 0.18f);
    sh.setVec3("dirLight.diffuse",    1.0f,  0.8f,  0.6f);
    sh.setVec3("dirLight.specular",   0.5f,  0.5f,  0.5f);

    // --- 点光源 0: 路灯 (5, 4, 0) ---
    sh.setVec3 ("pointLights[0].position",  5.f, 4.f, 0.f);
    sh.setVec3 ("pointLights[0].ambient",   0.05f, 0.05f, 0.05f);
    sh.setVec3 ("pointLights[0].diffuse",   0.9f,  0.9f,  1.0f);
    sh.setVec3 ("pointLights[0].specular",  0.5f,  0.5f,  0.5f);
    sh.setFloat("pointLights[0].constant",  1.0f);
    sh.setFloat("pointLights[0].linear",    0.09f);
    sh.setFloat("pointLights[0].quadratic", 0.032f);

    // --- 点光源 1: 路灯 (-5, 4, 5) ---
    sh.setVec3 ("pointLights[1].position", -5.f, 4.f, 5.f);
    sh.setVec3 ("pointLights[1].ambient",   0.05f, 0.05f, 0.05f);
    sh.setVec3 ("pointLights[1].diffuse",   0.9f,  0.9f,  1.0f);
    sh.setVec3 ("pointLights[1].specular",  0.5f,  0.5f,  0.5f);
    sh.setFloat("pointLights[1].constant",  1.0f);
    sh.setFloat("pointLights[1].linear",    0.09f);
    sh.setFloat("pointLights[1].quadratic", 0.032f);
}
```

### Uniform 名称对照

| C++ 调用中的 string | Shader 声明 | 类型 |
|---------------------|-------------|------|
| `"dirLight.direction"` | `uniform DirLight dirLight;` → `.direction` | vec3 |
| `"dirLight.ambient"` | `.ambient` | vec3 |
| `"dirLight.diffuse"` | `.diffuse` | vec3 |
| `"dirLight.specular"` | `.specular` | vec3 |
| `"pointLights[0].position"` | `uniform PointLight pointLights[2];` → `[0].position` | vec3 |
| `"pointLights[0].constant"` | `[0].constant` | float |
| `"pointLights[0].linear"` | `[0].linear` | float |
| `"pointLights[0].quadratic"` | `[0].quadratic` | float |

### 渲染循环中的调用位置

```cpp
objShader.use();
objShader.setVec3("viewPos", g_camera.position);
objShader.setFloat("material.shininess", 32.f);
setLightUniforms(objShader);   // ← 添加这一行
```

## Task Breakdown

| Task | Lane | 内容 | 文件 | 估算 | 依赖 |
|------|------|------|------|------|------|
| T1 | impl | 实现 `setLightUniforms()` 函数体 | `src/main.cpp` | 5min | PR-HW3-07 |
| T2 | impl | 在渲染循环中添加 `setLightUniforms()` 调用 | `src/main.cpp` | 1min | T1 |

## Planned File Changes

- `[edit]` `src/main.cpp` (填充 `setLightUniforms()` + 渲染循环中添加调用)

## Verification

### CI gates

```bash
cd f:/Learn/ECE6122_hmk3
cmake -B build -S . && cmake --build build 2>&1 | tail -5
```

### Structural verification

```bash
./build/bin/Hmk3_Skeleton.exe
```

视觉验证清单：
```
1. 场景整体被暖色调方向光照亮（偏橙黄色）     → diffuse 工作
2. 物体背光面较暗但不全黑                      → ambient 工作
3. 移动摄像机时物体表面亮斑位置变化              → specular 工作
4. 路灯位置 (5,4,0) 和 (-5,4,5) 附近明显更亮    → point light 工作
5. 远离点光源的物体不受点光影响                  → attenuation 工作
```

## Risk

| 风险 | 严重度 | 缓解 |
|------|--------|------|
| uniform 名称拼写错误导致光照参数未生效（silent failure） | HIGH | 逐字对照 shader 声明，注意 `pointLights` 复数形式和 `[0]`/`[1]` 索引 |
| ambient 设太高导致场景过亮无对比度 | LOW | 设为 diffuse 的 ~30%，可后续微调 |

## Acceptance Criteria

- [ ] `setLightUniforms()` 上传方向光 4 个属性和每个点光源 7 个属性（共 18 个 uniform 调用）
- [ ] 渲染循环中 `setLightUniforms()` 在 `objShader.use()` 之后被调用
- [ ] uniform 名称与 `shaders/object.frag` 中的声明完全匹配
- [ ] 编译零错误
- [ ] 场景有明显的明暗变化（非纯色）
- [ ] 移动摄像机时 specular 高光位置变化可见
