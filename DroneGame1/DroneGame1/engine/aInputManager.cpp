#include "aInputManager.h"

void aInputManager::setInputActivated(int index, int value)
{
	//intended to allow representing keys and joystick inputs with the same structure
	this->inputs[index] = (value <= 0) ? 0 : ((value >= input_full_value) ? input_full_value : value);
}

void aInputManager::setInputActivated(int index, bool activated)
{
	//set the value as on or off
	this->inputs[index] = (activated) ? input_full_value : 0;
}

bool aInputManager::isInputActivated(int index)
{
	return (index >= 0 && index < this->inputs_length) ? this->inputs[index] : false;
}

void aInputManager::setCursorPosition(int x, int y)
{
	this->cursor_x = x;
	this->cursor_y = y;
}

aInputManager::aInputManager()
{
}

aInputManager::aInputManager(const aInputManager & copy)
{
}

aInputManager::~aInputManager()
{
}

void aInputManager::setLeftMouseButton(bool clicked)
{

	this->leftMouseButton = clicked;

}