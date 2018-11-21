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

void GameEngine3D::processKeys()
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
		localZ += 0.01f;
	}
	if (input_manager->isInputActivated(SKEY))
	{
		localZ += -0.01f;
	}
	if (input_manager->isInputActivated(AKEY))
	{
		localX += 0.01f;
	}
	if (input_manager->isInputActivated(DKEY))
	{
		localX += -0.01f;
	}
	if (input_manager->isInputActivated(QKEY))
	{
		localY += 0.01f;
	}
	if (input_manager->isInputActivated(EKEY))
	{
		localY += -0.01f;
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

	bool drawDebug = input_manager->isInputActivated(VK_F1);

	float speed = 100.0f;

	if (current_scene != NULL) {
		GameObject* firstObject = current_scene->getGameObjects()[0];
		firstObject->spinXinc = spinXinc * speed;
		firstObject->spinYinc = spinYinc * speed;
		firstObject->spinZinc = spinZinc * speed;

		firstObject->worldX += worldX;
		firstObject->worldY += worldY;
		firstObject->worldZ += worldZ;

		firstObject->localX += localX;
		firstObject->localY += localY;
		firstObject->localZ += localZ;

		firstObject->drawBounds = drawDebug;
		firstObject->drawOctree = drawDebug;

		if (input_manager->isInputActivated(VK_NUMPAD0)){
			current_scene->nextCamera();
		}
		
		if (input_manager->isInputActivated(VK_TAB)) {
			current_scene->setCameraTargetTrack(true);
		}
		else {
			//current_scene->setCameraRotation(spinXinc, spinYinc, spinZinc);
			//current_scene->setCameraPosition(localX, localY, localZ);
		}

	}
	

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
	//glClear(GL_COLOR_BUFFER_BIT) in the display function
	//will clear the buffer to this colour

	glEnable(GL_DEPTH_TEST);

}

void GameEngine3D::update()
{

	//collisions
	//physics
	//constraints

	current_scene = scene_manager->getActiveScene();
	

}

void GameEngine3D::resize(int width, int height)
{
	this->screenWidth = width; 
	this->screenHeight = height;

	glViewport(0, 0, width, height);
	scene_manager->resizeCameras((GLfloat)width, (GLfloat)height);
	
}
