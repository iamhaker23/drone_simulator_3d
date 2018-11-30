#pragma once

#include "gl/glew.h"
#include "gl/wglew.h"
#pragma comment(lib, "glew32.lib")
#include "shaders/Shader.h"   // include shader header file, this is not part of OpenGL
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include "../lib/3DStruct/threeDModel.h"
#include "../lib/Obj/OBJLoader.h"

#include "GameEngine.h"
#include "Cameras.h"

class GameEngine3D : public GameEngine {
protected:
	Scene* current_scene;

public:
	GameEngine3D();
	GameEngine3D(const GameEngine3D &copy);
	~GameEngine3D();

	void cameraUpdateFovManual(Camera* current_camera, float fovDelta);
	void cameraUpdateFovAuto(Camera* current_camera, float xSpeed, float ySpeed, float zSpeed);
	void cameraUpdateFov(Camera* current_camera, float xSpeed, float ySpeed, float zSpeed, bool automaticMode, float fovDelta);

	void draw();
	void init();
	void update();
	void resize(int width, int height);
};