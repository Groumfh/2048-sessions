#pragma once
#include <base.h>

#define IS_MOUSE_IN_RECTANGLE( mX, mY, rect ) ( mX <= ( rect.x + rect.width / 2.0f ) && mX >= ( rect.x - rect.width / 2.0f )\
                                                && mY <= ( rect.y + rect.height / 2.0f ) && mY >= ( rect.y - rect.height / 2.0f ) )


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
	bool isEnabled = true;
	Button_Type bType;

public:
	Button(Button_Type type);
	bool IsMouseHover();
	void OnClick(void * callback=NULL);
	bool IsEnabled();
	Rect GetRectangle() { return rectangle; }
	Button_Type GetButtonType() { return bType; }
};