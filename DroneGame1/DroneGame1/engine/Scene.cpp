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
	
	Cameras::cameras[activeCamera].targetPos = glm::vec3(objects[0]->worldX, objects[0]->worldY, objects[0]->worldZ);
	Cameras::cameras[activeCamera].updateTransformation();

	for (int i = 0; i < (int)objects.size(); i++) {
		objects[i]->draw(Cameras::cameras[activeCamera].projectionMatrix, Cameras::cameras[activeCamera].viewMatrix);
	}
}

int Scene::addObject(GameObject* toAdd) {
	objects.push_back(toAdd);
	return objects.size() - 1;
}

vector<GameObject*> Scene::getGameObjects() {
	return this->objects;
}

void Scene::nextCamera() {
	activeCamera = activeCamera + 1;
	if (activeCamera >= (int)Cameras::cameras.size()) activeCamera = 0;
}

void Scene::setCameraTargetTrack(bool tracking) {
	Cameras::cameras[activeCamera].trackTarget = tracking;
}
void Scene::setCameraPosition(float x, float y, float z){
	Cameras::cameras[activeCamera].camPosX = x;
	Cameras::cameras[activeCamera].camPosY = y;
	Cameras::cameras[activeCamera].camPosZ = z;
}
void Scene::setCameraRotation(float x, float y, float z){
	Cameras::cameras[activeCamera].camRotX = x;
	Cameras::cameras[activeCamera].camRotY = y;
	Cameras::cameras[activeCamera].camRotZ = z;
}

