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

class Grid {
public:

	vector<GameObject*> objects = vector<GameObject*>();

	static const int midpoint = 2;
	float resolution = 100.f;

	//midpoint 2 will give a 4x4x4 grid
	vector<GameObject*> grid[midpoint* 2][midpoint * 2][midpoint * 2] = { vector<GameObject*>() };

	inline Grid() {

	}

	inline ~Grid() {
		objects.clear();
	}

	inline Grid(vector<GameObject*> objects) {

		for (int i = 0; i < (int)objects.size(); i++) {
			
			addObject(objects[i]);

		}
	}
	
	inline void Grid::addObject(GameObject* toAdd) {

		GameObject* curr = toAdd;

		objects.push_back(curr);

		if (curr->physics == NULL || curr->physics->collisions == false) return;

		int x = (int)(((curr->worldX - (resolution / 2.f)) - (std::fmodf(curr->worldX, resolution))) / resolution);
		int y = (int)(((curr->worldY - (resolution / 2.f)) - (std::fmodf(curr->worldY, resolution))) / resolution);
		int z = (int)(((curr->worldZ - (resolution/2.f)) - (std::fmodf(curr->worldZ, resolution))) / resolution);

		//centre the grid to 0,0,0
		//x -= midpoint;
		//y -= midpoint;
		//z -= midpoint;

		//anything outside the grid will appear on the edges of the grid
		if (x <= -midpoint ) x = 1-midpoint ;
		else if (x >= midpoint ) x = midpoint-1 ;

		if (y <= -midpoint ) y = 1-midpoint ;
		else if (y >= midpoint ) y = midpoint-1 ;

		if (z <= -midpoint) z = 1-midpoint ;
		else if (z >= midpoint ) z = midpoint-1 ;

		
		//cout << "GRID:" << curr->name << "@" << x << "," << y << "," << z << endl;
		grid[x + midpoint][y + midpoint][z + midpoint].push_back(curr);
	}

};