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
private:

	//TODO: Refactor
	float amount = 0;
	float temp = 0.5f;
	Scene* current_scene;

public:
	GameEngine3D();
	GameEngine3D(const GameEngine3D &copy);
	~GameEngine3D();


	void processKeys();
	void draw();
	void init();
	void update();
	void resize(int width, int height);
};