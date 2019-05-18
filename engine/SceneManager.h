#pragma once

#include "Scene.h"
#include <vector>

using namespace std;

class SceneManager {

private:
	vector<Scene*> scenes;
	int activeScene = -1;

public:
	SceneManager();
	SceneManager(const SceneManager &copy);
	~SceneManager();

	virtual void setScene(int scene);
	virtual int addScene(Scene* scene, bool activate);
	virtual void drawActive();
	//virtual void updateActive();
	virtual void resizeCameras(GLfloat width, GLfloat height);
	Scene* getActiveScene();

};
