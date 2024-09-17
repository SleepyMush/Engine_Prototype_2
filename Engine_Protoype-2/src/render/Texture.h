#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <STB/stb_image.h>
#include <string>

#include <iostream>

class Texture {
private:
	int width, height, nrChannels;
public:
	GLuint textureID;
	void loadtexture(std::string filepath);
	void cleanUp() const;
	void bind(unsigned int slot);
};