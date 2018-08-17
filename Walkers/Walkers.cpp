#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Vector2.h"
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// shader "programs"
// Vertex shader (transform input point data)
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Fragment shader (calculating color output as orange-y)
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

// Fragment shader (calculating color output as yellow)
const char *fragmentShaderSourceYellow = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
"}\n\0";

int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Walkers", NULL, NULL);
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

	// Vector2 demo
	Vector2 vec1 = Vector2(4, 5);
	Vector2 vec2 = Vector2(9, 3);

	Vector2 vec3 = vec1 + vec2;
	vec1.print();
	vec2.print();
	vec3.print();
	vec3 = vec3 - vec1 - vec2;
	vec3.print();

	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// fragment shader yellow
	int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderYellow, 1, &fragmentShaderSourceYellow, NULL);
	glCompileShader(fragmentShaderYellow);
	// check for shader compile errors
	glGetShaderiv(fragmentShaderYellow, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderYellow, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// link shaders yellow
	int shaderProgramYellow = glCreateProgram();
	glAttachShader(shaderProgramYellow, vertexShader);
	glAttachShader(shaderProgramYellow, fragmentShaderYellow);
	glLinkProgram(shaderProgramYellow);
	// check for linking errors
	glGetProgramiv(shaderProgramYellow, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramYellow, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShaderYellow);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	/* Square vertices
	0.5f,  0.5f, 0.0f,  // top right
	0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f,   // top left

	Square indices
	0, 1, 3,  // first Triangle
	1, 2, 0   // second Triangle
	*/

	// These next lines are just to demonstrate how 2 VAO, VBO and EBO's would work, 
	// normally I would combine these into 1 array for vertices and 1 for indices
	float vertices1[] = {
		-0.5f, -0.5f, 0.0f,  // bottom left
		0.0f, -0.5f, 0.0f, // bottom middle
		-0.25f,  0.5f, 0.0f,   // top left
	};
	unsigned int indices1[] = {  // note that we start from 0!
		0, 1, 2
	};
	float vertices2[] = {
		0.0f, -0.5f, 0.0f, // bottom middle
		0.25f,  0.5f, 0.0f,   // top right
		0.5f, -0.5f, 0.0f // bottom right
	};
	unsigned int indices2[] = {
		0, 1, 2   // right Triangle
	};

	unsigned int VAOs[2], VBOs[2], EBOs[2];
	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	glGenBuffers(2, EBOs);
	// Vertex Array Object registers/binds VBO and EBO (1)
	glBindVertexArray(VAOs[0]);

	// Vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	// Element Buffer Object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Vertex Array Object registers/binds VBO and EBO (2)
	glBindVertexArray(VAOs[1]);

	// Vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	// Element Buffer Object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);


	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) 
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}	

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw our first triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAOs[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // glDrawArrays(GL_TRIANGLES, 0, 6);

		// draw our second triangle
		glUseProgram(shaderProgramYellow);
		glBindVertexArray(VAOs[1]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//glBindVertexArray(0); // no need to unbind it every time 

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