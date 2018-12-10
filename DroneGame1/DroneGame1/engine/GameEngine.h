#pragma once

#include "SceneManager.h"
#include "InputManager.h"

class GameEngine {
	
protected:
	int screenWidth;
	int screenHeight;
	SceneManager* scene_manager;
	InputManager* input_manager;
	float clearColorRGBA[4] = { 0.0f, 0.0f, 0.0f, 0.0f};
	

public:

	GameEngine();
	GameEngine(const GameEngine &copy);
	~GameEngine();

	virtual void processKeys() = 0;
	virtual void draw() = 0;
	virtual void init() = 0;
	virtual void update(bool doPhysics) = 0;
	virtual void resize(int width, int height) = 0;
	SceneManager* getSceneManager();
	InputManager* getInputManager();

	int getScreenWidth();
	int getScreenHeight();
	void setScreenWidth(int width);
	void setScreenHeight(int height);
};