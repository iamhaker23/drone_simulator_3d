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
float GameObject::time = 0.f;

//0 - sphere, 1 - box, 2 - octree, 3 - vertex
int GameObject::collisionType = 1;

vector<string> GameObject::collisionsResolved;
float GameObject::yAxisFloor = -29.0f;


float* GameObject::getLightPosition() {
	return new float[4]{ worldX, worldY, worldZ, 1.f };
}

GameObject* GameObject::makeLight(glm::vec3 localPosition, glm::vec4 color, float power) {
	GameObject* light = new GameObject();
	
	//light->inheritRotation = true;
	
	light->localX = localPosition[0];
	light->localY = localPosition[1];
	light->localZ = localPosition[2];

	light->myLight = new Light(color, power);
	return light;
}


GameObject::GameObject() {
	this->slowParentFactor = 1.0f;
	shaderIdx = -1;
	modelIdx = -1;

	worldPositionMatrix = glm::translate(glm::mat4(1.0), glm::vec3(worldX, worldY, worldZ));
	viewMatrix = glm::mat4();
	modelViewMatrix = glm::mat4();
	oldMV = glm::mat4();

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

void GameObject::doCollisionsAndApplyForces(bool doPhysics, vector<GameObject*> colliders) {
	if (physics == NULL) {
		return;
	}


	if (physics->createsLift) {
		float speedSqrd = (physics->forces[0] * physics->forces[0]) + (physics->forces[1] * physics->forces[1]) + (physics->forces[2] * physics->forces[2]);
		float lift = (5.f * physics->mass) * ((speedSqrd > 0.f) ? speedSqrd : 0.f);
		this->worldY += lift;
		//TODO: the lift should be the world-space vector projected onto local y-axis
		physics->forces[1] += lift;
	}

	if (physics->gravity) {
		float gravity = 0.1f * physics->mass;
		this->worldY -= gravity;
	}
	
	if (!physics->ghost && !physics->notMoveable && physics->collisions && doPhysics) {
		vector<glm::vec3> forcesFromCollisions = getForcesFromCollisions(colliders);
		physics->collisionForces += resolveForces(forcesFromCollisions);
		if (physics->collisionForces.x != 0 || physics->collisionForces.y != 0 || physics->collisionForces.z != 0) {
			//if cross product <= max && >= -max then ADD
			/*
			glm::vec3 cross = glm::cross(overrideForce, physics->forces);
			if (cross.x != 0 && cross.y != 0 && cross.z != 0) cross = glm::normalize(cross);
			float lengthSqrd = (cross.x*cross.x) + (cross.y*cross.y) + (cross.z*cross.z);
			physics->forces = (lengthSqrd <= 0.6f) ? overrideForce : physics->forces + overrideForce;
			*/
			//physics->forces = overrideForce;

			/*
			this->localX += ((physics->collisionForces[0] < 0.f) ? -1.f : 1.f) * std::sqrt(std::abs(physics->collisionForces[0]));
			this->localY += ((physics->collisionForces[1] < 0.f) ? -1.f : 1.f) * std::sqrt(std::abs(physics->collisionForces[1]));
			this->localZ += ((physics->collisionForces[2] < 0.f) ? -1.f : 1.f) * std::sqrt(std::abs(physics->collisionForces[2]));
			*/
			physics->forces += physics->collisionForces;
			physics->collisionForces = glm::vec3(0.f, 0.f, 0.f);
		}
	}

	if (physics->dynamic) {

		float xDelta = physics->forces[0] / (physics->mass*10.0f);
		float yDelta = physics->forces[1] / (physics->mass*10.0f);
		float zDelta = physics->forces[2] / (physics->mass*10.0f);
		
		this->localX += xDelta;
		this->localY += yDelta;
		this->localZ += zDelta;
		physics->oldForces = glm::vec3(xDelta, yDelta, zDelta);
		physics->forces -= physics->oldForces;
		
		//if (physics->forces.x < 0.1f &&physics->forces.x < 0.1f &&physics->forces.x < 0.1f) physics->forces = glm::vec3(0.f);

	}


	//physics->oldForces = glm::vec3(physics->forces);
	//physics->forces = glm::vec3(0.f, 0.f, 0.f);
}


vector<glm::vec3> GameObject::getHitPositions(GameObject* a, GameObject* b) {
	
	vector<glm::vec3> hits = vector<glm::vec3>();
		
	//if a will be affected by a collision
	//if (!a->physics->notMoveable || !b->physics->notMoveable){
	//if (a->name == "Drone1" && b->name == "Tardis") {
	if (a->physics->dynamic || b->physics->dynamic) {

		ThreeDModel* aModel = GameObject::modelList[a->modelIdx];
		ThreeDModel* bModel = GameObject::modelList[b->modelIdx];


		Octree* aOct = GameObject::modelList[a->modelIdx]->octree;
		Octree* bOct = GameObject::modelList[b->modelIdx]->octree;
			   
		//glm::vec3 aLargestExtent = a->scale * glm::vec3(aOct->box->verts[21], aOct->box->verts[22], aOct->box->verts[23]) - glm::vec3(aOct->box->verts[0], aOct->box->verts[1], aOct->box->verts[2]);
		//glm::vec3 bLargestExtent = b->scale *  glm::vec3(bOct->box->verts[21], bOct->box->verts[22], bOct->box->verts[23]) - glm::vec3(bOct->box->verts[0], bOct->box->verts[1], bOct->box->verts[2]);
		
		//float coverageSqr = ((aLargestExtent.x*aLargestExtent.x) + (aLargestExtent.x*aLargestExtent.x) + (aLargestExtent.x*aLargestExtent.x))
		//	+ ((bLargestExtent.x*bLargestExtent.x) + (bLargestExtent.x*bLargestExtent.x) + (bLargestExtent.x*bLargestExtent.x));
		glm::vec3 distVector = glm::vec3(a->worldX, a->worldY, a->worldZ) - glm::vec3(b->worldX, b->worldY, b->worldZ);
		float distSqr = (distVector.x*distVector.x) + (distVector.y*distVector.y) + (distVector.z*distVector.z);
		float coverageSqr = ((a->scale * aModel->boundingBox.getLargestExtent())*((a->scale * aModel->boundingBox.getLargestExtent()))) + ((b->scale * bModel->boundingBox.getLargestExtent())*(b->scale * bModel->boundingBox.getLargestExtent()));

		//Collision detection
		//cout << a->name << "---------->" << b->name << endl;

		if (distSqr < (coverageSqr/2.f)) {
			if (GameObject::collisionType == 0) {
				//sphere only collisions, and collision has been detected
				
				//ELASTIC BAND CODE
				//hits.push_back(glm::vec3(b->worldX - a->worldX, b->worldY - a->worldY, b->worldZ - a->worldZ));
				hits.push_back(glm::vec3(a->worldX - b->worldX, a->worldY - b->worldY, a->worldZ - b->worldZ));
				
			}
			else {
				/* Attempt for better "has moved" detection
				glm::mat4 aDelta = a->oldMV - a->worldPositionMatrix;
				glm::mat4 bDelta = b->oldMV - b->worldPositionMatrix;

				float deltaThreshold = 0.1f;

				if (aDelta[3][0] >= deltaThreshold || aDelta[3][1] >= deltaThreshold || aDelta[3][2] >= deltaThreshold
					|| bDelta[3][0] >= deltaThreshold || bDelta[3][1] >= deltaThreshold || bDelta[3][2] >= deltaThreshold){
					*/

				if (a->oldMV != a->worldPositionMatrix || b->oldMV != b->worldPositionMatrix) {

					/*
					BACKUP collision force
					//if (Collisions::doSAT(aModel, bModel, a->modelViewMatrix, b->modelViewMatrix, true) ) {
					if ( Collisions::doSAT(aModel, bModel, glm::scale(a->modelViewMatrix, glm::vec3(a->scale, a->scale, a->scale)), glm::scale(b->modelViewMatrix, glm::vec3(b->scale, b->scale, b->scale)),true)){
						hits.push_back(glm::vec3(a->worldX, a->worldY, a->worldZ));
					}
					else {
						//need to move to enable collisions again

						a->oldMV = a->worldPositionMatrix;
						b->oldMV = b->worldPositionMatrix;
					}
					*/

					//cout << "SAT:" << a->name << " to " << b->name << endl;
					
					
					//glm::vec3 hitPoint = Collisions::doSAT(GameObject::collisionType, aModel, bModel, glm::scale(glm::mat4(1.f), glm::vec3(a->scale, a->scale, a->scale))*a->modelViewMatrix, glm::scale(glm::mat4(1.f), glm::vec3(b->scale, b->scale, b->scale))*b->modelViewMatrix);
					//glm::vec3 hitPoint = Collisions::doSAT(GameObject::collisionType, aModel, bModel, a->modelViewMatrix, b->modelViewMatrix);

					//if (Collisions::doSAT(aModel, bModel, glm::scale(glm::mat4(1.0f), glm::vec3(a->scale, a->scale, a->scale))*a->modelViewMatrix, glm::scale(glm::mat4(1.0f), glm::vec3(b->scale, b->scale, b->scale))*b->modelViewMatrix)) {
					//if (Collisions::doSAT(aModel, bModel, glm::scale(a->modelViewMatrix, glm::vec3(a->scale, a->scale, a->scale)), glm::scale(b->modelViewMatrix, glm::vec3(b->scale, b->scale, b->scale)))) {
					//if (Collisions::doSAT(aModel, bModel, a->modelViewMatrix, b->modelViewMatrix)) {
					
					glm::vec3 hitPoint = Collisions::doSAT(
							GameObject::collisionType,
							aModel,
							bModel,
							glm::scale(glm::translate(a->modelViewMatrix, a->physics->forces), glm::vec3(a->scale, a->scale, a->scale)),
							glm::scale(glm::translate(b->modelViewMatrix, b->physics->forces), glm::vec3(b->scale, b->scale, b->scale)),
							//glm::scale(a->modelViewMatrix, glm::vec3(a->scale, a->scale, a->scale)),
							//glm::scale(b->modelViewMatrix, glm::vec3(b->scale, b->scale, b->scale)),
							a->scale,
							b->scale
						);

					if (hitPoint.x != 0.f || hitPoint.y != 0.f || hitPoint.z != 0.f) {

						//hits.push_back(glm::vec3(a->worldX, a->worldY, a->worldZ));
						hits.push_back(hitPoint);
					}
					else {
						//will need to move to enable collisions again
						
						a->oldMV = a->worldPositionMatrix;
						b->oldMV = b->worldPositionMatrix;
					}

				}
			}
		}
	}

	return hits;
}

vector<glm::vec3> GameObject::getForcesFromCollisions(vector<GameObject*> colliders) {
	vector<glm::vec3> forceList = {};
	if (physics == NULL) return forceList;

	ThreeDModel* myModel = GameObject::modelList[modelIdx];
	glm::vec3 myWorld = glm::vec3(worldX, worldY, worldZ);

	for (int i = 0; i < (int)colliders.size(); i++) {
		GameObject* other = colliders[i];

		if (other->name != name && other->physics != NULL) {

			//quick check if the collision is already resolved
			bool resolved = false;
			/*
			for (int collRes = 0; collRes < (int)GameObject::collisionsResolved.size(); collRes++) {
				if (GameObject::collisionsResolved[collRes] == (other->name + name)) {
					//cout << "COLLISION CULL" << GameObject::collisionsResolved[collRes]  << endl;
					resolved = true;
					break;
				}
			}
			*/
			
			if (!resolved) {

				collisionsResolved.push_back(name + other->name);

				ThreeDModel* otherModel = GameObject::modelList[other->modelIdx];
				glm::vec3 otherWorld = glm::vec3(other->worldX, other->worldY, other->worldZ);// *modelViewMatrix;

				//float direction = (other->extent > this->extent) ? -1.f : 1.f;
				//vector<glm::vec3> hitPositions = (direction == 1.f) ? GameObject::getHitPositions(this, other) : GameObject::getHitPositions(other, this);
				vector<glm::vec3> hitPositions = GameObject::getHitPositions(this, other);

				if ((int)hitPositions.size() > 0) {
					
					//the good one 
					//glm::vec3 collisionForce = glm::vec3((myWorld.x - otherWorld.x), (myWorld.y - otherWorld.y), (myWorld.z - otherWorld.z));
					
					//glm::vec3 collisionForce = glm::vec3((hitPositions[0].x - otherWorld.x), (hitPositions[0].y - otherWorld.y), (hitPositions[0].z - otherWorld.z));
					glm::vec3 collisionForce = (GameObject::collisionType != 1) ? hitPositions[0] :
						glm::vec3((myWorld.x - otherWorld.x), (myWorld.y - otherWorld.y), (myWorld.z - otherWorld.z));
					//glm::vec3 collisionForce = glm::vec3((myWorld.x - hitPositions[0].x), (myWorld.y - hitPositions[0].y), (myWorld.z - hitPositions[0].z));
					
					if (collisionForce.x != 0.f || collisionForce.y != 0.f || collisionForce.z != 0.f) {
						//cout << name << "COLLISION" << endl;
						forceList.push_back(glm::normalize(collisionForce));
						//temporary way of applying force to other
						other->physics->collisionForces -= glm::normalize(collisionForce);
					
					//if (hitPositions[0].x != 0.f || hitPositions[0].y != 0.f || hitPositions[0].z != 0.f) {
						//forceList.push_back(glm::normalize(-hitPositions[0]));
					}
				}
			}

		}

	}

	return forceList;
}

void GameObject::debugDraw(float vertices[], int vertexCount, int tris[], int numOfTris, bool init) {
		
	if (debugEnabled && GameObject::debugShader != -1) {
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
	oldMV = glm::mat4();

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

	Octree* myOct = GameObject::modelList[modelIdx]->octree;
	//glm::vec3 myDiagonalExtent = (glm::vec3(myOct->box->verts[21], myOct->box->verts[22], myOct->box->verts[23]) - glm::vec3(myOct->box->verts[0], myOct->box->verts[1], myOct->box->verts[2]));
	//this->extent = glm::distance(myDiagonalExtent, glm::vec3(0));

	this->extent = GameObject::modelList[GameObject::modelIdx]->boundingBox.boxWidthY;
	
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
	this->oldMV = copy.oldMV;
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
	
	//hack for local rotation
	bool doLocalRotation = false;
	glm::mat4 localRotation = glm::mat4();

	if (parent != NULL && inheritRotation) {
		//TODO: slow parent rotations
		//worldRotation *= getRotationMatrix(parent->spinXinc*-slowParentFactor, parent->spinYinc*-slowParentFactor, parent->spinZinc*-slowParentFactor);
		//worldRotation = parent->worldRotation *  getRotationMatrix(spinXinc, spinYinc, spinZinc);
		worldRotation = parent->worldRotation;
		localRotation = getRotationMatrix(spinXinc, spinYinc, spinZinc);
		doLocalRotation = true;

	}
	else {
		//cumulative rotation
		worldRotation *= getRotationMatrix(spinXinc, spinYinc, spinZinc);

	}


	//apply local transformation
	glm::mat4 localPosition = glm::translate(glm::mat4(1), glm::vec3(localX, localY, localZ));
	//reset local space co-ordinates if this is root
	//BECAUSE local co-ordinates are converted to a world space translation and applied to world space co-ordinates
	//So if this object is the root then it will be at origin of it's local space
	//OTHERWISE it's local space co-ordinates must be preserved to be applied in parent-space
	if (parent == NULL) {
		
		localX = 0;
		localY = 0;
		localZ = 0;
	}
	else {
		if (parent->physics != NULL) localPosition = localPosition * glm::translate(glm::mat4(1), glm::vec3(parent->physics->oldForces.x, parent->physics->oldForces.y, parent->physics->oldForces.z));
	}

	//update ViewModel
	//if (parent != NULL) parent->updateTransformation();
	worldPositionMatrix = (parent != NULL) ? parent->worldPositionMatrix : glm::translate(glm::mat4(1), glm::vec3(worldX, worldY, worldZ));

	//apply rotation and translations to modelViewMatrix
	if (doLocalRotation) {
		modelViewMatrix = worldPositionMatrix * (worldRotation) * (localPosition) * localRotation;
	}
	else {
		modelViewMatrix = worldPositionMatrix * (worldRotation) * (localPosition);
	}

	//update worldPosition (localtransform is accumulated into worldPosition modelViewMatrix)
	worldX = modelViewMatrix[3][0];
	worldY = modelViewMatrix[3][1];
	worldZ = modelViewMatrix[3][2];

	if (physics != NULL && (!physics->notMoveable) && worldY <= GameObject::yAxisFloor+ (this->extent / 2.f)) worldY = GameObject::yAxisFloor + (this->extent/ 2.f);
	/*
	if (physics != NULL && (!physics->notMoveable) && worldY <= GameObject::yAxisFloor) {
		//TODO: remove hack to account for person extent
		if (name != "Person") {
			worldY = GameObject::yAxisFloor + 4.0f;
		}
		else {
			worldY = GameObject::yAxisFloor;
		}
	}
	*/
}


void GameObject::draw(glm::mat4 projectionMatrix, glm::mat4 camViewMatrix) {

	glMatrixMode(GL_MODELVIEW);
	
	//lights which are drawn will be updated in the light loop
	if (myLight == NULL) updateTransformation();
	
	Shader* shaderHandle = shaderList[shaderIdx];
	ThreeDModel* modelHandle = modelList[modelIdx];

	glUseProgram(shaderHandle->handle());  // use the shader


	GLuint matLocation = glGetUniformLocation(shaderHandle->handle(), "ProjectionMatrix");
	glUniformMatrix4fv(matLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

	GLuint scaleLocation = glGetUniformLocation(shaderHandle->handle(), "scale");
	glUniform1f(scaleLocation, scale);

	if (numLights > 0) {
		glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "LightPosA"), 1, activeLights[0]->getLightPosition());

		glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "light_ambienta"), 1, activeLights[0]->myLight->color);
		glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "light_diffusea"), 1, activeLights[0]->myLight->color);
		glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "light_speculara"), 1, activeLights[0]->myLight->specular);
		glUniform1f(glGetUniformLocation(shaderHandle->handle(), "light_powera"), activeLights[0]->myLight->power);

	}
	if (numLights > 1) {
		glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "LightPosB"), 1, activeLights[1]->getLightPosition());

		glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "light_ambientb"), 1, activeLights[1]->myLight->color);
		glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "light_diffuseb"), 1, activeLights[1]->myLight->color);
		glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "light_specularb"), 1, activeLights[1]->myLight->specular);
		glUniform1f(glGetUniformLocation(shaderHandle->handle(), "light_powerb"), activeLights[1]->myLight->power);
	}
	if (numLights > 2) {
		glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "LightPosC"), 1, activeLights[2]->getLightPosition());

		glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "light_ambientc"), 1, activeLights[2]->myLight->color);
		glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "light_diffusec"), 1, activeLights[2]->myLight->color);
		glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "light_specularc"), 1, activeLights[2]->myLight->specular);
		glUniform1f(glGetUniformLocation(shaderHandle->handle(), "light_powerc"), activeLights[2]->myLight->power);
	}
	if (numLights > 3) {
		glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "LightPosD"), 1, activeLights[3]->getLightPosition());

		glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "light_ambientd"), 1, activeLights[3]->myLight->color);
		glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "light_diffused"), 1, activeLights[3]->myLight->color);
		glUniform4fv(glGetUniformLocation(shaderHandle->handle(), "light_speculard"), 1, activeLights[3]->myLight->specular);
		glUniform1f(glGetUniformLocation(shaderHandle->handle(), "light_powerd"), activeLights[3]->myLight->power);
	}
	glUniform1i(glGetUniformLocation(shaderHandle->handle(), "numLights"), numLights);
	glUniform1f(glGetUniformLocation(shaderHandle->handle(), "alphaClip"), material->alphaClipThreshold);
	glUniform2fv(glGetUniformLocation(shaderHandle->handle(), "uvOffset"), 1, material->uvOffset);

	//TODO: more robust way of binding variables for shaders
	if (shaderHandle->m_name == "Assets/glslfiles/water") {
		glUniform1f(glGetUniformLocation(shaderHandle->handle(), "time"), GameObject::time);
	}
	
	glUniform1i(glGetUniformLocation(shaderHandle->handle(), "shadeless"), material->shadeless);
	
	
	glUniform1i(glGetUniformLocation(shaderHandle->handle(), "normalMapping"), material->normalMapping);

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

		//TODO: triangle collision detection (mode 3) is not accurate, 
		//this line keeps necessary model data if you use (mode 3) regardless
		if (GameObject::collisionType != 3) {
			model->deleteVertexFaceData();
		}

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
