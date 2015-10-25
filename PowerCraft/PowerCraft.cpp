#include "stdafx.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
}

int main() {
	int width, height;
	glfwInit();

	GLFWwindow *window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

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
