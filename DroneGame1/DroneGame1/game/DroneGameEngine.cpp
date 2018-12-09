#include "DroneGameEngine.h"

DroneGameEngine::DroneGameEngine() : GameEngine3D() {

	

}

void DroneGameEngine::init() {

	//override GameEngine3D clear color
	clearColorRGBA[0] = 0.1f;
	clearColorRGBA[1] = 0.1f;
	clearColorRGBA[2] = 0.1f;
	clearColorRGBA[3] = 1.0f;
	GameEngine3D::init();
	
	Scene* myScene = new Scene();

	GameObject* drone = new GameObject("Drone1", "Assets/models/drone-1.obj", "Assets/glslfiles/basicTransformations", false);
	drone->physics = new Physics(2.0f, 1.0f, 0.5f, false, true, false);
	drone->physics->gravity = false;
	drone->radius = 1.0f;
	drone->material->normalMapping = 1;
	
	GameObject* tardis = new GameObject("Tardis", "Assets/models/tardis_1.obj", "Assets/glslfiles/basicTransformations", true);
	tardis->worldY = -10.f;
	tardis->worldX = -20.f;
	tardis->material->normalMapping = 1;
	tardis->radius = 5.0f;
	tardis->physics = new Physics(1.0f, 1.0f, 0.1f, false, true, false);

	GameObject* misc1 = new GameObject("Dragon", "Assets/models/dragon/dragon.obj", "Assets/glslfiles/basicTransformations", true);
	misc1->worldZ = 100.f;
	misc1->worldX = 5.f;
	misc1->worldY = -17.0f;
	misc1->scale = 2.0f;
	misc1->radius = 25.f;
	misc1->physics = new Physics();
	misc1->material->shininess = 0.5f;
	misc1->material->normalMapping = 1;


	GameObject* misc2 = new GameObject("Rock1", "Assets/models/rock/rock.obj", "Assets/glslfiles/basicTransformations", true);
	misc2->worldZ = 70.f;
	misc2->worldX = 10.f;
	misc2->worldY = -29.0f;
	misc2->physics = new Physics();


	GameObject* misc3 = new GameObject("Water", "Assets/models/water/water.obj", "Assets/glslfiles/basicTransformations", true);
	misc3->worldY = -60.0f;
	misc3->scale = 10.0f;

	GameObject* sky_clouds = new GameObject("SkyClouds", "Assets/models/sky/clouds.obj", "Assets/glslfiles/basicTransformations", true);
	GameObject* sky_stars = new GameObject("SkyStars", "Assets/models/sky/stars.obj", "Assets/glslfiles/basicTransformations", true);
	GameObject* sky_col = new GameObject("SkyHorizon", "Assets/models/sky/sky.obj", "Assets/glslfiles/basicTransformations", true);
	sky_clouds->material->shadeless = 1;
	sky_clouds->material->diffuse[4] = 0.1f;
	sky_stars->material->shadeless = 1;
	sky_col->material->diffuse[4] = 0.1f;
	sky_col->material->shadeless = 1;
	//sky_clouds->material->alphaClipThreshold = 0.5f;

	sky_clouds->parent = drone;
	sky_clouds->inheritRotation = false;

	sky_stars->parent = drone;
	sky_stars->inheritRotation = false;

	sky_col->parent = drone;
	sky_col->inheritRotation = false;
	sky_col->material->diffuse[3] = 0.2f;

	myScene->addObject(drone);
	myScene->addObject(tardis); 
	myScene->addObject(misc1);
	myScene->addObject(misc2);

	myScene->addObject(sky_stars);
	myScene->addObject(sky_col);
	myScene->addObject(sky_clouds);

	myScene->addObject(misc3);

	
	
	GameObject* terrain_1 = new GameObject("Terrain", "Assets/models/terrain_1.obj", "Assets/glslfiles/basicTransformations", true);
	terrain_1->worldY = -50.f;
	terrain_1->scale = 10.0f;
	terrain_1->physics = new Physics();
	//terrain_1->material->shadeless = 1;

	myScene->addObject(terrain_1);
	
	GLfloat currentAspect = (GLfloat)(screenWidth / screenHeight);
	Camera* camera1 = new Camera(60.0f, currentAspect, (GLfloat)0.1f, (GLfloat)650.0f, true, drone);
	Camera* camera2 = new Camera(40.0f, currentAspect, (GLfloat)0.1f, (GLfloat)650.0f, true, drone);
	Camera* camera3 = new Camera(20.0f, currentAspect, (GLfloat)0.1f, (GLfloat)650.0f, true, NULL);

	myScene->addCamera(camera1, true);
	myScene->addCamera(camera2, false);
	myScene->addCamera(camera3, false);

	camera1->name = "CAMERA1";
	camera2->name = "CAMERA2";
	camera3->name = "CAMERA3";

	camera1->inheritRotation = true;
	//camera2->inheritRotation = true;

	camera1->localZ = 10.0f;
	camera1->localY = 2.0f;
	camera2->localZ = 40.0f;
	camera3->worldZ = -120.f;
	camera3->worldX = 20.f;

	camera3->trackTarget = true;
	camera3->target = drone;

	//GameObject* droneLamp = GameObject::makeLight(glm::vec3(0.f, 2.f, -1.f), glm::vec4(0.4f, 0.4f, 0.9f, 1.f), 10.0f);
	//droneLamp->parent = drone;
	//myScene->lights.push_back(drone);
	drone->myLight = new Light(glm::vec4(0.4f, 0.9f, 0.5f, 1.f), 5.0f);
	myScene->lights.push_back(drone);

	GameObject* sun = GameObject::makeLight(glm::vec3(0.f, 20.f, 0.f), glm::vec4(0.8f, 0.7f, 0.1f, 1.f), 15.0f);
	sun->myLight->specular[0] = 0.1f;
	sun->myLight->specular[1] = 0.1f;
	sun->myLight->specular[2] = 0.1f;
	myScene->lights.push_back(sun);
	
	scene_manager->addScene(myScene, true);

	input_manager->cooldown_config[VK_F2] = 40;
	input_manager->cooldown_config[VK_NUMPAD0] = 100;

}

void DroneGameEngine::processKeys()
{

	float spinXinc = 0, spinYinc = 0, spinZinc = 0;
	if (input_manager->isInputActivated(VK_UP))
	{
		spinXinc += 0.01f;
	}
	if (input_manager->isInputActivated(VK_DOWN))
	{
		spinXinc += -0.01f;
	}
	if (input_manager->isInputActivated(VK_LEFT))
	{
		spinYinc += 0.01f;
	}
	if (input_manager->isInputActivated(VK_RIGHT))
	{
		spinYinc += -0.01f;
	}
	if (input_manager->isInputActivated(VK_SPACE))
	{
		spinZinc += 0.01f;
	}
	if (input_manager->isInputActivated(VK_SHIFT))
	{
		spinZinc += -0.01f;
	}

	int WKEY = 0x57;
	int AKEY = 0x41;
	int SKEY = 0x53;
	int DKEY = 0x44;
	int EKEY = 0x45;
	int QKEY = 0x51;
	int MODIFIER = VK_LCONTROL;

	float worldX = 0, worldY = 0, worldZ = 0;
	float localX = 0, localY = 0, localZ = 0;
	if (input_manager->isInputActivated(WKEY))
	{
		localZ += -0.01f;
	}
	if (input_manager->isInputActivated(SKEY))
	{
		localZ += 0.01f;
	}
	if (input_manager->isInputActivated(AKEY))
	{
		localX += -0.01f;
	}
	if (input_manager->isInputActivated(DKEY))
	{
		localX += 0.01f;
	}
	if (input_manager->isInputActivated(QKEY))
	{
		localY += -0.01f;
	}
	if (input_manager->isInputActivated(EKEY))
	{
		localY += 0.02f;
	}


	if (input_manager->isInputActivated(VK_NUMPAD8))
	{
		worldZ += 0.01f;
	}
	if (input_manager->isInputActivated(VK_NUMPAD2))
	{
		worldZ += -0.01f;
	}
	if (input_manager->isInputActivated(VK_NUMPAD4))
	{
		worldX += 0.01f;
	}
	if (input_manager->isInputActivated(VK_NUMPAD6))
	{
		worldX += -0.01f;
	}
	if (input_manager->isInputActivated(VK_NUMPAD7))
	{
		worldY += 0.01f;
	}
	if (input_manager->isInputActivated(VK_NUMPAD9))
	{
		worldY += -0.01f;
	}


	float speed = 100.0f;
	float moveSpeed = 25.0f;

	float fovDelta = 0.f;
	if (input_manager->isInputActivated(VK_ADD)) {
		fovDelta = -1.0f;
	}
	if (input_manager->isInputActivated(VK_SUBTRACT)) {
		fovDelta = 1.0f;
	}


	if (current_scene != NULL) {
		GameObject* firstObject = current_scene->getGameObjects()[0];
		
		firstObject->spinXinc = spinXinc * speed;
		firstObject->spinYinc = spinYinc * speed;
		firstObject->spinZinc = spinZinc * speed;


		bool drawDebug = input_manager->isInputActivated(VK_F1);

		if (input_manager->isInputActivated(VK_F2)) {
			firstObject->physics->gravity = !firstObject->physics->gravity;
		}


		bool cameraAutoFov = !input_manager->isInputActivated(VK_F3);
		Camera* current_camera = current_scene->getCurrentCamera();
		if (cameraAutoFov) {
			cameraUpdateFovAuto(current_camera, firstObject->physics->oldForces[0], firstObject->physics->oldForces[1], firstObject->physics->oldForces[2]);
		}
		else {
			cameraUpdateFovManual(current_camera, fovDelta);
		}

		firstObject->addForce(localX * moveSpeed, localY * moveSpeed, localZ * moveSpeed);



		for (int go = 0; go < (int)current_scene->objects.size(); go++) {
			current_scene->objects[go]->drawBounds = drawDebug;
			current_scene->objects[go]->drawOctree = drawDebug;
		}

		GameObject* secondObject = current_scene->getGameObjects()[1];
		secondObject->localX += worldX;
		secondObject->localY += worldY;
		secondObject->localZ += worldZ;


		GameObject* clouds = current_scene->getGameObjects()[6];
		clouds->spinYinc = 0.01f;
		clouds->material->uvOffset[1] = (clouds->material->uvOffset[0] + 0.0006f);
		//clouds->material->uvOffset[1] = (clouds->material->uvOffset[1] + 0.0001f);

		clouds = current_scene->getGameObjects()[4];
		clouds->spinYinc = -0.002f;


		GameObject* water = current_scene->getGameObjects()[7];
		water->spinYinc = 0.07f;
		water->material->uvOffset[0] = (clouds->material->uvOffset[0] + 0.1f);
		water->material->uvOffset[1] = (clouds->material->uvOffset[1] + 0.1f);

		if (input_manager->isInputActivated(VK_NUMPAD0)) {
			current_scene->nextCamera();
		}

	}


}
