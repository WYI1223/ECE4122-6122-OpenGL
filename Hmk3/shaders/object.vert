#version 330 core
// ─────────────────────────────────────────────────────────────────────────────
// object.vert  –  Starter vertex shader  (provided – no edits required)
//
// Transforms each vertex from object space to clip space and passes
// world-space position and normal to the fragment shader.
// ─────────────────────────────────────────────────────────────────────────────

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMatrix;  // transpose(inverse(model)), packed in mat4

void main()
{
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos   = worldPos.xyz;
    Normal    = mat3(normalMatrix) * aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * worldPos;
}
