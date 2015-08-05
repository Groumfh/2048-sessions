#pragma once

#include <memory>

class GLFWwindow;
class Board;
class BoardView;
class NVGcontext;

class Application
{
public:

	Application(int argc, char **argv);

	~Application();

	bool isInitialized() const;

	bool initGL();

	int run();

private:
	typedef std::unique_ptr<GLFWwindow,void(*)(GLFWwindow*)> ScopedWindowPtr;
	ScopedWindowPtr window_;
	std::unique_ptr<Board> board_;
	std::unique_ptr<BoardView> boardView_;

	static void resizeWindow(GLFWwindow* window, int width, int height);
	void resizeWindow2(GLFWwindow* window, int width, int height);
	static void keyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);

	void paint(NVGcontext* context);
	bool propagateKeyEvent(int key, int scancode, int action, int mods);
};

