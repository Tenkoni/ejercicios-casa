/*---------------------------------------------------------*/
/* ----------------   Prática 2 --------------------------*/
/*-----------------    2019-2   ---------------------------*/
/*------------- Computaci Gráfica e interacción humano computadora---------------*/
/*------------- Miramontes Sarabia Luis Enrique  ---------------------------*/
/*------------- Visual Studio 2017  ---------------------------*/
/*------------- Práctica 2 trabajo en casa  ---------------------------*/

#include <glew.h>
#include <glfw3.h>

#include <iostream>
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;
GLuint shaderProgramRed, shaderProgramColor;

static const char* myVertexShader = "										\n\
#version 330 core															\n\
																			\n\
layout (location = 0) in vec4 aPos;											\n\
																			\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);							\n\
}";

static const char* myVertexShaderColor = "									\n\
#version 330 core															\n\
																			\n\
layout (location = 0) in vec3 aPos;											\n\
layout (location = 1) in vec3 aColor;										\n\
out vec3 ourColor;															\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(aPos, 1.0);											\n\
	ourColor = aColor;														\n\
}";

// Fragment Shader
static const char* myFragmentShaderRed = "									\n\
#version 330																\n\
																			\n\
out vec3 finalColor;														\n\
																			\n\
void main()																	\n\
{																			\n\
    finalColor = vec3(1.0f, 1.0f, 0.0f);									\n\
}";

static const char* myFragmentShaderColor = "								\n\
#version 330 core															\n\
out vec4 FragColor;															\n\
in vec3 ourColor;															\n\
																			\n\
void main()																	\n\
{																			\n\
	FragColor = vec4(ourColor, 1.0f);										\n\
}";

void myData(void);
void setupShaders(void);
void display(void);
void getResolution(void);


void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{
	

	float vertices[] = 
	{
		// positions         //
		//L 
		-0.9f, 0.7f, 0.0f, 0.4f, 0.2f, 0.5f,     //primeros tres: posici, restantes tres: color  cambiamos la forma por el shader 0
		-0.7f, 0.7f, 0.0f,  0.12f, 0.322f, 0.23f,     //X,Y,Z,R,G,B  1
		-0.7f, -0.6f, 0.0f,  0.2f, 0.314f, 0.421f, //2
		-0.2f, -0.6f, 0.0f,  0.4f, 0.854f, 0.1510f, //3
		-0.2f, -0.8f, 0.0f, 0.34f, 0.430f, 0.32f, //4
		-0.9f, -0.8f, 0.0f,  0.423f, 0.86f, 0.53f, //5
		//M
		 0.1f, 0.7f, 0.0f, 0.23f, 0.41f, 0.63f,     //primeros tres: posici, restantes tres: color  cambiamos la forma por el shader 6
		 0.3f, 0.7f, 0.0f,  0.4f, 0.3f, 0.1f,     //X,Y,Z,R,G,B  7
		 0.3f, 0.3f, 0.0f,  0.5f, 0.3f, 0.7f, //8
		 0.5f, 0.1f, 0.0f,  0.8f, 0.2f, 0.1f, //9
		 0.7f, 0.3f, 0.0f, 0.5f, 0.3f, 0.2f, //10
		 0.7f, 0.7f, 0.0f,  0.64f, 0.123f, 0.42f, //11
		 0.9,  0.7f, 0.0f,  0.23f, 0.43f, 0.0f, //12
		 0.9f, -0.8f, 0.0f,	0.604f, 0.23f, 0.8f, //13
	     0.7f, -0.8f, 0.0f,  0.654f, 0.34f, 0.7f, //14
		 0.5f, -0.6f, 0.0f,  0.654f, 0.34f, 0.7f, //15
		 0.3f, -0.8f, 0.0f, 0.432f, 0.3f, 0.4f, //16
		 0.1f, -0.8f, 0.0f,  0.654f, 0.38f, 0.43f, //17
	};

	unsigned int indices[] =
	{
		//L
		0, 1, 5, //0
		5, 1, 2,//3
		5, 2, 4,//6
		2, 3, 4,//9
		//M
		6, 7, 17,//12
		16, 17, 7,//15
		7, 8, 9,//18
		15, 8, 15,//21
		9, 10, 11,//24
		9, 10, 11,//27
		12, 13, 14,//30
		13, 11    //33
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void setupShaders()
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &myVertexShader, NULL);
	glCompileShader(vertexShader);

	unsigned int vertexShaderColor = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderColor, 1, &myVertexShaderColor, NULL);
	glCompileShader(vertexShaderColor);

	unsigned int fragmentShaderRed = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderRed, 1, &myFragmentShaderRed, NULL);
	glCompileShader(fragmentShaderRed);

	unsigned int fragmentShaderColor = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderColor, 1, &myFragmentShaderColor, NULL);
	glCompileShader(fragmentShaderColor);


	//Crear el Programa que combina Geometr僘 con Color
	shaderProgramRed = glCreateProgram();
	glAttachShader(shaderProgramRed, vertexShader);
	glAttachShader(shaderProgramRed, fragmentShaderRed);
	glLinkProgram(shaderProgramRed);

	shaderProgramColor = glCreateProgram();
	glAttachShader(shaderProgramColor, vertexShaderColor);
	glAttachShader(shaderProgramColor, fragmentShaderColor);
	glLinkProgram(shaderProgramColor);
	//Check for errors 

	//ya con el Programa, el Shader no es necesario
	glDeleteShader(vertexShader);
	glDeleteShader(vertexShaderColor);
	glDeleteShader(fragmentShaderRed);
	glDeleteShader(fragmentShaderColor);

}

void display(void)
{
	glUseProgram(shaderProgramColor);

	glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glPointSize(5.0);
	glDrawElements(GL_TRIANGLE_FAN, 3, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLE_FAN, 3, GL_UNSIGNED_INT, BUFFER_OFFSET(3 * sizeof(GLuint)));
	glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, BUFFER_OFFSET(6*sizeof(GLuint)));
	glDrawElements(GL_TRIANGLE_FAN, 3, GL_UNSIGNED_INT, BUFFER_OFFSET(12 * sizeof(GLuint)));
	glDrawElements(GL_TRIANGLE_FAN, 3, GL_UNSIGNED_INT, BUFFER_OFFSET(15 * sizeof(GLuint)));
	glDrawElements(GL_TRIANGLE_FAN, 5, GL_UNSIGNED_INT, BUFFER_OFFSET(18 * sizeof(GLuint)));
	glDrawElements(GL_TRIANGLE_FAN, 5, GL_UNSIGNED_INT, BUFFER_OFFSET(23 * sizeof(GLuint)));
	glDrawElements(GL_TRIANGLE_FAN, 3, GL_UNSIGNED_INT, BUFFER_OFFSET(29 * sizeof(GLuint)));
	glDrawElements(GL_TRIANGLE_FAN, 3, GL_UNSIGNED_INT, BUFFER_OFFSET(32 * sizeof(GLuint)));

	glBindVertexArray(0);

	glUseProgram(0);

}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 2", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	myData();
	//Configurar Shaders
	setupShaders();
    

    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Backgound color
        glClearColor((float)74/255, (float)164/255, (float)185/255, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		//Mi funci de dibujo
		display();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}