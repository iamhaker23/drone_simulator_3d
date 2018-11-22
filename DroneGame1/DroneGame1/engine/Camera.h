#pragma once

#include "gl/glew.h"
#include "gl/wglew.h"
#pragma comment(lib, "glew32.lib")
#include "shaders/Shader.h"   // include shader header file, this is not part of OpenGL
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include "GameObject.h"

class Camera : public GameObject {
private:
	float farPlane;
	float nearPlane;
	float aspect;
	bool updateOnResize;
	

public:

	bool trackTarget = false;
	GameObject* target;
	glm::vec3 up;
	float fov;

	glm::mat4 projectionMatrix;

	Camera();
	Camera(GLfloat fov, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane, bool updateOnResize, GameObject* drone);
	~Camera();
	Camera(const Camera &copy);
	
	void resize(GLfloat width, GLfloat height);
	void updateCameraTransformation();
	void addFovDelta(float fovDelta);
	void updateProjectionMatrix();
};
