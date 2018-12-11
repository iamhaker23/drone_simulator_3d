#pragma once

#include "gl/glew.h"
#include "gl/wglew.h"
#pragma comment(lib, "glew32.lib")
#include "shaders/Shader.h"   // include shader header file, this is not part of OpenGL
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include "../lib/3dStruct/threeDModel.h"
#include "../lib/Octree/Octree.h"
#include <vector>

using namespace std;

class Collisions {


public:
#include "Collisions.h"
	

	static glm::vec3 Collisions::triSAT(ThreeDModel* a, ThreeDModel* b, int aTriIdx, int bTriIdx, glm::mat4 MVa, glm::mat4 MVb, float ascale, float bscale);
	static vector<glm::vec3> Collisions::getAxesTri(vector<glm::vec3> a, vector<glm::vec3> b, glm::mat4 MVa, glm::mat4 MVb);
	static void projectTris(float minMax[], vector<glm::vec3> aTri, glm::vec3 axis, glm::mat4 MVa);

	static glm::vec3 doSAT(int collisionType, ThreeDModel* a, ThreeDModel* b, glm::mat4 MVa, glm::mat4 MVb, float ascale, float bscale);
	static vector<Octree*> doSAT(bool goToMaxDepth, Octree* a, Octree* b, glm::mat4 MVa, glm::mat4 MVb, bool overrideBoundingBoxSAT);
	static vector<Octree*> doSAT(bool goToMaxDepth, Octree* a, Octree* b, glm::mat4 MVa, glm::mat4 MVb);

	static bool Collisions::SAT(Box* a, Box* b, glm::mat4 MVa, glm::mat4 MVb);
	
	static bool Collisions::overlap(float a[], float b[]);

	static void projectBox(float minMax[], Box* a, glm::vec3 axis, glm::mat4 MVa);
	static vector<glm::vec3> Collisions::getAxes(Box* a, Box* b, glm::mat4 MVa, glm::mat4 MVb);
};

