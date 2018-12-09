#include "Collisions.h"


bool Collisions::doSAT(Octree* a, Octree* b, glm::mat4 MVa) {
	
	if (SAT(a->box, b->box, MVa)) {
		if (a->getLevel() != MAX_DEPTH) {
			for (int aChild = 0; aChild < 8; aChild++) {
				for (int bChild = 0; bChild < 8; bChild++) {
					Octree* aChildOct = a->getChild(aChild);
					Octree* bChildOct = b->getChild(bChild);
					if (aChildOct != NULL && bChildOct != NULL && doSAT(aChildOct, bChildOct, MVa)) return true;
				}
			}
		}
		else {
			//lowest level octree collision
			return true;
		}
	}
	
	//worst case near miss on octree leaf
	return false;

}

bool Collisions::SAT(Box* a, Box* b, glm::mat4 MVa) {
	vector<glm::vec3> axes = getAxes(a, b, MVa);
	//for each axis
	for (int i = 0; i < (int)axes.size(); i++) {
		glm::vec3 axis = axes[i];
		
		if (glm::vec3(0) == axis) continue;

		float aproj[2] = { 0, 0 };
		float bproj[2] = { 0, 0 };
		projectBox(aproj, a, axis);
		projectBox(bproj, b, axis, MVa);

		if (!overlap(aproj, bproj)) {
			return false;
		}
	}
	//all axis overlap 
	return true;
}

bool Collisions::overlap(float a[], float b[]){
	float coverage = ((a[1] > b[1]) ? a[1] : b[1]) - ((a[0] < b[0]) ? a[0] : b[0]);
	float summation = (a[1] - a[0]) + (b[1] - b[0]);
	return coverage <= summation;

}

void Collisions::projectBox(float minMax[], Box* a, glm::vec3 axis) {
	float min = glm::dot(axis, glm::vec3(a->verts[0], a->verts[1], a->verts[2]));
	float max = min;

	for (int i = 3; i < NumberOfVertexCoords; i+=3) {
		float curr = glm::dot(axis, glm::vec3(a->verts[i], a->verts[i+1], a->verts[i+2]));
		if (curr > max) {
			max = curr;
		}
		else if (curr < min) {
			min = curr;
		}
	}

	minMax[0] = min;
	minMax[1] = max;
}

void Collisions::projectBox(float minMax[], Box* a, glm::vec3 axis, glm::mat4 MVa) {

	glm::mat4 min_mat = glm::translate(glm::mat4(1.f), glm::vec3(a->verts[0], a->verts[1], a->verts[2]))*MVa;
	glm::vec3 point = glm::vec3(min_mat[3][0], min_mat[3][1], min_mat[3][2]);
	float min = glm::dot(axis, point);
	float max = min;

	for (int i = 3; i < NumberOfVertexCoords; i += 3) {
		
		glm::mat4 min_mat = glm::translate(glm::mat4(1.f), glm::vec3(a->verts[i], a->verts[i+1], a->verts[i+2]))*MVa;
		glm::vec3 point = glm::vec3(min_mat[3][0], min_mat[3][1], min_mat[3][2]);
		float curr = glm::dot(axis, point);

		if (curr > max) {
			max = curr;
		}
		else if (curr < min) {
			min = curr;
		}
	}

	minMax[0] = min;
	minMax[1] = max;
}

vector<glm::vec3> Collisions::getAxes(Box* a, Box* b, glm::mat4 MVa) {

	vector<glm::vec3> axes = vector<glm::vec3>();

	glm::vec3 v1 = glm::vec3(a->verts[0], 0, 0);
	glm::vec3 v2 = glm::vec3(a->verts[21], 0, 0);
	glm::vec3 a0 = (v1 - v2);

	v1 = glm::vec3(0, a->verts[1], 0);
	v2 = glm::vec3(0, a->verts[22], 0);
	glm::vec3 a1 = (v1 - v2);

	v1 = glm::vec3(0, 0, a->verts[2]);
	v2 = glm::vec3(0, 0, a->verts[23]);
	glm::vec3 a2 = (v1 - v2);

	v1 = glm::vec3(b->verts[0], 0, 0);
	v2 = glm::vec3(b->verts[21], 0, 0);
	glm::mat4 b0m = glm::translate(glm::mat4(1.f), (v1 - v2))*MVa;

	v1 = glm::vec3(0, b->verts[1], 0);
	v2 = glm::vec3(0, b->verts[22], 0);
	glm::mat4 b1m = glm::translate(glm::mat4(1.f), (v1 - v2))*MVa;
	
	v1 = glm::vec3(0, 0, b->verts[2]);
	v2 = glm::vec3(0, 0, b->verts[23]);
	glm::mat4 b2m = glm::translate(glm::mat4(1.f), (v1 - v2))*MVa;

	glm::vec3 b0 = glm::vec3(b0m[3][0], b0m[3][1], b0m[3][2]);
	glm::vec3 b1 = glm::vec3(b1m[3][0], b1m[3][1], b1m[3][2]);
	glm::vec3 b2 = glm::vec3(b2m[3][0], b2m[3][1], b2m[3][2]);

	glm::vec3 axis7 = glm::cross(a0, b0);
	glm::vec3 axis8 = glm::cross(a0, b1);
	glm::vec3 axis9 = glm::cross(a0, b2);

	glm::vec3 axis10 = glm::cross(a1, b0);
	glm::vec3 axis11 = glm::cross(a1, b1);
	glm::vec3 axis12 = glm::cross(a1, b2);

	glm::vec3 axis13 = glm::cross(a2, b0);
	glm::vec3 axis14 = glm::cross(a2, b1);
	glm::vec3 axis15 = glm::cross(a2, b2);

	axes.push_back(a0);
	axes.push_back(a1);
	axes.push_back(a2);
	axes.push_back(b0);
	axes.push_back(b1);
	axes.push_back(b2);
	axes.push_back(axis7);
	axes.push_back(axis8);
	axes.push_back(axis9);
	axes.push_back(axis10);
	axes.push_back(axis11);
	axes.push_back(axis12);
	axes.push_back(axis13);
	axes.push_back(axis14);
	axes.push_back(axis15);

	return axes;
}