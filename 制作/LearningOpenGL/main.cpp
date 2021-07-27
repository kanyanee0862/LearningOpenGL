#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"
#include "VertexArray.h"
#include "Camera.h"

bool Initialize(unsigned int screenwidth, unsigned int screenheight);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void createSpriteVerts();
GLuint loadtexture(const char* path);

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;
GLFWwindow* window;
VertexArray* mSpriteVerts;
//Global vec3 to represent the light
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
//-----------------------------------------------------
//Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
//deltatime
float deltaTime = 0.0f; //Time between current frame and last frame
float lastFrame = 0.0f; //Time of last frame

int main(int argc, char** argv)
{

	Initialize(SCR_WIDTH, SCR_HEIGHT);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	Shader shader("./Shader/Material.vert", "./Shader/Material.frag");
	Shader lampShader("./Shader/lamp.vert", "./Shader/lamp.frag");
	
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	// Point lights color
	glm::vec3 pointLightColor[] = {
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0, 0.0),
		glm::vec3(0.0f, 0.0f, 0.0f)
	};
	createSpriteVerts();
	

	GLuint diffusemap = loadtexture("./Texture/container2.png");
	GLuint specularMap = loadtexture("./Texture/container2_specular.png");
	//GLuint emissionMap = loadtexture("./Texture/matrix.jpg");
	shader.Activate();
	shader.setInt("material.diffuse", 0);
	shader.setInt("material.specular", 1);
	//shader.setInt("material.emission", 2);
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		mSpriteVerts->SetActive();
		//Activate before setting uniforms
		shader.Activate();
		shader.setVec3("viewPos", camera.GetPosition());
		
		//set lighting
		shader.setFloat("material.shininess", 32.0f);
		// directional light
		shader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		shader.setVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("dirLight.diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		shader.setVec3("dirLight.specular", glm::vec3(0.5f));
		// point light 1
		shader.setVec3("pointLights[0].position", pointLightPositions[0]);
		shader.setVec3("pointLights[0].ambient", pointLightColor[0]);
		shader.setVec3("pointLights[0].diffuse", pointLightColor[0]);
		shader.setVec3("pointLights[0].specular", pointLightColor[0]);
		shader.setFloat("pointLights[0].constant", 1.0f);
		shader.setFloat("pointLights[0].linear", 0.09);
		shader.setFloat("pointLights[0].quadratic", 0.032);
		// point light 2
		shader.setVec3("pointLights[1].position", pointLightPositions[1]);
		shader.setVec3("pointLights[1].ambient", pointLightColor[1]);
		shader.setVec3("pointLights[1].diffuse", pointLightColor[1]);
		shader.setVec3("pointLights[1].specular", pointLightColor[1]);
		shader.setFloat("pointLights[1].constant", 1.0f);
		shader.setFloat("pointLights[1].linear", 0.09);
		shader.setFloat("pointLights[1].quadratic", 0.032);
		// point light 3
		shader.setVec3("pointLights[2].position", pointLightPositions[2]);
		shader.setVec3("pointLights[2].ambient", pointLightColor[2]);
		shader.setVec3("pointLights[2].diffuse", pointLightColor[2]);
		shader.setVec3("pointLights[2].specular", pointLightColor[2]);
		shader.setFloat("pointLights[2].constant", 1.0f);
		shader.setFloat("pointLights[2].linear", 0.09);
		shader.setFloat("pointLights[2].quadratic", 0.032);
		// point light 4
		shader.setVec3("pointLights[3].position", pointLightPositions[3]);
		shader.setVec3("pointLights[3].ambient", pointLightColor[3]);
		shader.setVec3("pointLights[3].diffuse", pointLightColor[3]);
		shader.setVec3("pointLights[3].specular", pointLightColor[3]);
		shader.setFloat("pointLights[3].constant", 1.0f);
		shader.setFloat("pointLights[3].linear", 0.09);
		shader.setFloat("pointLights[3].quadratic", 0.032);
		// spotLight
		shader.setVec3("spotLight.position", camera.GetPosition());
		shader.setVec3("spotLight.direction", camera.GetFront());
		shader.setVec3("spotLight.ambient", glm::vec3(0.0f));
		shader.setVec3("spotLight.diffuse", glm::vec3(1.0f));
		shader.setVec3("spotLight.specular", glm::vec3(1.0f));
		shader.setFloat("spotLight.constant", 1.0f);
		shader.setFloat("spotLight.linear", 0.09);
		shader.setFloat("spotLight.quadratic", 0.032);
		shader.setFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
		shader.setFloat("spotLight.outercutoff", glm::cos(glm::radians(15.0f)));
		
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), static_cast<float>(SCR_WIDTH / SCR_HEIGHT), 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);

		//bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffusemap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, emissionMap);
		for (unsigned int i = 0; i < 10; ++i)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, sin((float)glfwGetTime())* glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setMat4("model", model);
			//Draw count vertices foun in array
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		
		// also draw the lamp object(s)
		lampShader.Activate();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);

		// we now draw as many light bulbs as we have point lights.
		//glBindVertexArray(lightVAO);
		for (unsigned int i = 0; i < 4; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			lampShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		//draw indexes    
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glEnable(GL_DEPTH_TEST);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	

	glfwTerminate();
	return 0;
}

bool Initialize(unsigned int screenwidth, unsigned int screenheight)
{
	//Initialize GLFW
	glfwInit();
	//Setting GLFW version (3)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Create window
	window = glfwCreateWindow(screenwidth, screenheight, "LearningOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to Initialize GLAD" << std::endl;
		return false;
	}

	//Setting Viewport
	glViewport(0, 0, screenwidth, screenheight);
	return true;
}
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	float cameraSpeed = 2.5f*deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

GLuint loadtexture(const char* path)
{
	//Texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	//load and generate texture
	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		//set the texture wrapping/filtering options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}
void createSpriteVerts()
{
	//Triangle
	//verticle in x,y,z
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	mSpriteVerts = new VertexArray(vertices, 8, 0, 0);
	
}