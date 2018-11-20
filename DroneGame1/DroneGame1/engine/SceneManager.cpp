#include "SceneManager.h"

SceneManager::SceneManager()
{
}

SceneManager::SceneManager(const SceneManager & copy)
{
	//point to the copy, not deep copy
	*this = copy;
}

SceneManager::~SceneManager()
{

	while (scenes.size() > 0) {
		scenes.erase(scenes.begin());
	}

}
void SceneManager::setScene(int sceneToActivate) {
	activeScene = sceneToActivate;
}
int SceneManager::addScene(Scene* scene, bool activate)
{
	this->scenes.push_back(scene);
	if (activate) {
		activeScene = this->scenes.size() - 1;
	}
	return this->scenes.size() - 1;
}

void SceneManager::drawActive() {
	if (activeScene != -1 && (int)scenes.size() > activeScene) {
		scenes[activeScene]->draw();
	}
}