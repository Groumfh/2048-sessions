#include "button.h"

Button::Button(Button_Type type)
{
	this->bType = type;
}


void Button::OnClick(void* callback)
{
	if(isEnabled)
		callback();

}

