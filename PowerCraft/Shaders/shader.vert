#version 330

in vec2 position;
in vec2 texCoord;
in vec3 color;

out vec2 outTexCoord;
out vec3 Color;

void main() {
	gl_Position = vec4(position, 0.0, 1.0);
	outTexCoord = texCoord;
	Color = color;
}
