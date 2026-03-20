#version 330 core
// ─────────────────────────────────────────────────────────────────────────────
// object.frag  –  Phong shading  [SKELETON]
//
// The struct definitions, texture-sampling helpers, and main() skeleton are
// provided.  You must implement:
//   • calcDirLight()   – ambient + diffuse + specular for a directional light
//   • calcPointLight() – ambient + diffuse + specular + attenuation for a point light
//
// Light model (per light):
//   ambient  = light.ambient  * diffuseColor
//   diffuse  = light.diffuse  * max(dot(N, L), 0)      * diffuseColor
//   specular = light.specular * pow(max(dot(N, H), 0), shininess) * specularColor
//
//   where H = normalize(L + V)  (Blinn-Phong half-vector)
//         L = normalize(light direction toward surface)
//         V = normalize(viewPos - FragPos)
//
// Point light attenuation:
//   atten = 1 / (constant + linear * dist + quadratic * dist * dist)
//   Apply atten to all three terms (ambient, diffuse, specular).
// ─────────────────────────────────────────────────────────────────────────────

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// ── Material ──────────────────────────────────────────────────────────────────
struct Material {
    sampler2D diffuse0;
    sampler2D specular0;
    sampler2D emissive0;
    float     shininess;
    int hasDiffuse;
    int hasSpecular;
    int hasEmissive;
};
uniform Material material;

// ── Directional light ─────────────────────────────────────────────────────────
struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight dirLight;

// ── Point lights ─────────────────────────────────────────────────────────────
#define MAX_POINT_LIGHTS 2
struct PointLight {
    vec3  position;
    float constant;
    float linear;
    float quadratic;
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
};
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform vec3 viewPos;

// ── Texture helpers (provided) ────────────────────────────────────────────────
vec3 getDiffuseColor()
{
    if (material.hasDiffuse == 1)
        return texture(material.diffuse0, TexCoords).rgb;
    return vec3(0.7);
}

vec3 getSpecularColor()
{
    if (material.hasSpecular == 1)
        return texture(material.specular0, TexCoords).rgb;
    return vec3(0.3);
}

// ── calcDirLight ─────────────────────────────────────────────────────────────
// Parameters:
//   light     – the DirLight uniform
//   norm      – normalized surface normal (already computed in main)
//   viewDir   – normalized vector from fragment to camera
//   diffColor – result of getDiffuseColor()
//   specColor – result of getSpecularColor()
// Return the sum: ambient + diffuse + specular
// TODO: Implement.
vec3 calcDirLight(DirLight light, vec3 norm, vec3 viewDir,
                  vec3 diffColor, vec3 specColor)
{
    // TODO
    return vec3(0.0);
}

// ── calcPointLight ────────────────────────────────────────────────────────────
// Same as calcDirLight but light direction is computed from light.position,
// and all three terms are multiplied by the distance attenuation factor.
// TODO: Implement.
vec3 calcPointLight(PointLight light, vec3 norm, vec3 fragPos, vec3 viewDir,
                    vec3 diffColor, vec3 specColor)
{
    // TODO
    return vec3(0.0);
}

// ── main (provided) ───────────────────────────────────────────────────────────
void main()
{
    vec3 norm    = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 diffColor = getDiffuseColor();
    vec3 specColor = getSpecularColor();

    // Accumulate directional + point light contributions
    vec3 result = calcDirLight(dirLight, norm, viewDir, diffColor, specColor);

    for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
        result += calcPointLight(pointLights[i], norm, FragPos, viewDir,
                                 diffColor, specColor);

    // Emissive channel (lamp heads glow regardless of lighting)
    if (material.hasEmissive == 1)
        result += texture(material.emissive0, TexCoords).rgb;

    FragColor = vec4(result, 1.0);
}
