#pragma once

#include "../engine/GameEngine3D.h"

class DroneGameEngine : public GameEngine3D {


public:

	static float DroneGameEngine::skyColMultiplier;

	DroneGameEngine();
	void init();
	void processKeys();

	void generateRocks(Scene* myScene);
	void generateHouses(Scene* myScene);
	void generateTrees(Scene* myScene);
	void generateSkyscrapers(Scene* myScene);

};