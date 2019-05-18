#include "GameEngine.h"

SceneManager* GameEngine::getSceneManager() {
	return this->scene_manager;
}

InputManager* GameEngine::getInputManager() {
	return this->input_manager;
}

GameEngine::GameEngine()
{
	this->input_manager = new InputManager();
	this->scene_manager = new SceneManager();

	this->screenWidth = 800;
	this->screenHeight = 640;

	clearColorRGBA[0] = 0.2f;
	clearColorRGBA[1] = 0.2f;
	clearColorRGBA[2] = 0.2f;
	clearColorRGBA[3] = 0.0f;

}

GameEngine::GameEngine(const GameEngine & copy)
{
	//point to the copy, not deep copy
	*this = copy;
}

GameEngine::~GameEngine()
{
	delete this->input_manager;

}

int GameEngine::getScreenWidth() {
	return this->screenWidth;
}
int GameEngine::getScreenHeight() {
	return this->screenHeight;
}
void GameEngine::setScreenWidth(int width) {
	this->screenWidth = width;
}
void GameEngine::setScreenHeight(int height) {
	this->screenHeight = height;
}
