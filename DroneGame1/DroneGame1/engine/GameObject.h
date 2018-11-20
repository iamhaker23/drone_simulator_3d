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

#include <map>

class GameObject {
private:

	static OBJLoader objLoader;

	glm::mat4 objectRotation;
	glm::quat q;
	glm::mat4 modelViewMatrix;

	//load once
	static vector<Shader*> shaderList;
	static vector<ThreeDModel*> modelList;
	static map<int, string> shadersLoaded;
	static map<int, string> modelsLoaded;

	int shaderIdx;
	int modelIdx;

	float Material_Ambient[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float Material_Diffuse[4] = { 0.8f, 0.8f, 0.5f, 1.0f };
	float Material_Specular[4] = { 0.9f,0.9f,0.8f,1.0f };
	float Material_Shininess = 5;
	float Light_Ambient_And_Diffuse[4] = { 0.8f, 0.8f, 0.6f, 1.0f };
	float Light_Specular[4] = { 1.0f,1.0f,1.0f,1.0f };
	float LightPos[4] = { 0.0f, 5.0f, 10.0f, 0.0f };

public:

	float spinXinc = 0, spinYinc = 0, spinZinc = 0;
	float worldX = 0, worldY = 0, worldZ = 0;
	float localX = 0, localY = 0, localZ = 0;
	bool drawBounds = false, drawOctree = false;

	string name;

	GameObject();
	GameObject(string name, string modelPath, string shaderPath);
	~GameObject();
	GameObject(const GameObject &copy);

	void updateTransformation();
	int doModelLoad(string modelPath);
	int doShaderLoad(string shaderPath);

	static void clearResources() {

		while (modelList.size() > 0) {
			modelList.erase(modelList.begin());
		}

		while (shaderList.size() > 0) {
			shaderList.erase(shaderList.begin());
		}
	}

	void draw(glm::mat4 projectionMatrix);

};