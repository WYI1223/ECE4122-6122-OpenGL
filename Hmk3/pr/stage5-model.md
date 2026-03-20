# PR-HW3-05: Assimp Model Loading

- Proposed title: `feat(model): implement processMesh and loadMaterialTextures`
- Status: Draft

## Goal

实现 Model 类中从 Assimp 数据结构提取顶点/索引/纹理的核心逻辑，使 OBJ 文件能被正确加载为可绘制的 Mesh 对象。

前置条件：PR-HW3-04 (mesh)

## Execution Contract (Canonical Inputs)

| 类型 | 引用 | 与本 PR 的关系 |
|------|------|---------------|
| skeleton | `src/model.h` | 包含 `processMesh()` 和 `loadMaterialTextures()` 两个 TODO |
| dependency | `src/mesh.h` | `processMesh()` 返回 `Mesh` 对象 |
| assets | `assets/models/*/` | OBJ/MTL/texture 文件供加载测试 |

## Scope

In scope:
- `processMesh()` — 从 `aiMesh` 提取 vertices（position, normal, texCoords）、indices（faces）、textures（materials）
- `loadMaterialTextures()` — 从 `aiMaterial` 提取纹理路径、查缓存、调用 `loadTextureFromFile`

Out of scope:
- `load()`、`processNode()`、`loadTextureFromFile()`、`createFallbackTexture()`（均已提供）
- Model 类接口变更

## Design

### processMesh()

```cpp
Mesh processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex>  vertices;
    std::vector<GLuint>  indices;
    std::vector<Texture> textures;

    // --- Vertices ---
    for (unsigned i = 0; i < mesh->mNumVertices; ++i) {
        Vertex v;
        v.position = { mesh->mVertices[i].x,
                       mesh->mVertices[i].y,
                       mesh->mVertices[i].z };

        if (mesh->HasNormals()) {
            v.normal = { mesh->mNormals[i].x,
                         mesh->mNormals[i].y,
                         mesh->mNormals[i].z };
        } else {
            v.normal = { 0.f, 0.f, 0.f };
        }

        if (mesh->mTextureCoords[0]) {
            v.texCoords = { mesh->mTextureCoords[0][i].x,
                            mesh->mTextureCoords[0][i].y };
        } else {
            v.texCoords = { 0.f, 0.f };
        }

        vertices.push_back(v);
    }

    // --- Indices ---
    for (unsigned i = 0; i < mesh->mNumFaces; ++i) {
        aiFace& face = mesh->mFaces[i];
        for (unsigned j = 0; j < face.mNumIndices; ++j)
            indices.push_back(face.mIndices[j]);
    }

    // --- Textures ---
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

        auto diff = loadMaterialTextures(mat, aiTextureType_DIFFUSE,  "texture_diffuse");
        auto spec = loadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");
        auto emis = loadMaterialTextures(mat, aiTextureType_EMISSIVE, "texture_emissive");

        textures.insert(textures.end(), diff.begin(), diff.end());
        textures.insert(textures.end(), spec.begin(), spec.end());
        textures.insert(textures.end(), emis.begin(), emis.end());
    }

    return Mesh(std::move(vertices), std::move(indices), std::move(textures));
}
```

### loadMaterialTextures()

```cpp
std::vector<Texture> loadMaterialTextures(aiMaterial* mat,
                                          aiTextureType type,
                                          const std::string& typeName)
{
    std::vector<Texture> out;

    for (unsigned i = 0; i < mat->GetTextureCount(type); ++i) {
        aiString aiPath;
        mat->GetTexture(type, i, &aiPath);

        // 尝试原始相对路径
        std::string rawStr(aiPath.C_Str());
        std::replace(rawStr.begin(), rawStr.end(), '\\', '/');
        std::string rawPath = m_directory + "/" + rawStr;

        // 如果原始路径不存在，退回到仅文件名
        std::string fullPath = fs::exists(rawPath)
            ? rawPath
            : m_directory + "/" + fs::path(aiPath.C_Str()).filename().string();

        // 查缓存
        auto it = m_texCache.find(fullPath);
        if (it != m_texCache.end()) {
            out.push_back(it->second);
            continue;
        }

        // 加载新纹理
        GLuint texID = loadTextureFromFile(fullPath);
        Texture tex { texID, typeName, fullPath };
        m_texCache[fullPath] = tex;
        out.push_back(tex);
    }

    return out;
}
```

### 纹理路径解析策略

MTL 文件中的纹理路径有两种情况：
1. `textures/Farm_house_D.jpg` — 子目录路径 → 用 `m_directory + "/" + rawPath`
2. `barrel_BaseColor.png` — 仅文件名 → 用 `m_directory + "/" + filename`

设计中先尝试原始路径，不存在则退回文件名，覆盖两种情况。

## Task Breakdown

| Task | Lane | 内容 | 文件 | 估算 | 依赖 |
|------|------|------|------|------|------|
| T1 | impl | 实现 `processMesh()` | `src/model.h` | 8min | PR-HW3-04 |
| T2 | impl | 实现 `loadMaterialTextures()` | `src/model.h` | 5min | T1 |

## Planned File Changes

- `[edit]` `src/model.h` (填充 `processMesh()` 和 `loadMaterialTextures()` 函数体)

## Verification

### CI gates

```bash
cd f:/Learn/ECE6122_hmk3
cmake -B build -S . && cmake --build build 2>&1 | tail -5
```

### Structural verification

```bash
# 运行程序，检查模型加载日志
./build/bin/Hmk3_Skeleton.exe 2>&1 | grep "\[Model\]"
# 预期输出:
#   [Model] Loaded: ./assets/models/lamp/objLamp.obj (N meshes)
#   [Model] Loaded: ./assets/models/robot/Robot.obj (N meshes)
# 无 "[Model] Assimp error" 行
```

## Risk

| 风险 | 严重度 | 缓解 |
|------|--------|------|
| MTL 中纹理路径含子目录，`filename()` 丢失路径导致纹理找不到 | HIGH | 先尝试原始路径，退回 filename（双重策略） |
| robot 模型含 .tif 纹理，stb_image 默认不支持 | MEDIUM | `loadTextureFromFile` 已有 fallback 灰色纹理，不会崩溃 |
| `mMaterialIndex >= 0` 比较无符号数永远为 true 的编译器警告 | LOW | 保留原始检查，不影响正确性 |

## Acceptance Criteria

- [ ] `processMesh()` 正确提取 vertices、indices、textures
- [ ] `loadMaterialTextures()` 正确解析纹理路径并使用缓存
- [ ] 编译零错误
- [ ] 控制台输出 `[Model] Loaded: ...` 且无 `[Model] Assimp error`
- [ ] 纹理找不到时 fallback 为灰色纹理（不崩溃）
