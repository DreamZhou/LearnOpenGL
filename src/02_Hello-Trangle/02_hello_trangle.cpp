#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//settings
const unsigned int scr_width = 800;
const unsigned int scr_height = 600;

// ��д������ɫ��
const char*  vertexShaderSource =
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\n\0";

//��дƬ����ɫ��
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


	//����������ɫ������
	unsigned int vertexShader=glCreateShader(GL_VERTEX_SHADER);

	//��ɫ��Դ�븽�ӵ���ɫ��������
	glShaderSource(vertexShader, 1, &vertexShaderSource,NULL);
	glCompileShader(vertexShader);

	//��֤�Ƿ����ɹ� 
	int success;
	char infoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//����Ƭ����ɫ������
	unsigned int fragementShader = glCreateShader(GL_FRAGMENT_SHADER);
	
	//��Ƭ����ɫ����������ɫ�������� ������
	glShaderSource(fragementShader,1,&fragmentShaderSource,NULL);
	glCompileShader(fragementShader);

	//��֤�Ƿ�ɹ�
	glGetShaderiv(fragementShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragementShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//��ɫ������
	//����һ���������
	unsigned int shaderProgram = glCreateProgram();

	//����ɫ�����ӵ���ɫ��������
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragementShader);
	glLinkProgram(shaderProgram);

	//��֤�Ƿ�ɹ�
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//ɾ����ɫ������
	glDeleteShader(vertexShader);
	glDeleteShader(fragementShader);



	// ���嶥������  ��׼���豸������ʽ
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

	// ���� VBO����	��������ж���  ���㻺������� GL_ARRAY_BUFFER
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;
    
    glGenVertexArrays(1, &VAO);                 //�����������
	glGenBuffers(1, &VBO);						//���ɶ���
    glGenBuffers(1, &EBO);
	
    // bind the Vertex Array Object first then set Vertex Buffers, and the configure vertex attributes
	glBindVertexArray(VAO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);			//������

    // ���������ݸ��Ƶ�����
    //***
    //GL_STATIC_DRAW			���ݲ���򼸺�����
    //GL_DYNAMIC_DRAW			���ݻᱻ�ı�ܶ�
    //GL_STREAM_DRAW			����ÿ�λ��ƶ���ı�
    //
    //			Ŀ�껺������		�������ݴ�С���ֽڣ� ����		 �Կ���ι�������
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//���Ӷ�������
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
		*	glClear �����������Ļ����ɫ���壬������һ������λ��Buffer Bit)
		*		GL_COLOR_BUFFER_BIT
		*		GL_DEPTH_BUFFER_BIT
		*		GL_STENCIL_BUFFER_BIT
		*	glClearColor �����������Ļ���õ���ɫ
		*/
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//ʹ����ɫ������
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

//���������¼�
void processInput(GLFWwindow * window)
{
	//ESC ���Ƿ񱻰���
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}









