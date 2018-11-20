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

}

GameObject::GameObject(string name, string modelPath, string shaderPath) {

	this->name = name;
	shaderIdx = -1;
	modelIdx = -1;
	
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

}



void GameObject::updateTransformation() {
	glm::mat4 matrixX, matrixXY;

	//rotation about the local x axis
	q = glm::angleAxis(spinXinc, glm::vec3(objectRotation[0][0], objectRotation[0][1], objectRotation[0][2]));
	matrixX = glm::mat4_cast(q) * objectRotation;

	//EXAMPLE FOR ACCESSING USING A 1D array
	const float *pSource = (const float*)glm::value_ptr(matrixX);
	//rotation about the local y axis
	q = glm::angleAxis(spinYinc, glm::vec3(pSource[4], pSource[5], pSource[6]));
	matrixXY = glm::mat4_cast(q) * matrixX;

	//EXAMPLE ACCESSING WITH 2D GLM structure.
	//rotation about the local z axis
	q = glm::angleAxis(spinZinc, glm::vec3(matrixXY[2][0], matrixXY[2][1], matrixXY[2][2]));
	objectRotation = glm::mat4_cast(q) * matrixXY;
}

void GameObject::draw(glm::mat4 projectionMatrix) {

	glMatrixMode(GL_MODELVIEW);

	Shader* shaderHandle = shaderList[shaderIdx];
	ThreeDModel* modelHandle = modelList[modelIdx];

	glUseProgram(shaderHandle->handle());  // use the shader

	GLuint matLocation = glGetUniformLocation(shaderHandle->handle(), "ProjectionMatrix");
	glUniformMatrix4fv(matLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

	glm::mat4 viewingMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0, 0, -50));
	glUniformMatrix4fv(glGetUniformLocation(shaderHandle->handle(), "ViewMatrix"), 1, GL_FALSE, &viewingMatrix[0][0]);

	glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "LightPos"), 1, LightPos);
	glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "light_ambient"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "light_diffuse"), 1, Light_Ambient_And_Diffuse);
	glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "light_specular"), 1, Light_Specular);

	glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "material_ambient"), 1, Material_Ambient);
	glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "material_diffuse"), 1, Material_Diffuse);
	glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "material_specular"), 1, Material_Specular);
	glUniform1f(glGetUniformLocation(shaderHandle->handle(), "material_shininess"), Material_Shininess);


	//DRAW THE MODEL
	modelViewMatrix = viewingMatrix * objectRotation;

	glUniformMatrix4fv(glGetUniformLocation(shaderHandle->handle(), "ModelViewMatrix"), 1, GL_FALSE, &modelViewMatrix[0][0]);


	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(modelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(shaderHandle->handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

	modelHandle->drawElementsUsingVBO(shaderHandle);
}

int GameObject::doShaderLoad(string shaderPath) {
	Shader* shader = new Shader();


	//convert string to char*
	string shaderPathFragStr = shaderPath + ".frag";
	string shaderPathVertStr = shaderPath + ".vert";

	//TODO: why strcpy_s no work!?

	//char* shaderPathFrag = new char[shaderPathFragStr.length() + 1];
	//strcpy_s(shaderPathFrag, sizeof shaderPathFrag, shaderPathFragStr.c_str());
	
	//char* shaderPathVert = new char[shaderPathVertStr.length() + 1];
	//strcpy_s(shaderPathVert, sizeof shaderPathVert, shaderPathVertStr.c_str());

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