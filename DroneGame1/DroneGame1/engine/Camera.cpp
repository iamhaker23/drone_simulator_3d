#include "Camera.h"

Camera::Camera() : GameObject(){
	
	this->farPlane = 200.0f;
	this->nearPlane = 1.f;
	this->fov = 50.f;
	this->aspect = 1.f;

	//update the aspect?
	this->updateOnResize = true;
	this->projectionMatrix = glm::perspective(fov, aspect, nearPlane, farPlane);

	this->targetPos = glm::vec3(0.f, 0.f, -10.f);
	this->up = glm::vec3(0.f, 1.f, 0.f);
	this->parent = NULL;
}

Camera::Camera(GLfloat fov, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane, bool updateOnResize, GameObject* parent) : GameObject() {
	
	this->farPlane = farPlane;
	this->nearPlane = nearPlane;
	this->fov = fov;
	this->aspect = aspect;
	this->updateOnResize = updateOnResize;

	this->projectionMatrix = glm::perspective(fov, aspect, nearPlane, farPlane);

	this->targetPos = glm::vec3(0.f, 0.f, -10.f);
	this->up = glm::vec3(0.f, 1.f, 0.f);
	this->parent = parent;
}

Camera::~Camera() {

}

Camera::Camera(const Camera &copy) : GameObject(copy) {

	this->projectionMatrix = copy.projectionMatrix;

	this->farPlane = copy.farPlane;
	this->nearPlane = copy.nearPlane;
	this->fov = copy.fov;
	this->aspect = copy.aspect;

	//update the aspect?
	this->updateOnResize = copy.updateOnResize;

	this->targetPos = copy.targetPos;
	this->up = copy.up;
	this->parent = copy.parent;
}

void Camera::resize(GLfloat width, GLfloat height) {
	//Set the projection matrix
	if (this->updateOnResize){
		this->aspect = (GLfloat)width / height;
		this->projectionMatrix = glm::perspective(fov, (GLfloat)width/height, nearPlane, farPlane);
	}
	
}

void Camera::updateCameraTransformation() {
	
	/*
	if (trackTarget) {
		trackTarget = false;

		glm::mat4 eyeSpaceTarget = glm::translate(glm::mat4(1), targetPos) * glm::inverse(modelViewMatrix);

		modelViewMatrix = glm::lookAt(glm::vec3(modelViewMatrix[3][0], modelViewMatrix[3][1], modelViewMatrix[3][2]), 
			glm::vec3(eyeSpaceTarget[3][0], eyeSpaceTarget[3][1], eyeSpaceTarget[3][2]), up);
	}
	*/

	GameObject::updateTransformation();
	
	
}

void Camera::addFovDelta(float fovDelta) {
	this->fov += fovDelta;
	if (this->fov < 1.f) this->fov = 1.f;
	if (this->fov >= 170.f) this->fov = 170.f;
	this->updateProjectionMatrix();
}
void Camera::updateProjectionMatrix() {
	this->projectionMatrix = glm::perspective(fov, aspect, nearPlane, farPlane);
}
