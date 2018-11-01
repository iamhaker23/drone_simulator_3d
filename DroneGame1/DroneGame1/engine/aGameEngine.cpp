#include "aGameEngine.h"

aSceneManager* aGameEngine::getSceneManager() {
	return this->scene_manager;
}

aInputManager* aGameEngine::getInputManager() {
	return this->input_manager;
}

aGameEngine::aGameEngine()
{
	this->input_manager = new aInputManager();

	this->screenWidth = 800;
	this->screenHeight = 640;

}

aGameEngine::aGameEngine(const aGameEngine & copy)
{
}

aGameEngine::~aGameEngine()
{
	delete this->input_manager;

}

int aGameEngine::getScreenWidth() {
	return this->screenWidth;
}
int aGameEngine::getScreenHeight() {
	return this->screenHeight;
}
void aGameEngine::setScreenWidth(int width) {
	this->screenWidth = width;
}
void aGameEngine::setScreenHeight(int height) {
	this->screenHeight = height;
}
