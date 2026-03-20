# PR-HW3-09: Polish, Ground Texture, Scale Tuning & Submission

- Proposed title: `chore(polish): add ground texture, tune scales, finalize submission`
- Status: Draft

## Goal

完成所有收尾工作：地面纹理、模型缩放调优、满足评分 checklist 中的全部量化要求、编写 README、打包提交。

前置条件：PR-HW3-08 (light uniforms — 此时所有核心功能已完成)

## Execution Contract (Canonical Inputs)

| 类型 | 引用 | 与本 PR 的关系 |
|------|------|---------------|
| asset | `assets/models/pinetree/Texture/grass.jpg` | 地面纹理来源 |
| spec | PDF Section 2.1 | 旋转 ≥3、非均匀缩放 ≥2 的量化要求 |
| spec | PDF Section 2.5 | 3+ 物体有纹理、地面平铺纹理 |
| spec | PDF Section 7 | 提交格式要求 |

## Scope

In scope:
- 地面纹理加载和绑定
- 模型缩放值视觉调优
- 确保至少 2 个物体有非均匀缩放
- 编写 `README.md`
- 构建验证（clean build）
- 打包提交

Out of scope:
- Extra credit（skybox、fog、shadow mapping — 如有余力可做，但不在本 spec 范围内）
- 演示视频录制（手动操作）

## Design

### 9.1 地面纹理

在 `main()` 中模型加载之后、渲染循环之前：

```cpp
GLuint groundTex = 0;
{
    int w, h, ch;
    unsigned char* data = stbi_load(
        "./assets/models/pinetree/Texture/grass.jpg", &w, &h, &ch, 0);
    if (data) {
        GLenum fmt = (ch == 4) ? GL_RGBA : GL_RGB;
        glGenTextures(1, &groundTex);
        glBindTexture(GL_TEXTURE_2D, groundTex);
        glTexImage2D(GL_TEXTURE_2D, 0, (GLint)fmt, w, h, 0,
                     fmt, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
    }
}
```

在地面绘制代码中，`glBindVertexArray(g_terrainVAO)` 之前：

```cpp
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, groundTex);
objShader.setInt("material.diffuse0", 0);
objShader.setInt("material.hasDiffuse", 1);
objShader.setInt("material.hasSpecular", 0);
objShader.setInt("material.hasEmissive", 0);
```

注意：`buildTerrain()` 中 UV 坐标范围为 `[0, 20]`（TILE=20），配合 `GL_REPEAT` 实现 20×20 的平铺效果。

### 9.2 非均匀缩放

至少 2 个物体使用不同的 x/y/z 缩放因子。示例：

```cpp
// Farmhouse: 横向拉宽
makeModel({0,0,-10}, 0.f, Y, {1.5f, 1.0f, 1.5f})

// Bench: 沿长度方向拉伸
makeModel({-6,0,2}, -30.f, Y, {1.2f, 1.0f, 0.8f})
```

具体值需在运行时视觉调优。

### 9.3 评分验收清单

| 要求 | 分值 | 验证方法 |
|------|------|----------|
| 5+ distinct OBJ 物体 | 30 | 计数模型 ≥ 5（当前 7 个） |
| ≥3 物体非零旋转 | (15) | Barrel2, 4×Tree, Bench, Robot, Farmer = 8 个 |
| ≥2 物体非均匀缩放 | (15) | 确认 2 个物体 scale.x ≠ scale.y 或 ≠ scale.z |
| 地面有纹理 | (10) | 草地纹理平铺 |
| 主题连贯 | (5) | 农场场景 |
| VAO/VBO/EBO 正确 | 25 | mesh.h setupGPU 实现 |
| 递归 aiNode 遍历 | (15) | processNode 已提供且递归 |
| normal matrix 正确 | (10) | `transpose(inverse(mat3(model)))` |
| specular 随摄像机变化 | (10) | 视觉确认 |
| WASD + 鼠标摄像机 | 15 | 功能测试 |
| 3+ 物体有纹理 | 10 | farmhouse, barrel, pinetree, robot, bench |

## Task Breakdown

| Task | Lane | 内容 | 文件 | 估算 | 依赖 |
|------|------|------|------|------|------|
| T1 | impl | 地面纹理加载和绑定 | `src/main.cpp` | 5min | PR-HW3-08 |
| T2 | tune | 模型缩放值视觉调优 | `src/main.cpp` | 15min | T1 |
| T3 | tune | 确认 ≥2 物体非均匀缩放 | `src/main.cpp` | 2min | T2 |
| T4 | doc | 编写 README.md | `README.md` | 10min | 无 |
| T5 | verify | clean build 验证 | — | 3min | T1-T3 |
| T6 | submit | 打包 zip | — | 2min | T4, T5 |

## Planned File Changes

- `[edit]` `src/main.cpp` (地面纹理加载 + 缩放调优)
- `[add]` `README.md` (构建说明、控制方式、资源列表)

## Verification

### CI gates

```bash
# Clean build 验证
cd f:/Learn/ECE6122_hmk3
rm -rf build
cmake -B build -S . && cmake --build build 2>&1 | tail -10
# 预期: 编译成功，零错误
```

### Structural verification

```bash
./build/bin/Hmk3_Skeleton.exe
```

```
视觉验证:
1. 地面显示重复草地纹理                         ✓/✗
2. 所有 7 个物体可见且大小协调                    ✓/✗
3. 至少 2 个物体明显非等比缩放                    ✓/✗
4. 暖色调方向光照亮场景                          ✓/✗
5. 路灯附近有明显局部亮光                        ✓/✗
6. 摄像机移动时高光变化                          ✓/✗
7. WASD/右键拖拽/滚轮/ESC/R 全部正常             ✓/✗
```

```bash
# 检查打包内容
ls README.md
# 预期: 文件存在

# 打包（不含 build/、pdf、pr/）
zip -r Lastname_Firstname_HW3.zip \
    src/ shaders/ assets/ CMakeLists.txt README.md stb_image.h \
    -x "build/*" "*.o" "*.exe" "pr/*" "*.pdf"
```

## Risk

| 风险 | 严重度 | 缓解 |
|------|--------|------|
| stb_image.h 已在 model.h 中定义 STB_IMAGE_IMPLEMENTATION，main.cpp 中再调用 stbi_load 导致重复定义 | HIGH | stbi_load 是头文件中的函数，model.h 已 define IMPLEMENTATION 且 model.h 被 main.cpp include → 同一翻译单元，不会重复定义 |
| 模型缩放调优耗时长 | MEDIUM | 设定 15 分钟上限，优先保证功能正确而非视觉完美 |
| grass.jpg 路径变化导致加载失败 | LOW | 加载失败时 groundTex=0，地面显示为无纹理灰色，不崩溃 |

## Acceptance Criteria

- [ ] 地面显示平铺草地纹理
- [ ] 至少 2 个物体有非均匀缩放（scale.x ≠ scale.y 或 ≠ scale.z）
- [ ] 至少 3 个物体有贴图纹理
- [ ] clean build（删除 build 目录后重新 cmake + build）零错误
- [ ] `README.md` 存在且包含构建步骤和控制说明
- [ ] 评分 checklist 中所有量化要求均满足
