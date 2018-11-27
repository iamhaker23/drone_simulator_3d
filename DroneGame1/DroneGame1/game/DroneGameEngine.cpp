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
	drone->physics = new Physics(2.0f, 1.0f, 0.5f, false, true, false);
	
	GameObject* tardis = new GameObject("Tardis", "Assets/models/tardis_1.obj", "Assets/glslfiles/basicTransformations", true);
	tardis->localY = -5.f;
	tardis->slowParentFactor = 5.f;

	GameObject* misc1 = new GameObject("Dragon", "Assets/models/dragon.obj", "Assets/glslfiles/basicTransformations", true);
	misc1->worldZ = 100.f;
	misc1->worldX = 5.f;
	misc1->scale = 5.0f;


	misc1->material->shininess = 5.f;
	misc1->material->ambient[0] = 1.f;
	misc1->material->ambient[1] = .8f;
	misc1->material->ambient[2] = .8f;
	misc1->material->ambient[3] = 1.f;


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
	camera2->localZ = 40.0f;
	camera3->worldZ = -120.f;
	camera3->worldX = 20.f;

	camera3->trackTarget = true;
	camera3->target = drone;
	
	myScene->addObject(drone);
	myScene->addObject(tardis); 
	myScene->addObject(misc1);
	
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			GameObject* tmp = new GameObject("Axes", "Assets/models/axes.obj", "Assets/glslfiles/basicTransformations", true);
			tmp->worldZ = -100.f;
			tmp->worldX = -20.f +(-25.f * i);
			tmp->worldY = (25.f * j);
			myScene->addObject(tmp);
		}
	}

	GameObject* terrain_1 = new GameObject("Axes", "Assets/models/terrain_1.obj", "Assets/glslfiles/basicTransformations", true);
	terrain_1->worldY = -30.f;
	terrain_1->scale = 10.0f;

	terrain_1->material->shininess = 0.5f;
	terrain_1->material->ambient[0] = 0.8f;
	terrain_1->material->ambient[1] = 0.8f;
	terrain_1->material->ambient[2] = 0.8f;
	terrain_1->material->ambient[3] = 1.f;
	terrain_1->material->specular[0] = 0.3f;
	terrain_1->material->specular[1] = 0.3f;
	terrain_1->material->specular[2] = 0.3f;
	terrain_1->material->specular[3] = 1.f;


	myScene->addObject(terrain_1);
	
	tardis->parent = drone;
	
	//myScene->lights.push_back(new Light(glm::vec3(0.f, 0.f, 0.f), glm::vec4(1.f, 1.f, 1.f, 1.f)));
	myScene->lights.push_back(new Light());

	scene_manager->addScene(myScene, true);

	input_manager->cooldown_config[VK_F2] = 40;
	input_manager->cooldown_config[VK_NUMPAD0] = 100;

}