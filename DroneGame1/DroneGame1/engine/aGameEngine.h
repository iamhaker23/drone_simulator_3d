#pragma once

#include "aSceneManager.h"
#include "aInputManager.h"

class aGameEngine {
	
protected:
	int screenWidth;
	int screenHeight;
	aSceneManager* scene_manager;
	aInputManager* input_manager;

public:

	aGameEngine();
	aGameEngine(const aGameEngine &copy);
	~aGameEngine();

	virtual void processKeys() = 0;
	virtual void draw() = 0;
	virtual void init() = 0;
	virtual void update() = 0;
	virtual void resize(int width, int height) = 0;
	aSceneManager* getSceneManager();
	aInputManager* getInputManager();

	int getScreenWidth();
	int getScreenHeight();
	void setScreenWidth(int width);
	void setScreenHeight(int height);
};