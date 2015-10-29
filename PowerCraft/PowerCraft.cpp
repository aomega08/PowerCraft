#include "stdafx.h"
#include "Engine/VertexBufferObject.h"
#include "Engine/FragmentShader.h"
#include "Engine/VertexShader.h"
#include "Engine/ShaderProgram.h"
#include "Engine/VertexArrayObject.h"
#include "Engine/Texture.h"

#include "Player.h"

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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 0.0f,

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
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 1.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
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

	Player p;

	float speed = 3.0f; // 3 units / second
	float mouseSpeed = 0.13f;

	glm::mat4 proj = glm::perspective(glm::radians(70.0f), 640.0f / 480.0f, 0.2f, 100.0f);
	GLint uniProj = program.GetUniformId("proj");
	GLint uniView = program.GetUniformId("view");
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	int frames = 0;
	double startTime = glfwGetTime();
	double lastTime = startTime;
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glm::vec3 position = glm::vec3(0, 1.63, 0);
	double time2 = lastTime;

	while (!glfwWindowShouldClose(window)) {
		frames++;
		double currentTime = glfwGetTime();
		float deltaTime = currentTime - time2;
		time2 = currentTime;

		// Every second print the FPS counter
		if (currentTime - lastTime >= 1.0) {
			std::cout << frames << " FPS" << std::endl;
			frames = 0;
			lastTime += 1.0;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (float x = -1.0f; x < 1.01f; x += 1.0f) {
			for (float y = -1.0f; y < 1.01f; y += 1.0f) {
				for (float z = -1.0f; z < 1.01f; z += 1.0f) {
					glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(x, y, z));
					glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model));

					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
			}
		}

		glFinish();
		glfwSwapBuffers(window);
		glfwPollEvents();

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		glfwSetCursorPos(window, 800 / 2, 600 / 2);

		float d_alpha = mouseSpeed * deltaTime * float(800 / 2 - xpos);
		float d_beta = mouseSpeed * deltaTime * float(600 / 2 - ypos);
		p.UpdateAngles(d_alpha, d_beta);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			p.MoveForward(deltaTime * speed);
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			p.MoveBackward(deltaTime * speed);
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			p.MoveRight(deltaTime * speed);
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			p.MoveLeft(deltaTime * speed);
		}
		
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			p.MoveUp(deltaTime * speed);
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			p.MoveDown(deltaTime * speed);
		}

		// Update the view for the next frame
		glm::mat4 view = p.GetViewMatrix();
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
