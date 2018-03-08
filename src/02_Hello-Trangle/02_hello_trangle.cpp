#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//settings
const unsigned int scr_width = 800;
const unsigned int scr_height = 600;

// 编写顶点着色器
const char*  vertexShaderSource =
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\n\0";

//编写片段着色器
const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
	"}\n\0";
int main()
{

	glfwInit();														//initialize GLFW
																	// configure 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					//
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			//on Mac OS X need this line
#endif
	//create a window object
	GLFWwindow* window = glfwCreateWindow(scr_width, scr_height, "LearnOpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window " << std::endl;
        glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//register a callback function on the window 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//initialize GLAD load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	//创建顶点着色器对象
	unsigned int vertexShader=glCreateShader(GL_VERTEX_SHADER);

	//着色器源码附加到着色器对象上
	glShaderSource(vertexShader, 1, &vertexShaderSource,NULL);
	glCompileShader(vertexShader);

	//验证是否编译成功 
	int success;
	char infoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//创建片段着色器对象
	unsigned int fragementShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	//将片段着色器对象负载着色器对象上 并编译
	glShaderSource(fragementShader,1,&fragmentShaderSource,NULL);
	glCompileShader(fragementShader);

	//验证是否成功
	glGetShaderiv(fragementShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragementShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//着色器程序
	//创建一个程序对象
	unsigned int shaderProgram = glCreateProgram();

	//将着色器附加到着色器程序上
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragementShader);
	glLinkProgram(shaderProgram);

	//验证是否成功
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//删除着色器对象
	glDeleteShader(vertexShader);
	glDeleteShader(fragementShader);



	// 定义顶点数组  标准化设备坐标形式
	float vertices[] = {
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
	   -0.5f, -0.5f, 0.0f,
       -0.5f,  0.5f, 0.0f
	};

    unsigned int indices[] = {
        0, 1, 3,      //first Triangle
        1, 2, 3       // seconde Triangle
    };

	// 生成 VBO对象	缓冲对象有多种  顶点缓冲对象是 GL_ARRAY_BUFFER
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    
    glGenVertexArrays(1, &VAO);                 //顶点数组对象
	glGenBuffers(1, &VBO);						//生成对象
    glGenBuffers(1, &EBO);
	
    // bind the Vertex Array Object first then set Vertex Buffers, and the configure vertex attributes
	glBindVertexArray(VAO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);			//绑定类型

    // 将顶点数据复制到缓冲
    //***
    //GL_STATIC_DRAW			数据不会或几乎不变
    //GL_DYNAMIC_DRAW			数据会被改变很多
    //GL_STREAM_DRAW			数据每次绘制都会改变
    //
    //			目标缓冲类型		传输数据大小（字节） 数据		 显卡如何管理数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//链接顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

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

		//使用着色器程序
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //glBindVertexArray(0); no need to unbind every time

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);  (0, 0);
}

//处理输入事件
void processInput(GLFWwindow * window)
{
	//ESC 键是否被按下
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}









