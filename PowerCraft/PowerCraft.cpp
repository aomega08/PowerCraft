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
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,

     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f
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
	program.SetupAttribute("position", 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	program.SetupAttribute("texCoord", 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 3 * sizeof(float));

	Texture dirt("dirt.png", GL_RGBA);
	dirt.Bind(GL_TEXTURE0);
	glUniform1i(program.GetUniformId("ourTexture"), 0);

	glm::mat4 trans;
	GLint uniTrans = program.GetUniformId("trans");

	glm::mat4 view = glm::lookAt(
		glm::vec3(0.0f, 1.85f, 3.0f),
		glm::vec3(0.0f, 1.85f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);
	GLint uniView = program.GetUniformId("view");
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 proj = glm::perspective(glm::radians(70.0f), 640.0f / 480.0f, 0.2f, 100.0f);
	GLint uniProj = program.GetUniformId("proj");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	int frames = 0;
	double startTime = glfwGetTime();
	double lastTime = startTime;
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	float cameraX = 0.0f;
	float cameraY = 0.0f;
	float cameraZ = 3.0f;
	while (!glfwWindowShouldClose(window)) {
		frames++;
		double currentTime = glfwGetTime();

		// Every second print the FPS counter
		if (currentTime - lastTime >= 1.0) {
			std::cout << frames << " FPS" << std::endl;
			frames = 0;
			lastTime += 1.0;
		}

		// Calculate transformation
		float time = currentTime - startTime;
		trans = glm::rotate(glm::mat4(), time * glm::radians(180.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));

		glm::vec3 cubePositions[] = {
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, 0.0f),
			glm::vec3(-1.0f, 1.0f, 0.0f),
		};

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (GLuint i = 0; i < 4; i++) {
			glm::mat4 model = glm::translate(glm::mat4(), cubePositions[i]);
			glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glFinish();
		glfwSwapBuffers(window);
		glfwPollEvents();

		int state = glfwGetKey(window, GLFW_KEY_D);
		if (state == GLFW_PRESS) {
			cameraX += 0.03;
		}

		state = glfwGetKey(window, GLFW_KEY_A);
		if (state == GLFW_PRESS) {
			cameraX -= 0.03;
		}

		state = glfwGetKey(window, GLFW_KEY_S);
		if (state == GLFW_PRESS) {
			cameraZ += 0.03;
		}

		state = glfwGetKey(window, GLFW_KEY_W);
		if (state == GLFW_PRESS) {
			cameraZ -= 0.03;
		}

		state = glfwGetKey(window, GLFW_KEY_SPACE);
		if (state == GLFW_PRESS) {
			cameraY += 0.03;
		}

		state = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
		if (state == GLFW_PRESS) {
			cameraY -= 0.03;
		}

		if (cameraX < -2.0f)
			cameraX = -2.0f;
		if (cameraX > 2.0f)
			cameraX = 2.0f;

		if (cameraZ < -4.0f)
			cameraZ = -4.0f;
		if (cameraZ > 4.0f)
			cameraZ = 4.0f;

		view = glm::lookAt(
			glm::vec3(cameraX, cameraY + 1.85f, cameraZ),
			glm::vec3(cameraX, cameraY + 1.85f, cameraZ - 3.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
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
