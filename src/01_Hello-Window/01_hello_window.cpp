#include <glad/glad.h>                //be sure to include GLAD before GLFW 
#include <GLFW/glfw3.h>
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() 
{
	glfwInit();														//initialize GLFW
	// configure 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					//
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			//on Mac OS X need this line

	//create a window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window " << std::endl;
		return -1;
	}
	glfwMakeContextCurrent(window);

	//initialize GLAD 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//register a callback function on the window 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//create a render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		/*
		*	glClear 函数来清空屏幕的颜色缓冲，它接受一个缓冲位（Buffer Bit) 
		*		GL_COLOR_BUFFER_BIT
		*		GL_DEPTH_BUFFER_BIT
		*		GL_STENCIL_BUFFER_BIT
		*	glClearColor 来设置清空屏幕所用的颜色
		*/
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
	glViewport(0, 0, width, height);
}

//处理输入事件
void processInput(GLFWwindow * window)
{
	//ESC 键是否被按下
	if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
