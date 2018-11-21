#include "Camera.h"

Camera::Camera() {
	
	this->farPlane = 200.0f;
	this->nearPlane = 1.f;
	this->fov = 50.f;
	this->aspect = 1.f;

	//update the aspect?
	this->updateOnResize = true;
	this->projectionMatrix = glm::perspective(fov, aspect, nearPlane, farPlane);
	this->viewMatrix = glm::mat4();

	this->targetPos = glm::vec3(0.f, 0.f, 1.f);
	this->up = glm::vec3(0.f, 1.f, 0.f);
	this->parent = NULL;
}

Camera::Camera(GLfloat fov, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane, bool updateOnResize, GameObject* parent) {
	
	this->farPlane = farPlane;
	this->nearPlane = nearPlane;
	this->fov = fov;
	this->aspect = aspect;
	this->updateOnResize = updateOnResize;

	this->viewMatrix = glm::mat4();
	this->projectionMatrix = glm::perspective(fov, aspect, nearPlane, farPlane);

	this->targetPos = glm::vec3(0.f, 0.f, 1.f);
	this->up = glm::vec3(0.f, 1.f, 0.f);
	this->parent = parent;
}

Camera::~Camera() {

}
Camera::Camera(const Camera &copy) {
	this->viewMatrix = copy.viewMatrix;
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

void Camera::updateTransformation() {
	
	glm::vec3 eyePos = glm::vec3(camPosX, camPosY, camPosZ);

	viewMatrix = glm::translate(glm::mat4(1), eyePos);

	if (trackTarget && parent != NULL) {
		
		//TODO: cameras are child of drone
		//-1. Gameobject with child
		//0. Camera extends gameobject?
		//1. camera transform
		//2. camera parent
	}

	if (trackTarget) {
		trackTarget = false;
		//viewMatrix = glm::lookAt(eyePos, targetPos, up);
	}
	else {
		glm::mat4 matrixX, matrixXY;
		glm::quat q;

		//rotation about the local x axis
		q = glm::angleAxis(camRotX, glm::vec3(1, 0, 0));
		matrixX = glm::mat4_cast(q);

		//rotation about the local y axis
		q = glm::angleAxis(camRotY, glm::vec3(matrixX[1][0], matrixX[1][1], matrixX[1][2]));
		matrixXY = glm::mat4_cast(q) * matrixX;

		//EXAMPLE ACCESSING WITH 2D GLM structure.
		//rotation about the local z axis
		q = glm::angleAxis(camRotZ, glm::vec3(matrixXY[2][0], matrixXY[2][1], matrixXY[2][2]));

		//matrixXYZ
		viewMatrix = glm::mat4_cast(q) * matrixXY * viewMatrix;
	}

	camPosX = 0.f;
	camPosY = 0.f;
	camPosZ = 0.f;

	camRotX = 0.f;
	camRotY = 0.f;
	camRotZ = 0.f;
}

void Camera::resize(GLfloat width, GLfloat height) {
	//Set the projection matrix
	if (this->updateOnResize){

		this->projectionMatrix = glm::perspective(fov, (GLfloat)width/height, nearPlane, farPlane);
	}
	
}
