#pragma once

#include "gl/glew.h"
#include "gl/wglew.h"
#pragma comment(lib, "glew32.lib")
#include "shaders/Shader.h"   // include shader header file, this is not part of OpenGL
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_inverse.hpp"

#include "Camera.h"

#include <vector>
using namespace std;

class Cameras {


public:
	static vector<Camera*> cameras;

	static void init();
	static void setCameras(vector<Camera*> camList);
	static void addCamera(Camera* cam);
	static void clearCameras();
};

