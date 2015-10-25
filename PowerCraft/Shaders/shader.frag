#version 330

in vec2 outTexCoord;
in vec3 Color;

out vec4 outColor;

uniform sampler2D ourTexture;

void main() {
	outColor = texture(ourTexture, outTexCoord) * vec4(Color, 1.0);
	outColor.a = 0.1;
}
