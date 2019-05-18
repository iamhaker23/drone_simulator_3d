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
	
	float ambient[4] = { .2f, .2f, .2f, 1.f};
	float diffuse[4] = { .8f, .8f, .8f, 1.f };
	float specular[4] = { 1.f, 1.f, 1.f, 1.f };
	float shininess = 10.f;
	int shadeless = 0;
	int normalMapping = 0;
	float alphaClipThreshold = 0.f;
	float uvOffset[2] = { 0 };

	inline Material() {
	}


};