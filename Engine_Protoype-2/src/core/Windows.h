#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window {
private:
	GLFWwindow* window;
public:
	Window() : window(nullptr) {};
	operator GLFWwindow* () const {
		return window;
	}
	int CreateWindow(int screenWidth, int screenHeight, const char* Windowname);
	~Window();
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void processInput(GLFWwindow* window);
	static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
};
