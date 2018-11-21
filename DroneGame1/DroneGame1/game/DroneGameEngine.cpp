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
	
	GameObject* drone = new GameObject("Drone1", "Assets/models/drone-1.obj", "Assets/glslfiles/basicTransformations", false);
	GameObject* tardis = new GameObject("Tardis", "Assets/models/tardis_1.obj", "Assets/glslfiles/basicTransformations", true);

	GLfloat currentAspect = (GLfloat)(screenWidth / screenHeight);
	Camera camera1 = Camera(20.0f, currentAspect, (GLfloat)0.1f, (GLfloat)500.0f, true, drone);
	Camera camera2 = Camera(40.0f, currentAspect, (GLfloat)0.1f, (GLfloat)500.0f, true, drone);
	Camera camera3 = Camera(60.0f, currentAspect, (GLfloat)0.1f, (GLfloat)500.0f, true, drone);

	myScene->addCamera(camera1, true);
	myScene->addCamera(camera2, false);
	myScene->addCamera(camera3, false);

	
	myScene->addObject(drone);
	myScene->addObject(tardis);
	
	tardis->parent = drone;

	scene_manager->addScene(myScene, true);

	input_manager->cooldown_config[VK_NUMPAD0] = 100;

}