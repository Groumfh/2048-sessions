#pragma once
#include <base.h>




enum Button_Type
{
	B_play,
	B_exit,
	B_retry
};
class Button
{
private:
	Rect rectangle;
	Button_Type bType;

public:
	Button(Button_Type type);

	Rect GetRectangle() { return rectangle; }
	Button_Type GetButtonType() { return bType; }
};