# PR-HW3-04: Mesh GPU Upload & Draw

- Proposed title: `feat(mesh): implement VAO/VBO/EBO setup and draw call`
- Status: Draft

## Goal

实现 Mesh 类的 GPU 资源上传（VAO/VBO/EBO 创建 + 顶点属性描述）和绘制方法（纹理绑定 + glDrawElements）。

前置条件：无（不依赖其他 Stage，可并行开发）

## Execution Contract (Canonical Inputs)

| 类型 | 引用 | 与本 PR 的关系 |
|------|------|---------------|
| skeleton | `src/mesh.h` | 包含 `setupGPU()` 和 `draw()` 两个 TODO |
| struct | `src/mesh.h` L24-28 | `Vertex` 结构体定义（position, normal, texCoords） |
| struct | `src/mesh.h` L31-35 | `Texture` 结构体定义（id, type, path） |

## Scope

In scope:
- `setupGPU()` — glGen / glBind / glBufferData / glVertexAttribPointer / glEnable
- `draw()` — 遍历纹理、设置 sampler uniform、设 has-flags、glDrawElements

Out of scope:
- Vertex/Texture 结构体定义（已提供）
- 构造函数、析构函数（已提供）
- move 语义（已提供）

## Design

### setupGPU()

```cpp
void setupGPU()
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(Vertex),
                 vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(GLuint),
                 indices.data(), GL_STATIC_DRAW);

    // location 0: position (vec3, offset 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, position));

    // location 1: normal (vec3, offset 12)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, normal));

    // location 2: texCoords (vec2, offset 24)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}
```

### draw()

```cpp
void draw(Shader& shader) const
{
    unsigned diffuseN  = 0;
    unsigned specularN = 0;
    unsigned emissiveN = 0;

    for (unsigned i = 0; i < textures.size(); ++i) {
        glActiveTexture(GL_TEXTURE0 + i);

        std::string name;
        if (textures[i].type == "texture_diffuse")
            name = "material.diffuse" + std::to_string(diffuseN++);
        else if (textures[i].type == "texture_specular")
            name = "material.specular" + std::to_string(specularN++);
        else if (textures[i].type == "texture_emissive")
            name = "material.emissive" + std::to_string(emissiveN++);

        shader.setInt(name, i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    shader.setInt("material.hasDiffuse",  diffuseN  > 0 ? 1 : 0);
    shader.setInt("material.hasSpecular", specularN > 0 ? 1 : 0);
    shader.setInt("material.hasEmissive", emissiveN > 0 ? 1 : 0);

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}
```

### 关键数据布局

```
Vertex (32 bytes):
  offset  0: position  (vec3, 12 bytes)
  offset 12: normal    (vec3, 12 bytes)
  offset 24: texCoords (vec2,  8 bytes)
```

## Task Breakdown

| Task | Lane | 内容 | 文件 | 估算 | 依赖 |
|------|------|------|------|------|------|
| T1 | impl | 实现 `setupGPU()` | `src/mesh.h` | 5min | 无 |
| T2 | impl | 实现 `draw()` | `src/mesh.h` | 5min | 无 |

## Planned File Changes

- `[edit]` `src/mesh.h` (填充 `setupGPU()` 和 `draw()` 函数体)

## Verification

### CI gates

```bash
cd f:/Learn/ECE6122_hmk3
cmake -B build -S . && cmake --build build 2>&1 | tail -5
# 预期: 编译成功，无错误
```

### Structural verification

无法独立视觉验证，需要配合 Stage 5 (Model) + Stage 6 (drawModel) 才能看到模型渲染结果。

## Risk

| 风险 | 严重度 | 缓解 |
|------|--------|------|
| EBO 在 VAO 解绑前被解绑导致渲染失败 | MEDIUM | `setupGPU()` 中不单独解绑 EBO，仅 `glBindVertexArray(0)` |
| Mesh move 后 VAO handle 被析构函数 delete | LOW | 已由提供的 move constructor 处理 |

## Acceptance Criteria

- [ ] `setupGPU()` 正确创建 VAO/VBO/EBO 并配置 3 个顶点属性
- [ ] `draw()` 正确绑定纹理、设置 uniform、调用 `glDrawElements`
- [ ] 编译零错误
- [ ] 不单独解绑 EBO（EBO 状态由 VAO 管理）
