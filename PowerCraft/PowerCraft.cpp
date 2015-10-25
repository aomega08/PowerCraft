#include "stdafx.h"
#include "VertexBufferObject.h"
#include "FragmentShader.h"
#include "VertexShader.h"
#include "ShaderProgram.h"
#include "VertexArrayObject.h"
#include "Texture.h"

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
		-0.25f, 0.25f,   0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		0.25f, -0.25f,   1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
		0.25f, 0.25f,    1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
		-0.25f, 0.25f,   0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		-0.25f, -0.25f,  0.0f, 1.0f,  1.0f, 1.0f, 1.0f,
		0.25f, -0.25f,   1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
	};

	VertexBufferObject vbo;

	vbo.Bind();
	vbo.Upload(vertices, sizeof(vertices), GL_STATIC_DRAW);

	ShaderProgram program;

	try {
		VertexShader vshader("Shaders/shader.vert");
		FragmentShader fshader("Shaders/shader.frag");
		vshader.Compile();
		fshader.Compile();

		program.Attach(vshader);
		program.Attach(fshader);
		program.Link();
	} catch (std::string exc) {
		std::cerr << exc << std::endl;
		return -1;
	}

	program.Use();
	program.SetupAttribute("position", 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);
	program.SetupAttribute("texCoord", 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 2 * sizeof(float));
	program.SetupAttribute("color", 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 4 * sizeof(float));

	Texture dirt("dirt.png", GL_RGBA);
	dirt.Bind(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(program._id, "ourTexture"), 0);

	while (!glfwWindowShouldClose(window)) {
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
