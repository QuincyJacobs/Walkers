#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// data send from vertex to fragment shader
const char *vertexShaderSourceColor = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"out vec4 vertexColor;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos, 1.0);\n"
	"	vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
	"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec4 vertexColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vertexColor;\n"
	"}\n\0";

// old fashioned way
const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos, 1.0);\n"
	"}\0";

const char *fragmentShaderSourceYellow = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
	"}\n\0";

// color send from this application to the fragment shader (as uniform (global))
const char *fragmentShaderSourceUniform = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
	"}\n\0";

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Error handling when creating shaders
	int  success;
	char infoLog[512];

	// shaders
	// color from vertex to fragmentshader
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
	{
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSourceColor, NULL);
		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	// regular shaders not connected by in/out
	unsigned int vertexShader2;
	unsigned int fragmentShader2;
	unsigned int shaderProgram2;
	{
		vertexShader2 = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader2, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader2);
		glGetShaderiv(vertexShader2, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader2, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader2, 1, &fragmentShaderSourceYellow, NULL);
		glCompileShader(fragmentShader2);
		glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		shaderProgram2 = glCreateProgram();
		glAttachShader(shaderProgram2, vertexShader2);
		glAttachShader(shaderProgram2, fragmentShader2);
		glLinkProgram(shaderProgram2);
		glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	// color from vertex to fragmentshader
	unsigned int fragmentShader3;
	unsigned int shaderProgram3;
	{
		fragmentShader3 = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader3, 1, &fragmentShaderSourceUniform, NULL);
		glCompileShader(fragmentShader3);
		glGetShaderiv(fragmentShader3, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader3, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		shaderProgram3 = glCreateProgram();
		glAttachShader(shaderProgram3, vertexShader2);
		glAttachShader(shaderProgram3, fragmentShader3);
		glLinkProgram(shaderProgram3);
		glGetProgramiv(shaderProgram3, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram3, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

	// cleanup shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader2);
	glDeleteShader(fragmentShader2);
	glDeleteShader(fragmentShader3);

	// specify vertices as triangle
	float vertices[] = {
		// triangle 1
		-1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		-0.5f, 1.0f, 0.0f,

		// triangle 2
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.5f, 1.0f, 0.0f,

		// triangle 3
		-1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		-0.5f, -1.0f, 0.0f
	};

	/* Unused second triangle
	float vertices2[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.5f, 1.0f, 0.0f
	};
	*/

	// Vertex Buffer Objects; Manages memory of all vertices
	unsigned int VBO;
	// Generate a unique ID for the buffer
	glGenBuffers(1, &VBO);

	// Vertex Array Object; Stored subsequent vertex attribute calls
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// TRIANGLE ONE
	glBindVertexArray(VAO);

	// bind the buffer to the GL_ARRAY_BUFFER target
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// copy vertices data into the buffer memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// tell gl how to interpret the vertex data (per vertex attribute)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/* Unused second triangle

	// TRIANGLE TWO
	glBindVertexArray(VAO[1]);
	// bind the buffer to the GL_ARRAY_BUFFER target
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	// copy vertices data into the buffer memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	// tell gl how to interpret the vertex data (per vertex attribute)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	*/

	//glBindVertexArray(0);
	glBindVertexArray(VAO);

	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// use shaders created earlier
		
		// triangle 1
		glUseProgram(shaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// triangle 2
		glUseProgram(shaderProgram2);
		glDrawArrays(GL_TRIANGLES, 3, 3);
		// triangle 3
		glUseProgram(shaderProgram3);
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram3, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 6, 3);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}