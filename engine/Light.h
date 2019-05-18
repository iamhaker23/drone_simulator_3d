#pragma once

#include "gl/glew.h"
#include "gl/wglew.h"
#pragma comment(lib, "glew32.lib")
#include "shaders/Shader.h"   // include shader header file, this is not part of OpenGL
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_inverse.hpp"

class Light {
public:
	float color[4] = {.5f, .5f, .5f, .5f};
	float specular[4] = {.5f, .5f, .5f, .5f};
	float power = 1.f;
	bool selfLight = false;
	
	inline Light() {

	}

	inline Light(glm::vec4 color, float power) {
		this->power = power;

		this->color[0] = color[0];
		this->color[1] = color[1];
		this->color[2] = color[2];
		this->color[3] = color[3];

		this->specular[0] = color[0];
		this->specular[1] = color[1];
		this->specular[2] = color[2];
		this->specular[3] = color[3];

	}

	inline Light(const Light &copy) {
		this->power = copy.power;

		this->color[0] = copy.color[0];
		this->color[1] = copy.color[1];
		this->color[2] = copy.color[2];
		this->color[3] = copy.color[3];

		this->specular[0] = copy.specular[0];
		this->specular[1] = copy.specular[1];
		this->specular[2] = copy.specular[2];
		this->specular[3] = copy.specular[3];

	}

	
};