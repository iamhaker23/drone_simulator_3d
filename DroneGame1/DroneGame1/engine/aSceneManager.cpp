#include "aSceneManager.h"

aSceneManager::aSceneManager()
{
}

aSceneManager::aSceneManager(const aSceneManager & copy)
{
}

aSceneManager::~aSceneManager()
{
}

void aSceneManager::addScene(Scene &scene)
{
	this->scenes.push_back(scene);
}
