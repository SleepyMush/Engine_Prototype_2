#include "core/Windows.h"
#include "render/Shader.h"
#include "render/Render.h"

#include "render/Texture.h"
#include "render/Camera.h"

Window window;
Render render;
Texture DebugTexture;
Camera camera;

int screenwidth = 1920;
int screenheight = 1080;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool firstMouse = true;
float lastX = 1920.0 / 2.0;
float lastY = 1080.0 / 2.0;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	window.CreateWindow(screenwidth, screenheight, "Engine_Prototype-2");

	Shader shader("res/shader/vertex.vert", "res/shader/fragment.frag");
	Shader light("res/shader/light.vert", "res/shader/light.frag");

	DebugTexture.loadtexture("res/texture/CustomUVChecker_byValle_1K (1).png");

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

	unsigned int materialdiffuse = glGetUniformLocation(shader.ID, "material.diffuse");
	glUniform1f(materialdiffuse, 0);

	unsigned int materialspecular = glGetUniformLocation(shader.ID, "material.specular");
	glUniform1f(materialspecular, 1);

	camera = glm::vec3(0.0f, 0.0f, 3.0f);
	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

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

		shader.use();

		camera.view = camera.GetCameraMatrix();
		camera.projection = glm::perspective(glm::radians(camera.fov), (float)screenwidth / screenheight, camera.near, camera.far);

		unsigned int ProjectionLoc = glGetUniformLocation(shader.ID, "Projection");
		glUniformMatrix4fv(ProjectionLoc, 1, GL_FALSE, glm::value_ptr(camera.projection));
		unsigned int ViewLoc = glGetUniformLocation(shader.ID, "View");
		glUniformMatrix4fv(ViewLoc, 1, GL_FALSE, glm::value_ptr(camera.view));

		//Lighting Information
		unsigned int pos = glGetUniformLocation(shader.ID, "light.position");
		glUniform3f(pos, lightPos.x, lightPos.y, lightPos.z);

		unsigned int viewpos = glGetUniformLocation(shader.ID, "viewPos");
		glUniform3f(viewpos, camera.Position.x, camera.Position.y, camera.Position.z);

		unsigned int ambient = glGetUniformLocation(shader.ID, "light.ambient");
		glUniform3f(ambient, 0.2f, 0.2f, 0.2f);
		unsigned int diffuse = glGetUniformLocation(shader.ID, "light.diffuse");
		glUniform3f(diffuse, 0.5f, 0.5f, 0.5f);
		unsigned int specular = glGetUniformLocation(shader.ID, "light.specular");
		glUniform3f(specular, 0.5f, 0.5f, 0.5f);

		unsigned int materialshininess = glGetUniformLocation(shader.ID, "material.shininess");
		glUniform1f(materialshininess, 32.0f);

		//Plane
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 0.0f));
		unsigned int MatLoc = glGetUniformLocation(shader.ID, "Model");
		glUniformMatrix4fv(MatLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(mesh1.vao);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_SHORT, nullptr);

		//Rotating Plane
		glm::mat4 Transform = glm::mat4(1.0f);
		Transform = glm::translate(Transform, glm::vec3(0.0f, 1.5f, 0.0f));
		Transform = glm::rotate(Transform, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		unsigned int MatrixLocation = glGetUniformLocation(shader.ID, "Model");
		glUniformMatrix4fv(MatrixLocation, 1, GL_FALSE, glm::value_ptr(Transform));

		DebugTexture.bind(0);
		glBindVertexArray(mesh.vao);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_SHORT, nullptr);

		//Cube
		light.use();

		unsigned int ProjectionLoc1 = glGetUniformLocation(light.ID, "Projection");
		glUniformMatrix4fv(ProjectionLoc1, 1, GL_FALSE, glm::value_ptr(camera.projection));
		unsigned int ViewLoc1 = glGetUniformLocation(light.ID, "View");
		glUniformMatrix4fv(ViewLoc1, 1, GL_FALSE, glm::value_ptr(camera.view));

		glm::mat4 cubemodel = glm::mat4(1.0f);
		cubemodel = glm::translate(cubemodel, glm::vec3(0.0f, 3.0f, 0.0f));
		cubemodel = glm::scale(cubemodel, glm::vec3(0.3f, 0.3f, 0.3f));
		unsigned int ModelLocation = glGetUniformLocation(light.ID, "Model");
		glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, glm::value_ptr(cubemodel));

		glBindVertexArray(Lightmesh1.vao);
		glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(index.size()), GL_UNSIGNED_SHORT, nullptr);

	}
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


//std::vector<Render::Vertex_Single> vertex =
//{

//{ glm::vec3(-0.5f, -0.5f, -0.5f)},
//{ glm::vec3(0.5f, -0.5f, -0.5f)},
//{ glm::vec3(0.5f, 0.5f, -0.5f)},
//{ glm::vec3(-0.5f, 0.5f, -0.5f)},
//{ glm::vec3(-0.5f, -0.5f, 0.5f)},
//{ glm::vec3(0.5f, -0.5f, 0.5f)},
//{ glm::vec3(0.5f, 0.5f, 0.5f)},
//{ glm::vec3(-0.5f, 0.5f, 0.5f)}

//};

//std::vector<uint16_t> index =
//{
//	//front
//	0,1,3,
//	1,2,3,

//	//right
//	1,5,6,
//	6,2,1,

//	//back
//	5,4,7,
//	7,6,5,

//	//left
//	4,0,3,
//	3,7,4,

//	//top
//	3,2,6,
//	6,7,3,

//	//bottom
//	4,5,1,
//	1,0,4
//};

//lights
//Render::Mesh Lightmesh1 = render.create_mesh(vertex, index);
//Render::Mesh Lightmesh2 = render.create_mesh(vertex, index);
//Render::Mesh Lightmesh3 = render.create_mesh(vertex, index);

		//unsigned int ProjectionLoc1 = glGetUniformLocation(light.ID, "Projection");
		//glUniformMatrix4fv(ProjectionLoc1, 1, GL_FALSE, glm::value_ptr(camera.projection));
		//unsigned int ViewLoc1 = glGetUniformLocation(light.ID, "View");
		//glUniformMatrix4fv(ViewLoc1, 1, GL_FALSE, glm::value_ptr(camera.view));

		//glm::mat4 cubemodel = glm::mat4(1.0f);
		//cubemodel = glm::translate(cubemodel, glm::vec3(0.0f, 3.0f, 0.0f));
		//cubemodel = glm::scale(cubemodel, glm::vec3(0.3f, 0.3f, 0.3f));
		//unsigned int ModelLocation = glGetUniformLocation(light.ID, "Model");
		//glUniformMatrix4fv(ModelLocation, 1, GL_FALSE, glm::value_ptr(cubemodel));

		//glBindVertexArray(Lightmesh1.vao);
		//glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(index.size()), GL_UNSIGNED_SHORT, nullptr);


		//glm::mat4 cubemodel2 = glm::mat4(1.0f);
		//cubemodel2 = glm::translate(cubemodel2, glm::vec3(3.0f, 3.0f, 0.0f));
		//cubemodel2 = glm::scale(cubemodel2, glm::vec3(0.3f, 0.3f, 0.3f));
		//unsigned int ModelLocation1 = glGetUniformLocation(light.ID, "Model");
		//glUniformMatrix4fv(ModelLocation1, 1, GL_FALSE, glm::value_ptr(cubemodel2));

		//glBindVertexArray(Lightmesh2.vao);
		//glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(index.size()), GL_UNSIGNED_SHORT, nullptr);

		//glm::mat4 cubemodel3 = glm::mat4(1.0f);
		//cubemodel3 = glm::translate(cubemodel3, glm::vec3(-3.0f, 3.0f, 0.0f));
		//cubemodel3 = glm::scale(cubemodel3, glm::vec3(0.3f, 0.3f, 0.3f));
		//unsigned int ModelLocation2 = glGetUniformLocation(light.ID, "Model");
		//glUniformMatrix4fv(ModelLocation2, 1, GL_FALSE, glm::value_ptr(cubemodel3));

		//glBindVertexArray(Lightmesh3.vao);
		//glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(index.size()), GL_UNSIGNED_SHORT, nullptr);