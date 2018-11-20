#include "Scene.h"

Scene::Scene()
{
	objects = vector<GameObject*>();
	Cameras::init();
}

Scene::Scene(const Scene & copy)
{
	//point to the copy, not deep copy
	*this = copy;
}

int Scene::addCamera(Camera cam, bool activate) {
	Cameras::addCamera(cam);
	if (activate) {
		activeCamera = Cameras::cameras.size() - 1;
	}
	return Cameras::cameras.size() - 1;
}

void Scene::setActiveCamera(int idx) {
	activeCamera = idx;
}

Scene::~Scene()
{

	while (objects.size() > 0) {
		objects.erase(objects.begin());
	}

}

std::string Scene::getName()
{
	return this->scene_name;
}

void Scene::resizeCameras(GLfloat width, GLfloat height) {
	for (int i = 0; i < (int)Cameras::cameras.size(); i++) {
		Cameras::cameras[i].resize(width, height);
	}
}

void Scene::draw() {
	if (activeCamera == -1 || (int)Cameras::cameras.size() <= activeCamera){
		return;
	}
	
	for (int i = 0; i < (int)objects.size(); i++) {
		objects[i]->draw(Cameras::cameras[activeCamera].projectionMatrix);
	}
}

int Scene::addObject(GameObject* toAdd) {
	objects.push_back(toAdd);
	return objects.size() - 1;
}

vector<GameObject*> Scene::getGameObjects() {
	return this->objects;
}