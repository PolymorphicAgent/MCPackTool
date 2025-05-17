#version 330 core

in vec3 v_normal;
in vec2 v_texcoord;

uniform sampler2D u_texture;
uniform vec3 u_lightDir;  // Should be normalized (e.g. vec3(0.5, 1.0, 0.8))

out vec4 fragColor;

void main() {
    // Sample the block’s texture
    vec4 texel = texture(u_texture, v_texcoord);

    // Simple Lambertian diffuse (clamped to [0.5,1.0] so faces aren’t too dark)
    float NdotL = max(dot(normalize(v_normal), normalize(u_lightDir)), 0.0);
    float light = mix(0.5, 1.0, NdotL);

    // Apply lighting to RGB, preserve alpha
    fragColor = vec4(texel.rgb * light, texel.a);
}