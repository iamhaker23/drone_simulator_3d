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
	drone->physicsSettings = new Physics(2.0f, 1.0f, 0.5f, false, true, true);
	
	GameObject* tardis = new GameObject("Tardis", "Assets/models/tardis_1.obj", "Assets/glslfiles/basicTransformations", true);
	tardis->localY = -5.f;
	tardis->slowParentFactor = 5.f;

	GameObject* box = new GameObject("Box", "Assets/models/box.obj", "Assets/glslfiles/basicTransformations", true);
	box->worldZ = -100.f;
	box->worldX = 5.f;

	GLfloat currentAspect = (GLfloat)(screenWidth / screenHeight);
	Camera* camera1 = new Camera(60.0f, currentAspect, (GLfloat)0.1f, (GLfloat)500.0f, true, drone);
	Camera* camera2 = new Camera(40.0f, currentAspect, (GLfloat)0.1f, (GLfloat)500.0f, true, drone);
	Camera* camera3 = new Camera(20.0f, currentAspect, (GLfloat)0.1f, (GLfloat)500.0f, true, NULL);

	myScene->addCamera(camera1, true);
	myScene->addCamera(camera2, false);
	myScene->addCamera(camera3, false);

	camera1->name = "CAMERA1";
	camera2->name = "CAMERA2";
	camera3->name = "CAMERA3";

	camera1->inheritRotation = true;
	//camera2->inheritRotation = true;

	camera1->localZ = 30.0f;
	camera2->localZ = 20.0f;
	camera3->localZ = 50.0f;
	camera3->localY = -10.0f;

	camera3->trackTarget = true;
	camera3->target = drone;
	
	myScene->addObject(drone);
	myScene->addObject(tardis);
	myScene->addObject(box);

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			GameObject* tmp = new GameObject("Axes", "Assets/models/axes.obj", "Assets/glslfiles/basicTransformations", true);
			tmp->worldZ = -100.f;
			tmp->worldX = -20.f +(-10.f * i);
			tmp->worldY = -30.f + (10.f * j);
			myScene->addObject(tmp);
		}
	}
	
	tardis->parent = drone;
	
	scene_manager->addScene(myScene, true);

	input_manager->cooldown_config[VK_NUMPAD0] = 100;

}