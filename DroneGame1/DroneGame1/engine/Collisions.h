#pragma once

#include "gl/glew.h"
#include "gl/wglew.h"
#pragma comment(lib, "glew32.lib")
#include "shaders/Shader.h"   // include shader header file, this is not part of OpenGL
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include "../lib/Octree/Octree.h"
#include <vector>

using namespace std;

class Collisions {


public:
#include "Collisions.h"

	static bool Collisions::doSAT(Octree* a, Octree* b, glm::mat4 MVa);
	static bool Collisions::overlap(float* a, float* b);
	static float* Collisions::projectBox(Box* a, glm::vec3 axis);
	static vector<glm::vec3> Collisions::getAxes(Box* a, Box* b, glm::mat4 MVa);
};

