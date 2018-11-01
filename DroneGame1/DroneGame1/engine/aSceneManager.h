#pragma once

#include "Scene.h"
#include <vector>

using namespace std;

class aSceneManager {

private:
	vector<Scene> scenes;
	int active_scene;

public:
	aSceneManager();
	aSceneManager(const aSceneManager &copy);
	~aSceneManager();

	virtual void setScene(int scene) = 0;
	virtual void addScene(Scene &scene);
	

};
