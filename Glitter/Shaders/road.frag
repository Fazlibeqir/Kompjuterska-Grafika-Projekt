#version 330 core

in vec2 TexCoords; // Received texture coordinates from vertex shader
out vec4 FragColor;

uniform sampler2D roadTexture; // Road texture

void main() {
    // Sample the road texture
    vec4 roadColor = texture(roadTexture, TexCoords);

    // Apply the road color to the fragment
    FragColor = roadColor;
}