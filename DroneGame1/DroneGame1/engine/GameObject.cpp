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
map<int, string> GameObject::shadersLoaded = {};
map<int, string> GameObject::modelsLoaded = {};
int GameObject::debugShader = -1;

GameObject::GameObject() {
	this->slowParentFactor = 1.0f;
	shaderIdx = -1;
	modelIdx = -1;

	worldPositionMatrix = glm::translate(glm::mat4(1.0), glm::vec3(worldX, worldY, worldZ));
	viewMatrix = glm::mat4();
	modelViewMatrix = glm::mat4();
	projectionMatrix = glm::mat4();

	parent = NULL;
	this->physics = NULL;
	this->material = new Material();

	if (GameObject::debugShader == -1) {
		GameObject::debugShader = loadShader("Assets/glslfiles/basic");
	}

}

void GameObject::addForce(float x, float y, float z) {
	if (physics == NULL) {
		return;
	}

	physics->forces += glm::vec3(x, y, z);
}

void GameObject::doCollisionsAndApplyForces(vector<GameObject*> colliders) {
	if (physics == NULL) {
		return;
	}

	if (!physics->ghost) {
		vector<glm::vec3> forcesFromCollisions = getForcesFromCollisions(colliders);
		physics->forces += resolveForces(forcesFromCollisions);
	}

	this->localX += physics->forces[0];
	this->localY += physics->forces[1];
	this->localZ += physics->forces[2];

	if (physics->createsLift) {
		float speedSqrd = (physics->forces[0] * physics->forces[0]) + (physics->forces[2] * physics->forces[2]) + (physics->forces[1] * physics->forces[1]);
		float lift = (5.f * physics->mass) * ((speedSqrd > 0.f) ? speedSqrd : 0.f);
		this->worldY += lift;
		//TODO: the lift should be the world-space vector projected onto local y-axis
		physics->forces[1] += lift;
	}

	if (physics->dynamic) {
		float gravity = 0.1f * physics->mass;
		this->worldY -= gravity;
		//TODO: the gravity should be the world-space vector projected onto local y-axis
		physics->forces[1] -= gravity;
	}

	physics->oldForces = glm::vec3(physics->forces);
	physics->forces = glm::vec3(0.f, 0.f, 0.f);
}


vector<glm::vec3> GameObject::getForcesFromCollisions(vector<GameObject*> colliders) {
	vector<glm::vec3> forceList = {};

	ThreeDModel* myModel = GameObject::modelList[modelIdx];
	//glm::vec4 myWorld = glm::vec4(worldX, worldY, worldZ, 1.f);
	glm::vec3 myWorld = glm::vec3(worldX, worldY, worldZ);

	for (int i = 0; i < (int)colliders.size(); i++) {
		GameObject* other = colliders[i];
		if (other->name != name) {
			ThreeDModel* otherModel = GameObject::modelList[other->modelIdx];
			//glm::vec4 otherWorld = glm::vec4(other->worldX, other->worldY, other->worldZ, 1.f);// *modelViewMatrix;
			glm::vec3 otherWorld = glm::vec3(other->worldX, other->worldY, other->worldZ);// *modelViewMatrix;

			/*float verts[] = { 0.f, 0.f, 0.f,
						myWorld.x,myWorld.y,myWorld.z,
						otherWorld.x,otherWorld.y,otherWorld.z };
			int triIdx[] = { 0, 1, 2 };
			debugDraw(verts, 3, triIdx, 1, tmpVaoId == 0);
			*/

			float diff = ((myWorld.x - otherWorld.x)*(myWorld.x - otherWorld.x)) +
				((myWorld.y - otherWorld.y)*(myWorld.y - otherWorld.y)) +
				((myWorld.z - otherWorld.z)*(myWorld.z - otherWorld.z));

			if (diff < (radius+other->radius)*(radius + other->radius) ){//(r+r)^2 
				glm::vec3 collisionForce = glm::vec3((myWorld.x - otherWorld.x), (myWorld.y - otherWorld.y), (myWorld.z - otherWorld.z));
				
				//TODO: normalize cannot take distance zero
				//forceList.push_back(0.1f * glm::normalize(collisionForce));
			}

			//projection of edge is dot(edge, axis)
			//if projections overlap 

		}
	}

	return forceList;
}

void GameObject::debugDraw(float vertices[], int vertexCount, int tris[], int numOfTris, bool init) {
		
	if (GameObject::debugShader != -1) {
		Shader* debugShaderHandle = GameObject::shaderList[GameObject::debugShader];
		glUseProgram(debugShaderHandle->handle());  // use the shader
		//glUniformMatrix4fv(glGetUniformLocation(debugShaderHandle->handle(), "ProjectionMatrix"), 1, GL_FALSE, &(projectionMatrix)[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(debugShaderHandle->handle(), "ModelViewMatrix"), 1, GL_FALSE, &((viewMatrix*modelViewMatrix))[0][0]);
		//glUniformMatrix4fv(glGetUniformLocation(debugShaderHandle->handle(), "ModelViewMatrix"), 1, GL_FALSE, &glm::mat4(1.f)[0][0]);

		if (init) {
			// VAO allocation
			glGenVertexArrays(1, &tmpVaoId);

			// First VAO setup
			glBindVertexArray(tmpVaoId);

			glGenBuffers(1, tmpVboId);



			glBindBuffer(GL_ARRAY_BUFFER, tmpVboId[0]);
			//initialises data storage of vertex buffer object
			glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
			GLint vertexLocation = glGetAttribLocation(debugShaderHandle->handle(), "in_Position");
			glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(vertexLocation);

			glGenBuffers(1, &tmpIbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmpIbo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfTris * 3 * sizeof(unsigned int), tris, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glEnableVertexAttribArray(0);
		}
		else {
			glBindBuffer(GL_ARRAY_BUFFER, tmpVboId[0]);
			//initialises data storage of vertex buffer object
			glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); 
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmpIbo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, numOfTris * 3 * sizeof(unsigned int), tris, GL_STATIC_DRAW);
		}

		glDisable(GL_CULL_FACE);

		glBindVertexArray(tmpVaoId);
		//ibo - indexes of which vertices are in which faces 
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmpIbo);
		glDrawElements(GL_TRIANGLES, numOfTris * 3, GL_UNSIGNED_INT, 0);

		// Done

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	}
}

glm::vec3 GameObject::resolveForces(vector<glm::vec3> forcesFromCollisions) {
	glm::vec3 forces = glm::vec3(0);

	for (int i = 0; i < (int)forcesFromCollisions.size(); i++) {
		
		glm::vec4 force = glm::vec4(forcesFromCollisions[i].x, forcesFromCollisions[i].x, forcesFromCollisions[i].z, 1.0f);

		force = force * (modelViewMatrix);
		forces.x += force.x;
		forces.y += force.y;
		forces.z += force.z;
	}

	return forces;
}

GameObject::GameObject(string name, string modelPath, string shaderPath, bool inheritRotation) {
	this->slowParentFactor = 1.0f;
	this->inheritRotation = inheritRotation;
	this->parent = NULL;
	this->physics = NULL;
	this->material = new Material();

	this->name = name;

	worldPositionMatrix = glm::translate(glm::mat4(1.0), glm::vec3(worldX, worldY, worldZ));
	viewMatrix = glm::mat4();
	modelViewMatrix = glm::mat4();
	projectionMatrix = glm::mat4();

	if (GameObject::debugShader == -1) {
		GameObject::debugShader = loadShader("Assets/glslfiles/basic");
	}

	shaderIdx = GameObject::loadShader(shaderPath);
	
	modelIdx = -1;
	if (modelsLoaded.size() > 0) {
		for (const auto& loadedModel : modelsLoaded) {
			if (loadedModel.second == modelPath) {
				modelIdx = loadedModel.first;
			}
		}
	}
	if (modelIdx == -1) {
		modelIdx = doModelLoad(modelPath, shaderIdx);
	}
	

}

int GameObject::loadShader(string shaderPath) {
	int shaderIdx = -1;

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
	return shaderIdx;
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
	this->viewMatrix = copy.viewMatrix;
	this->projectionMatrix = copy.projectionMatrix;
	this->worldPositionMatrix = copy.worldPositionMatrix;

	this->shaderIdx = copy.shaderIdx;
	this->modelIdx = copy.modelIdx;

	this->name = copy.name + "_COPY";

	this->parent = copy.parent;
	this->physics = copy.physics;
	this->slowParentFactor = copy.slowParentFactor;
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
		//TODO: slow parent rotations
		//worldRotation *= getRotationMatrix(parent->spinXinc*-slowParentFactor, parent->spinYinc*-slowParentFactor, parent->spinZinc*-slowParentFactor);
		worldRotation = parent->worldRotation;
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
	//if (parent != NULL) parent->updateTransformation();
	worldPositionMatrix = (parent != NULL) ? parent->worldPositionMatrix : glm::translate(glm::mat4(1), glm::vec3(worldX, worldY, worldZ));

	//apply rotation and translations to modelViewMatrix
	modelViewMatrix = worldPositionMatrix * (worldRotation) * localPosition;

	//update worldPosition (localtransform is accumulated into worldPosition modelViewMatrix)
	worldX = modelViewMatrix[3][0];
	worldY = modelViewMatrix[3][1];
	worldZ = modelViewMatrix[3][2];

	if (worldY <= -30.f) worldY = -30.f;
	
}

void GameObject::draw(glm::mat4 projectionMatrix, glm::mat4 camViewMatrix) {

	glMatrixMode(GL_MODELVIEW);
	
	updateTransformation();
	
	Shader* shaderHandle = shaderList[shaderIdx];
	ThreeDModel* modelHandle = modelList[modelIdx];

	glUseProgram(shaderHandle->handle());  // use the shader


	GLuint matLocation = glGetUniformLocation(shaderHandle->handle(), "ProjectionMatrix");
	glUniformMatrix4fv(matLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

	GLuint scaleLocation = glGetUniformLocation(shaderHandle->handle(), "scale");
	glUniform1f(scaleLocation, scale);
	
	glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "LightPos"), 1, activeLights[0]->worldPosition);
	glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "light_ambient"), 1, activeLights[0]->color);
	glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "light_diffuse"), 1, activeLights[0]->color);
	glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "light_specular"), 1, activeLights[0]->specular);

	glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "material_ambient"), 1, material->ambient);
	glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "material_diffuse"), 1, material->diffuse);
	glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "material_specular"), 1, material->specular);
	glUniform1f(glGetUniformLocation(shaderHandle->handle(), "material_shininess"), material->shininess);
	
	viewMatrix = camViewMatrix;
	projectionMatrix = projectionMatrix;
	glm::mat4 eyeSpace = (camViewMatrix) * modelViewMatrix;

	glUniformMatrix4fv(glGetUniformLocation(shaderHandle->handle(), "ViewMatrix"), 1, GL_FALSE, &camViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderHandle->handle(), "ModelViewMatrix"), 1, GL_FALSE, &eyeSpace[0][0]);

	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(modelViewMatrix));
	glUniformMatrix3fv(glGetUniformLocation(shaderHandle->handle(), "NormalMatrix"), 1, GL_FALSE, &normalMatrix[0][0]);

	modelHandle->drawElementsUsingVBO(shaderHandle);
	if (GameObject::debugShader != -1) {

		Shader* debugShader = GameObject::shaderList[GameObject::debugShader];

		glUseProgram(debugShader->handle());  // use the shader


		glUniformMatrix4fv(glGetUniformLocation(debugShader->handle(), "ModelViewMatrix"), 1, GL_FALSE, &eyeSpace[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(debugShader->handle(), "ProjectionMatrix"), 1, GL_FALSE, &projectionMatrix[0][0]);
		GLuint scaleLocation = glGetUniformLocation(debugShader->handle(), "scale");
		glUniform1f(scaleLocation, scale);

		if (drawBounds) modelHandle->drawBoundingBox(debugShader);
		if (drawOctree) modelHandle->drawOctreeLeaves(debugShader);
	}
}

int GameObject::doShaderLoad(string shaderPath) {
	Shader* shader = new Shader();


	//convert string to char*
	string shaderPathFragStr = shaderPath + ".frag";
	string shaderPathVertStr = shaderPath + ".vert";
	
	if (!shader->load(shaderPath, shaderPathVertStr.c_str(), shaderPathFragStr.c_str()))
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

int GameObject::doModelLoad(string modelPath, int shaderIdx) {
	
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
