#version 330

in vec2 position;
in vec2 texCoord;
in vec3 color;

out vec2 outTexCoord;
out vec3 Color;

uniform mat4 trans;
uniform mat4 view;
uniform mat4 proj;

void main() {
	gl_Position = proj * view * trans * vec4(position, 0.0, 1.0);
	outTexCoord = texCoord;
	Color = color;
}
