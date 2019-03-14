#include "DroneGameEngine.h"

float DroneGameEngine::skyColMultiplier = 1.f;

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


	GameObject* rotor1 = new GameObject("Rotor1", "Assets/models/rotor/rotor.obj", "Assets/glslfiles/basicTransformations", false);
	rotor1->parent = drone;
	rotor1->localY = -1.f;
	rotor1->scale = 0.3f;
	rotor1->inheritRotation = true;

	GameObject* drone2 = new GameObject("Drone2", "Assets/models/shack/shack.obj", "Assets/glslfiles/basicTransformations", false);
	drone2->scale = 0.3f;
	drone2->physics = new Physics(2.0f, 1.0f, 0.5f, false, true, false);
	drone2->physics->gravity = true;
	drone2->radius = 1.0f;
	drone2->material->normalMapping = 1;

	GameObject* rotor2 = new GameObject("Rotor2", "Assets/models/rotor/rotor.obj", "Assets/glslfiles/basicTransformations", false);
	rotor2->parent = drone2;
	rotor2->localY = 0.8f;
	rotor2->inheritRotation = true;

	GameObject* person = new GameObject("Person", "Assets/models/person/person.obj", "Assets/glslfiles/basicTransformations", true);
	person->worldY = -20.f;
	//person->material->normalMapping = 1;
	//person->radius = 5.0f;
	person->physics = new Physics(1.0f, 1.0f, 0.1f, false, true, false);
		
	GameObject* water = new GameObject("Water", "Assets/models/water/water.obj", "Assets/glslfiles/water", true);
	water->worldY = -60.0f;
	water->scale = 10.0f;

	GameObject* sky_clouds = new GameObject("SkyClouds", "Assets/models/sky/clouds.obj", "Assets/glslfiles/basicTransformations", true);
	GameObject* sky_stars = new GameObject("SkyStars", "Assets/models/sky/stars.obj", "Assets/glslfiles/basicTransformations", true);
	GameObject* sky_col = new GameObject("SkyHorizon", "Assets/models/sky/sky.obj", "Assets/glslfiles/basicTransformations", true);

	sky_clouds->debugEnabled = false;
	sky_stars->debugEnabled = false;
	sky_col->debugEnabled = false;

	sky_clouds->scale = 2.0f;
	sky_stars->scale = 2.0f;
	sky_col->scale = 2.0f;

	sky_clouds->material->shadeless = 1;
	sky_clouds->material->diffuse[3] = 0.5f;
	sky_stars->material->shadeless = 1;
	sky_col->material->diffuse[3] = 0.1f;
	sky_col->material->shadeless = 1;

	sky_clouds->parent = drone;
	sky_clouds->inheritRotation = false;

	sky_stars->parent = drone;
	sky_stars->inheritRotation = false;

	sky_col->parent = drone;
	sky_col->inheritRotation = false;
	sky_col->material->diffuse[3] = 0.2f;


	GameObject* prop = new GameObject("Prop", "Assets/models/truck/truck.obj", "Assets/glslfiles/basicTransformations", true);
	prop->worldX = 85.f;
	prop->worldY = -18.0f;
	prop->scale = 4.0f;
	prop->physics = new Physics();
	prop->material->shininess = 120.0f;

	GameObject* prop2 = new GameObject("Prop2", "Assets/models/tv/tv.obj", "Assets/glslfiles/basicTransformations", true);
	prop2->worldX = -50.f;
	prop2->worldZ = 10.0f;
	prop2->worldY = 18.0f;
	prop2->physics = new Physics(1.0f, 1.0f, 0.1f, false, true, false);
	prop2->material->shininess = 120.0f; 
	prop2->myLight = new Light(glm::vec4(0.5f, 0.5f, 0.9f, 1.f), 12.0f);
	prop2->myLight->selfLight = true;
	myScene->lights.push_back(prop2);

	//TODO: Indexed in update function, use member variables instead
	myScene->addObject(drone);
	myScene->addObject(person);
	myScene->addObject(sky_stars);
	myScene->addObject(sky_clouds);
	myScene->addObject(sky_col);
	myScene->addObject(water);
	myScene->addObject(drone2);

	myScene->addObject(rotor1);
	myScene->addObject(rotor2);

	//not used via index in update
	myScene->addObject(prop);
	myScene->addObject(prop2);
	
	generateHouses(myScene);
	generateSkyscrapers(myScene);
	generateRocks(myScene);
	generateTrees(myScene);
	

	GameObject* terrain_1 = new GameObject("Terrain", "Assets/models/terrain/terrain.obj", "Assets/glslfiles/basicTransformations", true);
	terrain_1->worldY = -62.5f;
	terrain_1->scale = 20.0f;
	//terrain_1->physics = new Physics();
	terrain_1->material->normalMapping = 1;
	terrain_1->material->shininess = 0.2f;

	myScene->addObject(terrain_1);
	
	GLfloat currentAspect = (GLfloat)(screenWidth / screenHeight);
	Camera* camera1 = new Camera(90.0f, currentAspect, (GLfloat)0.1f, (GLfloat)1500.0f, true, drone);
	Camera* camera2 = new Camera(40.0f, currentAspect, (GLfloat)0.1f, (GLfloat)1500.0f, true, drone);
	Camera* camera3 = new Camera(20.0f, currentAspect, (GLfloat)0.1f, (GLfloat)1500.0f, true, NULL);

	myScene->addCamera(camera1, true);
	myScene->addCamera(camera2, false);
	myScene->addCamera(camera3, false);

	camera1->name = "CAMERA1";
	camera2->name = "CAMERA2";
	camera3->name = "CAMERA3";

	camera1->inheritRotation = true;
	camera2->inheritRotation = true;

	camera1->localZ = 3.5f;
	camera1->localY = 2.f;

	camera2->localZ = 40.0f;
	
	camera3->localY = 8.0f;
	camera3->parent = person;
	camera3->trackTarget = true;
	camera3->target = drone;


	drone->myLight = new Light(glm::vec4(0.4f, 0.9f, 0.5f, 1.f), 8.0f);
	//shiny drone
	//drone->myLight->selfLight = true;
	myScene->lights.push_back(drone);

	GameObject* sun = GameObject::makeLight(glm::vec3(0.f, 20.f, 0.f), glm::vec4(0.8f, 0.7f, 0.1f, 1.f), 15.0f);
	sun->myLight->specular[0] = 0.1f;
	sun->myLight->specular[1] = 0.1f;
	sun->myLight->specular[2] = 0.1f;
	myScene->lights.push_back(sun);
	
	scene_manager->addScene(myScene, true);

	input_manager->cooldown_config[VK_F2] = 40;
	input_manager->cooldown_config[VK_NUMPAD0] = 40;

}

void DroneGameEngine::generateHouses(Scene* myScene) {
	
	for (int i = 0; i < 3; i++) {
		GameObject* house = new GameObject("House", "Assets/models/house/house.obj", "Assets/glslfiles/basicTransformations", true);
		house->worldX = -120.f+ (i*70.f);
		house->worldZ = 100.f;
		house->worldY = -5.0f;
		house->scale = 10.f;
		house->material->normalMapping = 1;
		house->material->shininess = 0.2f;
		house->physics = new Physics();

		myScene->addObject(house);
	}
}

void DroneGameEngine::generateRocks(Scene* myScene) {

	for (int i = 0; i < 10; i++) {
		GameObject* rock = new GameObject("Rock", "Assets/models/rock/rock.obj", "Assets/glslfiles/basicTransformations", true);
		rock->scale = 2.0f;
		rock->worldX = -150.f + (i * 30.f);
		rock->worldY = -28.f;
		rock->worldZ = (i % 2 == 0) ? 10.f + (i * -15.0f) : -50.f + (i * -10.f);
		rock->material->normalMapping = 1;
		rock->radius = 5.0f;
		rock->physics = new Physics();

		myScene->addObject(rock);
	}

}


void DroneGameEngine::generateTrees(Scene* myScene) {

	for (int i = 0; i < 8; i++) {
		GameObject* tree = new GameObject("Tree", "Assets/models/tree/tree.obj", "Assets/glslfiles/basicTransformations", true);
		tree->scale = 10.0f;
		tree->worldX = -120.f + (i * 30.f);
		tree->worldY = -15.f;
		tree->worldZ = (i % 2 == 0) ? 25.f + (i * -20.0f) : -25.f + (i * -15.f);
		tree->material->normalMapping = 1;
		//tree->radius = 5.0f;
		tree->physics = new Physics();

		myScene->addObject(tree);
	}

}
void DroneGameEngine::generateSkyscrapers(Scene* myScene) {

	for (int i = 0; i < 4; i++) {
		GameObject* building = new GameObject("Skyscraper", "Assets/models/skyscraper/skyscraper.obj", "Assets/glslfiles/basicTransformations", true);
		building->scale = 5.f;
		building->worldZ = -150.f + (i * 80.f);
		building->worldY = -15.f;
		building->worldX = (i % 2 == 0) ? -200.f + (i * -20.0f) : 200.f + (i * 60.f);
		//building->material->normalMapping = 1;
		building->physics = new Physics();

		myScene->addObject(building);
	}

	for (int i = 0; i < 4; i++) {
		GameObject* building = new GameObject("Skyscraper2", "Assets/models/skyscraper2/skyscraper2.obj", "Assets/glslfiles/basicTransformations", true);
		building->scale = 2.f;
		building->worldX = -150.f + (i * 80.f);
		building->worldY = -15.f;
		building->worldZ = (i % 2 == 0) ? -200.f + (i * -20.0f) : 200.f + (i * 60.f);
		//building->material->normalMapping = 1;
		//tree->radius = 5.0f;
		building->physics = new Physics();

		myScene->addObject(building);
	}

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

		bool deactivateAI = input_manager->isInputActivated(VK_F4);

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


		for (int go = 0; go < (int)current_scene->grid->objects.size(); go++) {
			current_scene->grid->objects[go]->drawBounds = drawDebug;
			current_scene->grid->objects[go]->drawOctree = drawDebug;
		}

		GameObject* secondObject = current_scene->getGameObjects()[1];
		secondObject->localX += worldX;
		secondObject->localY += worldY;
		secondObject->localZ += worldZ;

		GameObject* clouds = current_scene->getGameObjects()[2];
		clouds->spinYinc = -0.02f;
		clouds->material->uvOffset[0] = (clouds->material->uvOffset[0] - 0.0001f);
		clouds = current_scene->getGameObjects()[3];
		clouds->spinYinc = 0.01f;
		clouds->material->uvOffset[1] = (clouds->material->uvOffset[1] + 0.0001f);
		//clouds->material->uvOffset[1] = (clouds->material->uvOffset[1] + 0.0001f);

		GameObject* skyCol = current_scene->getGameObjects()[4];

		skyCol->material->diffuse[3] = skyCol->material->diffuse[3] + (0.01f * skyColMultiplier);
		//cout << skyCol->material->diffuse[3] << endl;
		if (skyCol->material->diffuse[3] > 0.99f) skyColMultiplier = -1.f;
		else if (skyCol->material->diffuse[3] < 0.0f) skyColMultiplier = 1.f;

		GameObject* water = current_scene->getGameObjects()[5];
		water->material->uvOffset[0] = (water->material->uvOffset[0] + 0.0001f);
		water->material->uvOffset[1] = (water->material->uvOffset[1] + 0.0001f);

		GameObject* droneai = current_scene->getGameObjects()[6];
		//droneai->addForce(10.f, 10.f, 10.f);

		if (!deactivateAI) {
			droneai->addForce(
				std::cosf(GameObject::time)
				, (std::sinf(GameObject::time)) + 0.35f
				, std::sinf(GameObject::time)
			);
		}

		GameObject* rotor = current_scene->getGameObjects()[7];
		
		if ( (firstObject->worldY >= GameObject::yAxisFloor + (firstObject->extent / 2.f) + 0.05f)
		|| (input_manager->isInputActivated(WKEY) ||
			input_manager->isInputActivated(SKEY) ||
			input_manager->isInputActivated(AKEY) ||
			input_manager->isInputActivated(DKEY) ||
			input_manager->isInputActivated(QKEY) ||
			input_manager->isInputActivated(EKEY) ||
			input_manager->isInputActivated(VK_UP) ||
			input_manager->isInputActivated(VK_DOWN) ||
			input_manager->isInputActivated(VK_LEFT) ||
			input_manager->isInputActivated(VK_RIGHT) ||
			input_manager->isInputActivated(VK_SHIFT) ||
			input_manager->isInputActivated(VK_SPACE)
			))
		{
			rotor->spinYinc += 20.f;
		}

		if (droneai->worldY >= GameObject::yAxisFloor + (droneai->extent / 2.f) + 0.05f) {
			rotor = current_scene->getGameObjects()[8];
			rotor->spinYinc += 20.f;
		}

		if (input_manager->isInputActivated(VK_NUMPAD0)) {
			current_scene->nextCamera();
		}

	}


}
