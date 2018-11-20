#include "Camera.h"

Camera::Camera() {
	
	this->farPlane = 200.0f;
	this->nearPlane = 1.f;
	this->fov = 50.f;
	this->aspect = 1.f;

	//update the aspect?
	this->updateOnResize = true;
	this->projectionMatrix = glm::perspective(fov, aspect, nearPlane, farPlane);
	this->modelViewMatrix = glm::mat4();

}

Camera::Camera(GLfloat fov, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane, bool updateOnResize) {
	
	this->farPlane = farPlane;
	this->nearPlane = nearPlane;
	this->fov = fov;
	this->aspect = aspect;
	this->updateOnResize = updateOnResize;

	this->modelViewMatrix = glm::mat4();
	this->projectionMatrix = glm::perspective(fov, aspect, nearPlane, farPlane);
}

Camera::~Camera() {

}
Camera::Camera(const Camera &copy) {
	this->modelViewMatrix = copy.modelViewMatrix;
	this->projectionMatrix = copy.projectionMatrix;

	this->farPlane = copy.farPlane;
	this->nearPlane = copy.nearPlane;
	this->fov = copy.fov;
	this->aspect = copy.aspect;

	//update the aspect?
	this->updateOnResize = copy.updateOnResize;

}

void Camera::resize(GLfloat width, GLfloat height) {
	//Set the projection matrix
	if (this->updateOnResize){

		this->projectionMatrix = glm::perspective(fov, (GLfloat)width/height, nearPlane, farPlane);
	}
	
}
