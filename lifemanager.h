#pragma once

#include <GLFW/glfw3.h>
#include "boardview.h"

class LifeManager
{
public:
	LifeManager(GLFWwindow* window, BoardView* boardview);



	void setBoardView(BoardView* boardview);

	void removeSquareAt(double xpos, double ypos);

	void mouseEvent(int button, int action, int mods, double xpos, double ypos);

	void paint(NVGcontext* context, Rect rect);

	int getLives() const;

private:
	GLFWwindow* window_;
	BoardView* boardView_;

	int lives_;

};