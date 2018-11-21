#pragma once

class InputManager {
private:
	bool leftMouseButton;
	
	int inputs[256] = { 0 };
	int inputs_length = 256;
	
	int current_cooldown[256] = { 0 };

	int input_full_value;

	int cursor_x;
	int cursor_y;

public:

	int cooldown_config[256] = { 0 };

	InputManager();
	InputManager(const InputManager &copy);
	~InputManager();

	void setLeftMouseButton(bool clicked);
	void setInputActivated(int index, int value);
	void setInputActivated(int index, bool activated);
	bool isInputActivated(int index);
	void setCursorPosition(int x, int y);

};