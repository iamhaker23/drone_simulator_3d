#include "Cameras.h"

vector<Camera> Cameras::cameras;

void Cameras::init() {
	Cameras::cameras = vector<Camera>();
}

void Cameras::setCameras(vector<Camera> camList) {
	for (int i = 0; i < (int)camList.size(); i++) {
		Cameras::addCamera(camList[i]);
	}
}
void Cameras::addCamera(Camera cam) {
	Cameras::cameras.push_back(cam);
}
void Cameras::clearCameras() {
	Cameras::cameras.clear();
}