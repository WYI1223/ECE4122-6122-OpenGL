# PR-HW3-01: Shader Compilation & Linking

- Proposed title: `feat(shader): implement GLSL compile, link, and constructor`
- Status: Draft

## Goal

实现 Shader 类的核心功能：编译顶点/片元着色器源码、链接为 program、构造函数串联整个流程。这是所有渲染的前置基础。

前置条件：无

## Execution Contract (Canonical Inputs)

| 类型 | 引用 | 与本 PR 的关系 |
|------|------|---------------|
| skeleton | `src/shader.h` | 包含 3 个 TODO 函数需要实现 |
| shader source | `shaders/object.vert` | 构造函数需要读取并编译此文件 |
| shader source | `shaders/object.frag` | 构造函数需要读取并编译此文件 |

## Scope

In scope:
- 实现 `compile()` — 创建 shader 对象、上传源码、编译、检查错误
- 实现 `checkLink()` — 检查 program 链接状态、打印错误日志
- 实现构造函数 — 读取文件、编译、创建 program、attach、link、清理

Out of scope:
- uniform setter（已提供，不修改）
- readFile（已提供，不修改）
- 其他文件的任何修改

## Design

```cpp
// compile()
static GLuint compile(GLenum type, const char* src, const char* label)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char buf[512];
        glGetShaderInfoLog(shader, 512, nullptr, buf);
        std::cerr << "[Shader] " << label << " compile error:\n" << buf << "\n";
    }
    return shader;
}

// checkLink()
static void checkLink(GLuint prog)
{
    int success;
    glGetProgramiv(prog, GL_LINK_STATUS, &success);
    if (!success) {
        char buf[512];
        glGetProgramInfoLog(prog, 512, nullptr, buf);
        std::cerr << "[Shader] link error:\n" << buf << "\n";
    }
}

// Constructor
Shader(const char* vertPath, const char* fragPath)
{
    std::string vCode = readFile(vertPath);
    std::string fCode = readFile(fragPath);

    GLuint vs = compile(GL_VERTEX_SHADER,   vCode.c_str(), vertPath);
    GLuint fs = compile(GL_FRAGMENT_SHADER, fCode.c_str(), fragPath);

    id = glCreateProgram();
    glAttachShader(id, vs);
    glAttachShader(id, fs);
    glLinkProgram(id);
    checkLink(id);

    glDeleteShader(vs);
    glDeleteShader(fs);
}
```

## Task Breakdown

| Task | Lane | 内容 | 文件 | 估算 | 依赖 |
|------|------|------|------|------|------|
| T1 | impl | 实现 `compile()`, `checkLink()`, 构造函数 | `src/shader.h` | 5min | 无 |

## Planned File Changes

- `[edit]` `src/shader.h` (填充 3 个 TODO 函数体)

## Verification

### CI gates

```bash
cd f:/Learn/ECE6122_hmk3
cmake -B build -S . && cmake --build build 2>&1 | tail -5
```

### Structural verification

```bash
# 运行程序，检查 console 输出
# 预期: 打印 OpenGL 版本和 GLEW 版本，无 "[Shader] compile error" 或 "[Shader] link error"
./build/bin/Hmk3_Skeleton.exe 2>&1 | head -10
```

## Risk

| 风险 | 严重度 | 缓解 |
|------|--------|------|
| shader 文件路径相对于 exe 工作目录，找不到文件 | MEDIUM | 确认 exe 运行时 cwd 为项目根目录，或 assets copy 机制正确 |

## Acceptance Criteria

- [ ] `src/shader.h` 中 `compile()`、`checkLink()`、构造函数三处 TODO 均已实现
- [ ] 编译零错误
- [ ] 程序启动后控制台无 `[Shader] compile error` 或 `[Shader] link error` 输出
- [ ] 程序启动后窗口打开（黑屏是预期的）
