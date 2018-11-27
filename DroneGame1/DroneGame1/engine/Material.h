#pragma once

#include "gl/glew.h"
#include "gl/wglew.h"
#pragma comment(lib, "glew32.lib")
#include "shaders/Shader.h"   // include shader header file, this is not part of OpenGL
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_inverse.hpp"

class Material {
public:
	
	float ambient[4] = { 0.1f, 0.1f, 0.1f, 1.f};
	float diffuse[4] = { 0.7f, 0.7f, 0.7f, 1.f };
	float specular[4] = { .7f, .7f, .7f, 1.f };
	float shininess = 50.0f;

	inline Material() {
	}


};