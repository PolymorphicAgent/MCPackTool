#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texcoord;

uniform mat4 u_projMatrix;
uniform mat4 u_modelViewMatrix;

out vec3 v_normal;
out vec2 v_texcoord;

void main() {
    // Transform to clip‐space
    gl_Position = u_projMatrix * u_modelViewMatrix * vec4(a_position, 1.0);

    // Pass UV straight through
    v_texcoord = a_texcoord;

    // Transform normal by the upper‐left 3×3 of the modelView
    // (you could also use a mat3(u_modelViewMatrix) if no non‐uniform scale)
    v_normal = mat3(u_modelViewMatrix) * a_normal;
}