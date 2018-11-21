#include "InputManager.h"

void InputManager::setInputActivated(int index, int value)
{
	//intended to allow representing keys and joystick inputs with the same structure
	this->inputs[index] = (value <= 0) ? 0 : ((value >= input_full_value) ? input_full_value : value);
}

void InputManager::setInputActivated(int index, bool activated)
{
	//set the value as on or off
	this->inputs[index] = (activated) ? input_full_value : 0;
}

bool InputManager::isInputActivated(int index)
{
	bool positive = this->inputs[index];
	
	//cooldown enabled
	bool enabled = (index >= 0 && index < this->inputs_length) ? (current_cooldown[index]) <= 0 : false;
	bool pressed = (index >= 0 && index < this->inputs_length) ? this->inputs[index] : false;
	if (enabled) {
		if (pressed) {
			current_cooldown[index] = cooldown_config[index];
			return true;
		}
	}
	current_cooldown[index] = current_cooldown[index] - 1;
	return false;

}

void InputManager::setCursorPosition(int x, int y)
{
	this->cursor_x = x;
	this->cursor_y = y;
}

InputManager::InputManager()
{
}

InputManager::InputManager(const InputManager & copy)
{
	//point to the copy, not deep copy
	*this = copy;
}

InputManager::~InputManager()
{
}

void InputManager::setLeftMouseButton(bool clicked)
{

	this->leftMouseButton = clicked;

}