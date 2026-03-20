# PR-HW3-07: Fragment Shader Lighting Functions

- Proposed title: `feat(shader): implement Blinn-Phong directional and point light calculations`
- Status: Draft

## Goal

在片元着色器中实现 Blinn-Phong 光照的两个核心函数：方向光计算和点光源计算（含衰减），使场景具备完整的光照效果。

前置条件：PR-HW3-06 (drawModel/scene，需要可见几何体来验证光照效果)

## Execution Contract (Canonical Inputs)

| 类型 | 引用 | 与本 PR 的关系 |
|------|------|---------------|
| skeleton | `shaders/object.frag` L90-95 | `calcDirLight()` TODO |
| skeleton | `shaders/object.frag` L101-106 | `calcPointLight()` TODO |
| spec | `shaders/object.frag` L11-21 注释 | Blinn-Phong 公式定义 |

## Scope

In scope:
- `calcDirLight()` — ambient + diffuse + specular（Blinn-Phong 半向量）
- `calcPointLight()` — 同上 + 距离衰减

Out of scope:
- 着色器中其他代码（struct 定义、main、texture helpers 均已提供）
- 光照 uniform 的上传（Stage 8）

## Design

### 光照模型

使用 **Blinn-Phong**（非经典 Phong），区别在于 specular 计算：
- Phong: `reflect(-L, N)` 与 `V` 的点积
- Blinn-Phong: `H = normalize(L + V)` 与 `N` 的点积 ← 本作业要求

### calcDirLight()

```glsl
vec3 calcDirLight(DirLight light, vec3 norm, vec3 viewDir,
                  vec3 diffColor, vec3 specColor)
{
    // 取反：light.direction 是光的传播方向（从天到地），L 是表面到光源方向
    vec3 lightDir = normalize(-light.direction);

    // Diffuse: Lambert 余弦定律
    float diff = max(dot(norm, lightDir), 0.0);

    // Specular: Blinn-Phong 半向量
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfDir), 0.0), material.shininess);

    // 组合三项
    vec3 ambient  = light.ambient  * diffColor;
    vec3 diffuse  = light.diffuse  * diff * diffColor;
    vec3 specular = light.specular * spec * specColor;

    return ambient + diffuse + specular;
}
```

### calcPointLight()

```glsl
vec3 calcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir,
                    vec3 diffColor, vec3 specColor)
{
    // 光源方向：从片元指向光源
    vec3 lightDir = normalize(light.position - fragPos);

    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);

    // Specular (Blinn-Phong)
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfDir), 0.0), material.shininess);

    // 衰减
    float dist = length(light.position - fragPos);
    float atten = 1.0 / (light.constant + light.linear * dist
                         + light.quadratic * dist * dist);

    // 组合三项 × 衰减
    vec3 ambient  = light.ambient  * diffColor;
    vec3 diffuse  = light.diffuse  * diff * diffColor;
    vec3 specular = light.specular * spec * specColor;

    return atten * (ambient + diffuse + specular);
}
```

### 关键公式对照

| 分量 | 公式 | 说明 |
|------|------|------|
| ambient | `light.ambient * diffColor` | 环境光，与方向无关 |
| diffuse | `light.diffuse * max(dot(N, L), 0) * diffColor` | Lambert 漫反射 |
| specular | `light.specular * pow(max(dot(N, H), 0), shininess) * specColor` | Blinn-Phong 高光 |
| attenuation | `1 / (c + l*d + q*d²)` | 仅点光源，三项全乘 |

### light.direction 取反的原因

`DirLight.direction` 存储的是光线传播方向（例如 `(-0.5, -1.0, -0.5)` 表示从右上方照向左下方）。做 diffuse 计算时需要的是从表面指向光源的方向 `L`，因此 `L = normalize(-direction)`。

## Task Breakdown

| Task | Lane | 内容 | 文件 | 估算 | 依赖 |
|------|------|------|------|------|------|
| T1 | impl | 实现 `calcDirLight()` | `shaders/object.frag` | 3min | 无 |
| T2 | impl | 实现 `calcPointLight()` | `shaders/object.frag` | 3min | 无 |

## Planned File Changes

- `[edit]` `shaders/object.frag` (填充 `calcDirLight()` 和 `calcPointLight()` 函数体)

## Verification

### CI gates

```bash
cd f:/Learn/ECE6122_hmk3
cmake -B build -S . && cmake --build build 2>&1 | tail -5
# shader 编译错误会在运行时而非 cmake 时出现
```

### Structural verification

```bash
./build/bin/Hmk3_Skeleton.exe 2>&1 | grep -i "compile error\|link error"
# 预期: 无输出（无 shader 编译/链接错误）
```

视觉验证（需配合 Stage 8 光照 uniform 上传后完整验证）：
- 物体表面有明暗过渡（diffuse）
- 摄像机移动时有亮斑跟随变化（specular）
- 远离点光源的区域较暗（attenuation）

## Risk

| 风险 | 严重度 | 缓解 |
|------|--------|------|
| 忘记取反 `light.direction` 导致 diffuse 全黑 | HIGH | 设计中已明确 `normalize(-light.direction)` |
| shininess 为 0 导致 `pow(x, 0)` 全为 1 | LOW | main.cpp 中 `material.shininess` 默认设为 32.f |
| 未上传光照 uniform 时场景全黑 | LOW | 预期行为，Stage 8 解决 |

## Acceptance Criteria

- [ ] `calcDirLight()` 实现 ambient + diffuse + specular 三项计算
- [ ] `calcPointLight()` 实现 ambient + diffuse + specular + attenuation 四项计算
- [ ] 使用 Blinn-Phong 半向量 `H = normalize(L + V)` 而非 Phong `reflect`
- [ ] 方向光中 `light.direction` 被取反
- [ ] 编译零错误，运行时无 shader compile/link error
