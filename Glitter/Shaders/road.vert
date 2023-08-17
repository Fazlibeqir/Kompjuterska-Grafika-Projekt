#version 330 core

layout(location = 0) in vec3 aPos;
out vec2 TexCoords; // Pass texture coordinates to fragment shader

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    // Transform vertex position
    vec4 worldPos = model * vec4(aPos, 1.0);
    vec4 viewPos = view * worldPos;
    gl_Position = projection * viewPos;

    // Calculate texture coordinates
    TexCoords = aPos.xy * 0.5 + 0.5;
}