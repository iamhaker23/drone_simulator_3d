#pragma once

#include <iostream>
#include "GameObject.h"
#include "Cameras.h"
#include "Light.h"

class Scene {
private:
	int activeCamera = -1;
	
public:

	Scene();
	Scene(const Scene &copy);
	~Scene();

	vector<GameObject*> lights;
	std::string scene_name;
	vector<GameObject*> objects;

	std::string getName();
	void resizeCameras(GLfloat width, GLfloat height);
	void draw();
	void update();

	int addCamera(Camera* cam, bool activate);
	int addObject(GameObject* toAdd);
	void setActiveCamera(int idx);
	vector<GameObject*> getGameObjects();
	void nextCamera();
	
	void setCameraTrackingEnabled(bool tracking);
	Camera* Scene::getCurrentCamera();

	void assignLights(GameObject* object);
};