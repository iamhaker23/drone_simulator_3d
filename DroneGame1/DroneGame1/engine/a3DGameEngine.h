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

#include "aGameEngine.h"

class a3DGameEngine : public aGameEngine {
private:
	glm::mat4 projectionMatrix;
	glm::mat4 modelViewMatrix;

	//TODO: Refactor
	ThreeDModel model, modelbox;
	OBJLoader objLoader;
	Shader* myShader;
	Shader* myBasicShader;
	float Material_Ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float Material_Diffuse[4] = { 0.8f, 0.8f, 0.5f, 1.0f };
	float Material_Specular[4] = { 0.9f,0.9f,0.8f,1.0f };
	float Material_Shininess = 50;
	float Light_Ambient_And_Diffuse[4] = { 0.8f, 0.8f, 0.6f, 1.0f };
	float Light_Specular[4] = { 1.0f,1.0f,1.0f,1.0f };
	float LightPos[4] = { 0.0f, 0.0f, 1.0f, 0.0f }; 
	float amount = 0;
	float temp = 0.5f;
	glm::mat4 objectRotation;
	glm::quat q;

public:
	a3DGameEngine();
	a3DGameEngine(const a3DGameEngine &copy);
	~a3DGameEngine();


	void processKeys();
	void draw();
	void init();
	void update();
	void resize(int width, int height);
};