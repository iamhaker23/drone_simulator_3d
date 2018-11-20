#include "DroneGameEngine.h"

DroneGameEngine::DroneGameEngine() : GameEngine3D() {

	

}

void DroneGameEngine::init() {

	//override GameEngine3D clear color
	clearColorRGBA[0] = 0.5f;
	clearColorRGBA[1] = 0.5f;
	clearColorRGBA[2] = 0.5f;
	clearColorRGBA[3] = 0.0f;
	GameEngine3D::init();
		
	
	Scene* myScene = new Scene();

	myScene->addCamera(Camera(), true);

	myScene->addObject(new GameObject("Drone1", "Assets/models/drone-1.obj", "Assets/glslfiles/basicTransformations"));

	myScene->addObject(new GameObject("Drone1", "Assets/models/tardis_1.obj", "Assets/glslfiles/basicTransformations"));

	scene_manager->addScene(myScene, true);

}