#pragma once

#include <iostream>
#include "GameObject.h"
#include "Cameras.h"

class Scene {
private:
	int activeCamera = -1;
	std::string scene_name;
	vector<GameObject*> objects;

public:

	Scene();
	Scene(const Scene &copy);
	~Scene();

	std::string getName();
	void resizeCameras(GLfloat width, GLfloat height);
	void draw();
	int addCamera(Camera cam, bool activate);
	int addObject(GameObject* toAdd);
	void setActiveCamera(int idx);
	vector<GameObject*> getGameObjects();
	void nextCamera();
	
	void setCameraTargetTrack(bool tracking);
	void setCameraPosition(float x, float y, float z);
	void setCameraRotation(float x, float y, float z);
};