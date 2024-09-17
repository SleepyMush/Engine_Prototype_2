#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>

class Render {
public:
	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 texture;
		glm::vec3 normal;
	};

	struct Vertex_Single
	{
		glm::vec3 position;
	};


	struct Mesh
	{
		GLuint vao;
		GLuint vbo;
		GLuint ebo;
	};
	Mesh create_mesh(std::vector<Vertex> const& vertices, std::vector<uint16_t> const& indices);
	Mesh create_mesh(std::vector<Vertex_Single> const& vertices, std::vector<uint16_t> const& indices);
};