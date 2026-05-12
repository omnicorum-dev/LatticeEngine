#version 330 core

in vec2 vTexCoord;
in vec3 vNormal;
in vec3 vFragPos;
in vec3 vTangent;
in vec3 vBitangent;

out vec4 FragColor;

uniform sampler2D u_albedo;
uniform sampler2D u_normal;
uniform sampler2D u_roughness;
uniform sampler2D u_metallic;
uniform sampler2D u_ao;

uniform vec3 u_lightDir;
uniform vec3 u_lightColor;
uniform vec3 u_camPos;

const float PI = 3.14159265359;

// --- PBR functions ---

// normal distribution: how many microfacets are aligned to the halfway vector
float distributionGGX(vec3 N, vec3 H, float roughness) {
    float a  = roughness * roughness;
    float a2 = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float denom  = (NdotH * NdotH * (a2 - 1.0) + 1.0);
    return a2 / (PI * denom * denom);
}

// geometry: self-shadowing of microfacets
float geometrySchlickGGX(float NdotV, float roughness) {
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return NdotV / (NdotV * (1.0 - k) + k);
}
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    return geometrySchlickGGX(max(dot(N, V), 0.0), roughness)
    * geometrySchlickGGX(max(dot(N, L), 0.0), roughness);
}

// fresnel: reflectance increases at grazing angles
vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main() {
    // --- sample textures ---
    vec3  albedo    = pow(texture(u_albedo,    vTexCoord).rgb, vec3(2.2)); // sRGB -> linear
    float roughness = texture(u_roughness, vTexCoord).r;
    float metallic  = texture(u_metallic,  vTexCoord).r;
    float ao        = texture(u_ao,        vTexCoord).r;

    // --- normal mapping ---
    // transform sampled normal from [0,1] to [-1,1] tangent space, then to world space
    vec3 normalSample = texture(u_normal, vTexCoord).rgb * 2.0 - 1.0;
    vec3 N = normalize(vNormal);
    vec3 T = normalize(vTangent - dot(vTangent, N) * N);
    vec3 B = cross(N, T);
    mat3 TBN = mat3(T, B, N);
    N = normalize(TBN * normalSample);

    vec3 V = normalize(u_camPos - vFragPos);  // view vector
    vec3 L = normalize(-u_lightDir);          // toward light
    vec3 H = normalize(V + L);                // halfway vector

    // F0 is base reflectance at normal incidence
    // 0.04 is a reasonable default for non-metals; metals use their albedo color
    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    // --- Cook-Torrance BRDF ---
    float NDF = distributionGGX(N, H, roughness);
    float G   = geometrySmith(N, V, L, roughness);
    vec3  F   = fresnelSchlick(max(dot(H, V), 0.0), F0);

    vec3  num         = NDF * G * F;
    float denom       = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3  specular    = num / denom;

    // kS is specular fraction, kD is diffuse fraction
    // metals have no diffuse (energy is absorbed into the metal)
    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);

    float NdotL   = max(dot(N, L), 0.0);
    vec3 radiance = u_lightColor;

    vec3 ambient = vec3(0.02) * albedo * ao;
    vec3 color = ambient + (kD * albedo / PI + specular) * radiance * NdotL;

    // gamma correct (linear -> sRGB)
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}