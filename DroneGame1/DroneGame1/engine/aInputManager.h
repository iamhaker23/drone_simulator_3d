#pragma once

class aInputManager {
private:
	bool leftMouseButton;
	
	int inputs[256] = { 0 };
	int inputs_length = 256;

	int input_full_value;

	int cursor_x;
	int cursor_y;

public:

	aInputManager();
	aInputManager(const aInputManager &copy);
	~aInputManager();

	void setLeftMouseButton(bool clicked);
	void setInputActivated(int index, int value);
	void setInputActivated(int index, bool activated);
	bool isInputActivated(int index);
	void setCursorPosition(int x, int y);

};