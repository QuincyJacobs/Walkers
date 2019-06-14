#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include "Vector2.h"
#include "Shader.h"
#include "ObjFileToVertice.h"

#include <iostream>
#include <stdlib.h>
#include <stdint.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void generateTexture(unsigned int* textureID, const GLchar* image);

// settings
const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;
float SCREEN_ASPECT = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
float SCREEN_FOV = 45.0f;

// time globals
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// mouse movement
bool firstMouse = true;
float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// Camera initialization
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
glm::mat4 view;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Walkers", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	Shader myShader("shader.vs", "shader.fs");

	float vertices[] = {
		0.336188, 0.910663, 0.541078,
		1, -0.222228, 1,
		-1, 0.972872, 1,
		-0.513766, 0.24394, -0.141828,
		1, 1.65452, -0.999999,
		0.999999, 1.74023, 1,
		-1, 1, 1,
		-1, 1.70679, -1

	};

	ObjFileToVertice *oftv = new ObjFileToVertice();
	std::vector<float> thingPositions = oftv->ReadVectors("Resource_Files/Models/thing.txt");

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, thingPositions.size() * sizeof(float), &thingPositions.front(), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// calculate time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw triangle
		//glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
		//glBindTexture(GL_TEXTURE_2D, texture1);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture2);

		// model, view and projection
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = glm::perspective(glm::radians(SCREEN_FOV), SCREEN_ASPECT, 0.1f, 100.0f); //(float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		unsigned int modelLoc = glGetUniformLocation(myShader.ID, "model");
		unsigned int viewLoc = glGetUniformLocation(myShader.ID, "view");
		unsigned int projectionLoc = glGetUniformLocation(myShader.ID, "projection");

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		myShader.use();
		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, thingPositions.size());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
	// CONTROLS:
	//
	// 'esc' to quit
	// '1' key shifts between edge view and normal view
	// '2' key controls the screen aspect ratio
	// '3' key controls the field of view
	// 'shift' + '#' key for the opposite action


	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	// Draw normal or just edges
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	// Screen aspect ratio control
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
	{
		if (SCREEN_ASPECT >= 0.1f) {
			SCREEN_ASPECT -= 0.001f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		if (SCREEN_ASPECT <= 5) {
			SCREEN_ASPECT += 0.001f;
		}
	}

	// Field of View control
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
	{
		if (SCREEN_FOV >= 1) {
			SCREEN_FOV -= 0.03f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		if (SCREEN_FOV <= 45) {
			SCREEN_FOV += 0.03f;
		}
	}

	// Camera movement
	float cameraSpeed = 4.0f * deltaTime;

	if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS))
		cameraPos += cameraSpeed * cameraFront;
	if ((glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS))
		cameraPos -= cameraSpeed * cameraFront;
	if ((glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS))
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if ((glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) || (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS))
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (SCREEN_FOV >= 1.0f && SCREEN_FOV <= 45.0f)
		SCREEN_FOV -= yoffset;
	if (SCREEN_FOV <= 1.0f)
		SCREEN_FOV = 1.0f;
	if (SCREEN_FOV >= 45.0f)
		SCREEN_FOV = 45.0f;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void generateTexture(unsigned int* textureID, const GLchar* image)
{
	glGenTextures(1, textureID);
	glBindTexture(GL_TEXTURE_2D, *textureID);

	// 2d texure repeat by S and T axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// texture filtering (used when images are being scaled)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // get interpolated mipmap and interpolated pixels when downscaling
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // get interpolated pixel when upscaling   (note: Do not ask for mipmaps when upscaling, will result in error)

	// load the image
	int width, height, nrChannels;
	unsigned char *data = stbi_load(image, &width, &height, &nrChannels, 0);
	if (data)
	{
		// generate the texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture " << image << std::endl;
	}
	stbi_image_free(data);
}

void vector2Demo() {
	// Vector2 demo
	Vector2 vec1 = Vector2(4, 5);
	Vector2 vec2 = Vector2(9, 3);

	Vector2 vec3 = vec1 + vec2;
	vec1.print();
	vec2.print();
	vec3.print();
	vec3 = vec3 - vec1 - vec2;
	vec3.print();
}