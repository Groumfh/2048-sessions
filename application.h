#pragma once

#include <memory>

class GLFWwindow;

class Application
{
public:

	Application(int argc, char **argv);

	~Application();

	bool isInitialized() const;

	int run();

private:
	typedef std::unique_ptr<GLFWwindow,void(*)(GLFWwindow*)> ScopedWindowPtr;
	ScopedWindowPtr window_;

	static void resizeWindow(GLFWwindow* window, int width, int height);
	static void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
};

