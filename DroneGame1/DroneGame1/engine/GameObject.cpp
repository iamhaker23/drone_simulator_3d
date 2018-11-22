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

#include "GameObject.h"

OBJLoader GameObject::objLoader;
vector<Shader*> GameObject::shaderList;
vector<ThreeDModel*> GameObject::modelList;
map<int, string> GameObject::shadersLoaded;
map<int, string> GameObject::modelsLoaded;

GameObject::GameObject() {
	shaderIdx = -1;
	modelIdx = -1;
	GameObject::shadersLoaded = { };
	GameObject::modelsLoaded = { };

	worldPositionMatrix = glm::translate(glm::mat4(1.0), glm::vec3(worldX, worldY, worldZ));

	parent = NULL;
}

GameObject::GameObject(string name, string modelPath, string shaderPath, bool inheritRotation) {
	
	this->inheritRotation = inheritRotation;
	parent = NULL;
	
	this->name = name;
	shaderIdx = -1;
	modelIdx = -1;

	worldPositionMatrix = glm::translate(glm::mat4(1.0), glm::vec3(worldX, worldY, worldZ));
	
	if (shadersLoaded.size() > 0) {
		for (const auto& loadedShader : shadersLoaded) {
			if (loadedShader.second == shaderPath) {
				shaderIdx = loadedShader.first;
			}
		}
	}
	if (shaderIdx == -1) {
		shaderIdx = doShaderLoad(shaderPath);
	}
	if (shadersLoaded.size() > 0) {
		for (const auto& loadedModel : modelsLoaded) {
			if (loadedModel.second == modelPath) {
				modelIdx = loadedModel.first;
			}
		}
	}
	if (modelIdx == -1) {
		modelIdx = doModelLoad(modelPath);
	}
	

}

GameObject::~GameObject() {

}

GameObject::GameObject(const GameObject & copy) {
	this->localX = copy.localX;
	this->localY = copy.localY;
	this->localZ = copy.localZ;

	this->spinXinc = copy.spinXinc;
	this->spinYinc = copy.spinYinc;
	this->spinZinc = copy.spinZinc;

	this->inheritRotation = copy.inheritRotation;
	this->modelViewMatrix = copy.modelViewMatrix;

	this->shaderIdx = copy.shaderIdx;
	this->modelIdx = copy.modelIdx;

	this->name = copy.name + "_COPY";

	this->parent = copy.parent;

}

glm::mat4 GameObject::getRotationMatrix(float xRot, float yRot, float zRot) {
	glm::mat4 matrixX, matrixXY;
	glm::quat q;

	//rotation about the local x axis
	q = glm::angleAxis(spinXinc, glm::vec3(1, 0, 0));
	matrixX = glm::mat4_cast(q);

	//rotation about the local y axis
	q = glm::angleAxis(spinYinc, glm::vec3(matrixX[1][0], matrixX[1][1], matrixX[1][2]));
	matrixXY = glm::mat4_cast(q) * matrixX;

	//EXAMPLE ACCESSING WITH 2D GLM structure.
	//rotation about the local z axis
	q = glm::angleAxis(spinZinc, glm::vec3(matrixXY[2][0], matrixXY[2][1], matrixXY[2][2]));

	//matrixXYZ
	return glm::mat4_cast(q) * matrixXY;
}

void GameObject::updateTransformation() {
		
	if (parent != NULL && inheritRotation) {
		worldRotation = parent->worldRotation;
		worldRotation *= getRotationMatrix(spinXinc, spinYinc, spinZinc);
	}

	//apply local transformation
	glm::mat4 localPosition = glm::translate(glm::mat4(1), glm::vec3(localX, localY, localZ));
		
	if (parent == NULL) {
		//cumulative rotation
		worldRotation *= getRotationMatrix(spinXinc, spinYinc, spinZinc);

		//reset local transform (current change will be accumulated in modelViewMatrix)
		localX = 0;
		localY = 0;
		localZ = 0;
	}
	
	//update ViewModel
	worldPositionMatrix = (parent != NULL) ? parent->worldPositionMatrix : glm::translate(glm::mat4(1), glm::vec3(worldX, worldY, worldZ));

	//apply rotation and translations to modelViewMatrix
	modelViewMatrix = worldPositionMatrix * worldRotation * localPosition;

	//update worldPosition (localtransform is accumulated into worldPosition modelViewMatrix)
	worldX = modelViewMatrix[3][0];
	worldY = modelViewMatrix[3][1];
	worldZ = modelViewMatrix[3][2];
	
}

void GameObject::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix ) {

	glMatrixMode(GL_MODELVIEW);


	updateTransformation();
	
	Shader* shaderHandle = shaderList[shaderIdx];
	ThreeDModel* modelHandle = modelList[modelIdx];

	glUseProgram(shaderHandle->handle());  // use the shader

	GLuint matLocation = glGetUniformLocation(shaderHandle->handle(), "ProjectionMatrix");
	glUniformMatrix4fv(matLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	
	glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "LightPos"), 1, LightPos);
	glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "light_ambient"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "light_diffuse"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "light_specular"), 1, Light_Specular);

	glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "material_ambient"), 1, Material_Ambient);
	glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "material_diffuse"), 1, Material_Diffuse);
	glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "material_specular"), 1, Material_Specular);
	glUniform1f(glGetUniformLocation(shaderHandle->handle(), "material_shininess"), Material_Shininess);
	
	glm::mat4 eyeSpace = (viewMatrix) * modelViewMatrix;

	glUniformMatrix4fv(glGetUniformLocation(shaderHandle->handle(), "ViewMatrix"), 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderHandle->handle(), "ModelViewMatrix"), 1, GL_FALSE, &eyeSpace[0][0]);
	
	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(modelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(shaderHandle->handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

	modelHandle->drawElementsUsingVBO(shaderHandle);
	if (drawBounds) modelHandle->drawBoundingBox(shaderHandle);
	if (drawOctree) modelHandle->drawOctreeLeaves(shaderHandle);
}

int GameObject::doShaderLoad(string shaderPath) {
	Shader* shader = new Shader();


	//convert string to char*
	string shaderPathFragStr = shaderPath + ".frag";
	string shaderPathVertStr = shaderPath + ".vert";
	
	if (!shader->load("BasicView", shaderPathVertStr.c_str(), shaderPathFragStr.c_str()))
	{
		cout << "failed to load shader" << endl;
	}
	else {
		shaderList.push_back(shader);
		GameObject::shadersLoaded.insert(pair<int, string>((int)shaderList.size() - 1, shaderPath));
	}

	//cleanup after char* conversion
	//delete[] shaderPathFrag;
	//delete[] shaderPathVert;

	return (int)shaderList.size() - 1;
}

int GameObject::doModelLoad(string modelPath) {
	
	//convert string to char*
	//char *modelPathCStr = new char[modelPath.length() + 1];
	//strcpy_s(modelPathCStr, sizeof modelPathCStr, modelPath.c_str());


	ThreeDModel* model = new ThreeDModel();
	if (objLoader.loadModel(modelPath.c_str(), *model))//returns true if the model is loaded, puts the model in the model parameter
	{
		cout << " model loaded: " << modelPath << endl;

		//if you want to translate the object to the origin of the screen,
		//first calculate the centre of the object, then move all the vertices
		//back so that the centre is on the origin.
		model->calcCentrePoint();
		model->centreOnZero();


		model->calcVertNormalsUsingOctree();  //the method will construct the octree if it hasn't already been created.


		//turn on VBO by setting useVBO to true in threeDmodel.cpp default constructor - only permitted on 8 series cards and higher
		model->initDrawElements();
		
		if (shaderIdx != -1) {
			model->initVBO(shaderList[shaderIdx]);
		}

		model->deleteVertexFaceData();

		modelList.push_back(model);
		GameObject::modelsLoaded.insert(pair<int, string>(modelList.size() - 1, modelPath));
		
	}
	else
	{
		cout << " model failed to load " << endl;
	}

	//cleanup after char* conversion
	//delete[] modelPathCStr; 

	return modelList.size() - 1;
}