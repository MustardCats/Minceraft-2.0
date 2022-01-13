#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec3 color;

void main() {
    vec4 tile_color = vec4(color, 1.0);
    FragColor = texture(ourTexture, TexCoord) * tile_color;
}