//Engine
#include "core/Windows.h"
#include "render/Render.h"
#include "render/Texture.h"
#include "render/Camera.h"
#include "render/GL_shader.h"

#include "render/Light.h"

Shader worldshader;
Shader solidcolor;

Window window;
Render render;
Texture DebugTexture;
Camera camera;
Lights light;

int screenwidth = 1920;
int screenheight = 1080;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = 1920.0 / 2.0;
float lastY = 1080.0 / 2.0;

int main()
{
	window.CreateWindow(screenwidth, screenheight, "Engine_Prototype-2");
	worldshader.Load("vertex.vert", "fragment.frag");
	solidcolor.Load("SolidColor.vert", "SolidColor.frag");
	DebugTexture.loadtexture("res/texture/CustomUVChecker_byValle_1K (1).png");

	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(window.glDebugOutput, nullptr);


	//Plane 1
	std::vector<Render::Vertex> vertices =
	{
		{glm::vec3(0.5f, 0.5f, 0.0f),    glm::vec2(1.0, 1.0), glm::vec3(0.0f,  0.0f,  1.0f)},
		{glm::vec3(0.5f, -0.5f, 0.0f),	 glm::vec2(1.0, 0.0), glm::vec3(0.0f,  0.0f,  1.0f)},
		{glm::vec3(-0.5f, -0.5f, 0.0f),  glm::vec2(0.0, 0.0), glm::vec3(0.0f,  0.0f,  1.0f)},
		{glm::vec3(-0.5f,  0.5f, 0.0f),  glm::vec2(0.0, 1.0), glm::vec3(0.0f,  0.0f,  1.0f)}
	};

	std::vector<uint16_t> indices =
	{
		3, 1, 0,
		3, 2, 1
	};

	std::vector<Render::Vertex_Single> vertex =
	{

	{ glm::vec3(-0.5f, -0.5f, -0.5f)},
	{ glm::vec3(0.5f, -0.5f, -0.5f)},
	{ glm::vec3(0.5f, 0.5f, -0.5f)},
	{ glm::vec3(-0.5f, 0.5f, -0.5f)},
	{ glm::vec3(-0.5f, -0.5f, 0.5f)},
	{ glm::vec3(0.5f, -0.5f, 0.5f)},
	{ glm::vec3(0.5f, 0.5f, 0.5f)},
	{ glm::vec3(-0.5f, 0.5f, 0.5f)}

	};

	std::vector<uint16_t> index =
	{
	//front
	0,1,3,
	1,2,3,

	//right
	1,5,6,
	6,2,1,

	//back
	5,4,7,
	7,6,5,

	//left
	4,0,3,
	3,7,4,

	//top
	3,2,6,
	6,7,3,

	//bottom
	4,5,1,
	1,0,4
	};

	//Mesh
	Render::Mesh mesh = render.create_mesh(vertices, indices);
	Render::Mesh mesh1 = render.create_mesh(vertices, indices);

	//lights
	Render::Mesh Lightmesh1 = render.create_mesh(vertex, index);

	glEnable(GL_DEPTH_TEST);

	glm::vec3 color = glm::vec3(0.0f, 1.0f, 1.0f);
	glm::vec3 position = glm::vec3(1.0f, 1.0f, 1.0f);
	float radius = 2.0f;
	light.CreateLights(color, position, radius);

	camera = glm::vec3(0.0f, 0.0f, 3.0f);

	//GLint maxUboSize;
	//glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUboSize);
	//std::cout << "Maximum UBO size: " << maxUboSize << " bytes\n";

	while (!glfwWindowShouldClose(window)) 
	{
		glfwSwapBuffers(window);
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.4f, 0.2f, 0.6f, 1.0f);

		window.processInput(window);
		window.mouse_callback(window, lastX, lastY);

		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		worldshader.Use();
		camera.view = camera.GetCameraMatrix();
		camera.projection = glm::perspective(glm::radians(camera.fov), (float)screenwidth / screenheight, camera.near, camera.far);

		worldshader.SetMat4("Projection", camera.projection);
		worldshader.SetMat4("View", camera.view);

		light.UpdateLights(worldshader);

		worldshader.SetVec3("viewPos", camera.Position);

		//Sun
		glm::vec3 directLightDir = glm::vec3(-0.2f, -1.0f, -0.3f); 
		glm::vec3 directLightColor = glm::vec3(0.1f, 0.4f, 0.3f); 

		//Sun Information
		worldshader.SetVec3("sun.direction", directLightDir);
		worldshader.SetVec3("sun.color", directLightColor);

		//Plane
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		worldshader.SetMat4("Model", model);

		DebugTexture.bind(0);
		glBindVertexArray(mesh1.vao);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_SHORT, nullptr);

		//Rotating Plane
		glm::mat4 Transform = glm::mat4(1.0f);
		Transform = glm::translate(Transform, glm::vec3(0.0f, 1.5f, 0.0f));
		Transform = glm::rotate(Transform, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		worldshader.SetMat4("Model", Transform);

		glBindVertexArray(mesh.vao);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_SHORT, nullptr);

		//Cube
		solidcolor.Use();

		solidcolor.SetMat4("Projection", camera.projection);
		solidcolor.SetMat4("View", camera.view);
		light.UpdateLights(solidcolor);

		//Transforms
		glm::mat4 cubemodel = glm::mat4(1.0f);
		cubemodel = glm::translate(cubemodel, glm::vec3(0.0f, 3.0f, 0.0f));
		cubemodel = glm::scale(cubemodel, glm::vec3(0.3f, 0.3f, 0.3f));
		solidcolor.SetMat4("Model", cubemodel);

		glBindVertexArray(Lightmesh1.vao);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(index.size()), GL_UNSIGNED_SHORT, nullptr);

	}
	light.~Lights();
	DebugTexture.cleanUp();
	return 0;
}

void Window::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
	{
		glfwSetWindowShouldClose(window, true);
	}

	const float speed = 2.5f * deltaTime;
	camera.Position.y = 1.0f;
	//CameraMovement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.Position += camera.Front * speed;
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.Position -= camera.Front * speed;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.Position -= glm::normalize(glm::cross(camera.Front, camera.WorldUp)) * speed;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.Position += glm::normalize(glm::cross(camera.Front, camera.WorldUp)) * speed;
	}
}

void Window::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse) // initially set to true
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	camera.Yaw += xoffset;
	camera.Pitch += yoffset;

	if (camera.Pitch > 89.0f)
		camera.Pitch = 89.0f;
	if (camera.Pitch < -89.0f)
		camera.Pitch = -89.0f;

	camera.CameraUpdate();
}


