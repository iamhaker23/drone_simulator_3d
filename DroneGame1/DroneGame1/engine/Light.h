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
	float worldPosition[4] = {0.f, 0.f, 0.f, 1.f};
	
	inline Light() {

	}

	inline Light(glm::vec3 worldPosition, glm::vec4 color) {
		this->color[0] = color[0];
		this->color[1] = color[1];
		this->color[2] = color[2];
		this->color[3] = color[3];

		this->color[0] = color[0];
		this->color[1] = color[1];
		this->color[2] = color[2];
		this->color[3] = color[3];

		this->worldPosition[0] = worldPosition[0];
		this->worldPosition[1] = worldPosition[1];
		this->worldPosition[2] = worldPosition[2];
		this->worldPosition[3] = 0.f;
	}

	inline Light(const Light &copy) {
		this->color[0] = copy.color[0];
		this->color[1] = copy.color[1];
		this->color[2] = copy.color[2];
		this->color[3] = copy.color[3];

		this->color[0] = copy.color[0];
		this->color[1] = copy.color[1];
		this->color[2] = copy.color[2];
		this->color[3] = copy.color[3];

		this->worldPosition[0] = copy.worldPosition[0];
		this->worldPosition[1] = copy.worldPosition[1];
		this->worldPosition[2] = copy.worldPosition[2];
		this->worldPosition[3] = copy.worldPosition[3];
	}

	
};