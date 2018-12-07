#include "Scene.h"

Scene::Scene()
{
	objects = vector<GameObject*>();
	lights = vector<GameObject*>();
	Cameras::init();
}

Scene::Scene(const Scene & copy)
{
	//point to the copy, not deep copy
	*this = copy;
}

int Scene::addCamera(Camera* cam, bool activate) {
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
	while (lights.size() > 0) {
		lights.erase(lights.begin());
	}

}

std::string Scene::getName()
{
	return this->scene_name;
}

void Scene::resizeCameras(GLfloat width, GLfloat height) {
	for (int i = 0; i < (int)Cameras::cameras.size(); i++) {
		Cameras::cameras[i]->resize(width, height);
	}
}

void Scene::draw() {
	if (activeCamera == -1 || (int)Cameras::cameras.size() <= activeCamera){
		return;
	}
	glm::mat4 cameraModelView = glm::inverse(Cameras::cameras[activeCamera]->modelViewMatrix);

	for (int i = 0; i < (int)lights.size(); i++) {
		lights[i]->updateTransformation();
	}

	for (int i = 0; i < (int)objects.size(); i++) {
		assignLights(objects[i]);
		
		objects[i]->draw(Cameras::cameras[activeCamera]->projectionMatrix, cameraModelView);
		
	}

	Cameras::cameras[activeCamera]->updateCameraTransformation();

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

void Scene::setCameraTrackingEnabled(bool tracking) {
	Cameras::cameras[activeCamera]->trackTarget = tracking;
}

Camera* Scene::getCurrentCamera() {
	return Cameras::cameras[activeCamera];
}

void Scene::assignLights(GameObject* object) {
	//TODO: get nearest lights? lights of most influence?
	object->numLights = 0;
	int skipped = 0;
	for (int i = 0; i < 4; i++) {
		if (i < (int)lights.size() && (object->name != lights[i]->name || lights[i]->myLight->selfLight)) {
			object->numLights++;
			object->activeLights[i-skipped] = lights[i];
		}
		else skipped++;
	}
}

