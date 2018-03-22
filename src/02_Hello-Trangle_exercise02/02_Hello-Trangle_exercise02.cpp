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
"uniform vec4 outColor;\n"
"void main()\n"
"{\n"
"	FragColor = outColor;\n"
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
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    //����������ɫ������
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //��ɫ��Դ�븽�ӵ���ɫ��������
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
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
    glShaderSource(fragementShader, 1, &fragmentShaderSource, NULL);
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
        -0.5f, 0.5f, 0.0f,
        0.f, 0.f, 0.0f,
    };
    float vertices2[] = {
        0.f, 0.f, 0.0f,
        -0.5f,  -0.5f, 0.0f,
        -0.5f,  0.f, 0.0f,
    };

    // ���� VBO����	��������ж���  ���㻺������� GL_ARRAY_BUFFER

    unsigned int VBO;
    unsigned int VAO;
    //unsigned int EBO;

    glGenVertexArrays(1, &VAO);                 //�����������
    glGenBuffers(1, &VBO);						//���ɶ���
                                                // glGenBuffers(1, &EBO);

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

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //���Ӷ�������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    //�ڶ���������
    unsigned int VAO1;
    unsigned int VBO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);

    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    //���Ӷ�������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
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
       
        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "outColor");
        glUniform4f(vertexColorLocation, 0.f, greenValue, 0.f, 1.0f);


        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);   //���ģʽ

                                                     // ��1������ ��Ҫ���Ƶ�ͼԪ����
                                                     // ��2������ �����������ʼ����
                                                     // ��3������ ��������
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //�ڶ���������
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�߿�ģʽ
        glBindVertexArray(VAO1);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //glBindVertexArray(0); no need to unbind every time

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    //glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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






