#include "Camera.h"

Camera::Camera() : GameObject(){
	
	this->farPlane = 200.0f;
	this->nearPlane = 1.f;
	this->fov = 50.f; 
	this->originalFov = 50.f;
	this->aspect = 1.f;

	//update the aspect?
	this->updateOnResize = true;
	this->projectionMatrix = glm::perspective(fov, aspect, nearPlane, farPlane);

	this->target = NULL;
	this->up = glm::vec3(0.f, 1.f, 0.f);
	this->parent = NULL;
}

Camera::Camera(GLfloat fov, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane, bool updateOnResize, GameObject* parent) : GameObject() {
	
	this->farPlane = farPlane;
	this->nearPlane = nearPlane;
	this->originalFov = fov;
	this->fov = fov;
	this->aspect = aspect;
	this->updateOnResize = updateOnResize;

	this->projectionMatrix = glm::perspective(fov, aspect, nearPlane, farPlane);

	this->target = NULL;
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
	this->originalFov = copy.originalFov;
	this->aspect = copy.aspect;

	//update the aspect?
	this->updateOnResize = copy.updateOnResize;

	this->target = copy.target;
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

	GameObject::updateTransformation();

	if (trackTarget && target != NULL) {
		
		glm::vec3 targetPos = glm::vec3(target->modelViewMatrix[3][0], target->modelViewMatrix[3][1], target->modelViewMatrix[3][2]);
		modelViewMatrix = glm::inverse(glm::lookAt(glm::vec3(modelViewMatrix[3][0], modelViewMatrix[3][1], modelViewMatrix[3][2]), targetPos, up));
	}
			

}

void Camera::updateProjectionMatrix() {
	this->projectionMatrix = glm::perspective(fov, aspect, nearPlane, farPlane);
}
