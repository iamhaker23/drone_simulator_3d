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
#include "Physics.h"
#include "Light.h"
#include "Material.h"

#include <map>

class GameObject {
private:

	static OBJLoader objLoader;

	glm::mat4 worldRotation;

	//load once
	static vector<Shader*> shaderList;
	static vector<ThreeDModel*> modelList;
	static map<int, string> shadersLoaded;
	static map<int, string> modelsLoaded;

	int shaderIdx;
	int modelIdx;

public:
	float slowParentFactor;
	glm::mat4 worldPositionMatrix;
	glm::mat4 modelViewMatrix;
	GameObject* parent;
	bool inheritRotation = false;
	Physics* physics;
	Material* material;
	Light* activeLights[4] = { 0 };

	float spinXinc = 0, spinYinc = 0, spinZinc = 0;
	float worldX = 0, worldY = 0, worldZ = 0;
	float localX = 0, localY = 0, localZ = 0;
	bool drawBounds = false, drawOctree = false;
	float scale = 1.0f;

	string name;

	GameObject();
	GameObject(string name, string modelPath, string shaderPath, bool inheritRotation);
	~GameObject();
	GameObject(const GameObject &copy);

	int doModelLoad(string modelPath);
	int doShaderLoad(string shaderPath);
	glm::mat4 getRotationMatrix(float xRot, float yRot, float zRot);
	void updateTransformation();

	static void clearResources() {

		while (modelList.size() > 0) {
			modelList.erase(modelList.begin());
		}

		while (shaderList.size() > 0) {
			shaderList.erase(shaderList.begin());
		}
	}

	void draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
	void addForce(float x, float y, float z);
	void doCollisionsAndApplyForces();


};