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

class Camera {
private:
	float farPlane;
	float nearPlane;
	float fov;
	float aspect;
	bool updateOnResize;
	

public:

	bool trackTarget = false;
	glm::vec3 targetPos;
	glm::vec3 up;

	GameObject* parent;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;

	float camPosX = 0;
	float camPosY = 0;
	float camPosZ = 0;

	float camRotX = 0;
	float camRotY = 0;
	float camRotZ = 0;

	Camera();
	Camera(GLfloat fov, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane, bool updateOnResize, GameObject* drone);
	~Camera();
	Camera(const Camera &copy);
	
	void resize(GLfloat width, GLfloat height);
	void updateTransformation();
};
