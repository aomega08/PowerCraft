#include "stdafx.h"
#include "VertexBufferObject.h"
#include "FragmentShader.h"
#include "VertexShader.h"
#include "ShaderProgram.h"
#include "VertexArrayObject.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

void SetupHints() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
}

int main() {
	int width, height;
	glfwInit();
	SetupHints();

	GLFWwindow *window = glfwCreateWindow(640, 480, "PowerCraft", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "Failed GLEW initialization" << std::endl;
		return -1;
	}

	glfwSwapInterval(1);

	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	
	float vertices[] = {
		0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.25f, 0.25f,
		0.25f, -0.25f,
		0.25f, 0.25f,
		-0.25f, 0.25f,
		-0.25f, -0.25f,
		0.25f, -0.25f
	};

	VertexBufferObject vbo;
	
	vbo.Bind();
	vbo.Upload(vertices, sizeof(vertices), GL_STATIC_DRAW);

	VertexShader vshader("Shaders/shader.vert");
	FragmentShader fshader("Shaders/shader.frag");
	vshader.Compile();
	fshader.Compile();
	
	ShaderProgram program;
	program.Attach(vshader);
	program.Attach(fshader);
	program.Link();

	program.Use();
	program.SetupAttribute("texCoord", 2, GL_FLOAT, GL_FALSE, 0, 0);
	program.SetupAttribute("position", 2, GL_FLOAT, GL_FALSE, 0, 12 * sizeof(GL_FLOAT));

	int tw, th;
	unsigned char* image = SOIL_load_image("dirt.png", &tw, &th, 0, SOIL_LOAD_RGB);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tw, th, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(program._id, "ourTexture"), 0);

	while (!glfwWindowShouldClose(window)) {
		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(program._id, "ourTexture"), 0);*/

		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glFinish();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}


#ifdef WIN32
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	return main();
}
#endif