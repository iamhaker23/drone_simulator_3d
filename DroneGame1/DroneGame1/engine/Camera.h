#pragma once

#include "gl/glew.h"
#include "gl/wglew.h"
#pragma comment(lib, "glew32.lib")
#include "shaders/Shader.h"   // include shader header file, this is not part of OpenGL
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_inverse.hpp"

class Camera {
private:
	float farPlane;
	float nearPlane;
	float fov;
	float aspect;
	bool updateOnResize;


public:
	glm::mat4 modelViewMatrix;
	glm::mat4 projectionMatrix;

	Camera();
	Camera(GLfloat fov, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane, bool updateOnResize);
	~Camera();
	Camera(const Camera &copy);
	void resize(GLfloat newAspect);

};
