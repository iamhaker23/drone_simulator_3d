#include "GameEngine3D.h"

GameEngine3D::GameEngine3D() : GameEngine()
{
	//std::cout << "It's GameEngine3D" << std::endl;

}

GameEngine3D::GameEngine3D(const GameEngine3D & copy) : GameEngine(copy)
{
}

GameEngine3D::~GameEngine3D()
{
}

void GameEngine3D::cameraUpdateFovManual(Camera* current_camera, float fovDelta) {
	cameraUpdateFov(current_camera, 0.f, 0.f, 0.f, false, fovDelta);
}
void GameEngine3D::cameraUpdateFovAuto(Camera* current_camera, float xSpeed, float ySpeed, float zSpeed) {
	cameraUpdateFov(current_camera, xSpeed, ySpeed, zSpeed, true, 0.f);
}

void GameEngine3D::cameraUpdateFov(Camera* current_camera, float xSpeed, float ySpeed, float zSpeed, bool automaticMode, float fovDelta) {

	float speedSquared = (xSpeed*xSpeed) + (ySpeed*ySpeed) + (zSpeed*zSpeed);

	float fovSpeedFactor = current_camera->speedToFov*speedSquared;

	float fovSpeedDelta = ((fovSpeedFactor < 0.01f) ? -0.1f : 0.1f);

	if (current_camera->fov >= current_camera->originalFov + 30.0f) {
		if (fovSpeedDelta > 0.0f) fovSpeedDelta = 0.0f;
		if (fovDelta > 0.0f) fovDelta = 0.0f;
	}
	else if (current_camera->fov <= current_camera->originalFov) {
		if (fovSpeedDelta < 0.0f) fovSpeedDelta = 0.0f;
		if (fovDelta < 0.0f) fovDelta = 0.0f;
	}
	
	current_camera->fov += (automaticMode) ? fovSpeedDelta : fovDelta;
	
	current_camera->updateProjectionMatrix();
}

void GameEngine3D::draw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	scene_manager->drawActive();

	glFlush();

}

void GameEngine3D::init()
{

	glClearColor(clearColorRGBA[0], clearColorRGBA[1], clearColorRGBA[2], clearColorRGBA[3]);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

}

void GameEngine3D::update(bool doPhysics, float time)
{

	//TODO: WHY DOES THIS NOT WORK!?
	//scene_manager->updateActive();
	GameObject::time = time;
	GameObject::collisionsResolved.clear();

	current_scene = scene_manager->getActiveScene();
	
	//vector<GameObject*> current_scene_objects = current_scene->getGameObjects();
	//for (int i = 0; i < (int)current_scene_objects.size(); i++) {

	for (int x = 0; x < current_scene->grid->midpoint * 2; x++) {
		for (int y = 0; y < current_scene->grid->midpoint * 2; y++) {
			for (int z = 0; z < current_scene->grid->midpoint * 2; z++) {

				vector<GameObject*> sector = current_scene->grid->grid[x][y][z];

				//if ((int)sector.size() > 0) cout << "COLLISIONS IN SECTOR:" << x << "," << y << "," << z << endl;

				for (int o = 0; o < (int)sector.size(); o++) {
					sector[o]->doCollisionsAndApplyForces(doPhysics, sector);
				}
			}
		}
	}
	//Grid* oldGrid = current_scene->grid;
	current_scene->grid = new Grid(current_scene->grid->objects);
	//delete oldGrid;

}

void GameEngine3D::resize(int width, int height)
{
	this->screenWidth = width; 
	this->screenHeight = height;

	glViewport(0, 0, width, height);
	scene_manager->resizeCameras((GLfloat)width, (GLfloat)height);
	
}
