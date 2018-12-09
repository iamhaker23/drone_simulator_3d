#pragma once

#include "gl/glew.h"
#include "gl/wglew.h"
#pragma comment(lib, "glew32.lib")
#include "shaders/Shader.h"   // include shader header file, this is not part of OpenGL
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_inverse.hpp"

class Physics {
public:

	float mass;
	float frictionCoefficient;
	float collisionMargin;
	bool ghost;
	bool dynamic;
	bool gravity;
	glm::vec3 forces;
	glm::vec3 oldForces;
	glm::vec3 collisionForces;
	bool createsLift;

	inline Physics() {
		this->forces = glm::vec3(0.f, 0.f, 0.f);
		this->mass = 1.0f;
		this->frictionCoefficient = 1.0f;
		this->collisionMargin = 0.1f;
		this->ghost = false;
		this->dynamic = false;
		this->gravity = false;
		this->createsLift = false;
	}

	inline Physics(float mass, float fric, float collMargin, bool ghost, bool dynamic, bool createsLift) {
		
		this->forces = glm::vec3(0.f, 0.f, 0.f);
		this->mass = mass;
		this->frictionCoefficient = fric;
		this->collisionMargin = collMargin;
		this->ghost = ghost;
		
		this->dynamic = dynamic;
		this->gravity = dynamic;

		this->createsLift = createsLift;
	}

};