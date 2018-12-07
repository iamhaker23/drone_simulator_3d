#include "Collisions.h"


bool Collisions::doSAT(Octree* a, Octree* b, glm::mat4 MVa) {

	//NOTE: will convert b into a-localspace
	Octree* aLevel = a;
	Octree* bLevel = b;

	vector<glm::vec3> axes = getAxes(a->box, b->box, MVa);
	//for each axis
	for (int i = 0; i < (int)axes.size(); i++) {
		glm::vec3 axis = axes[i];
		float* aproj = projectBox(a->box, axis);
		float* bproj = projectBox(b->box, axis);
			if (!overlap(aproj, bproj)) return false;
			else {
				if (a->getLevel()== MAX_DEPTH) {
					//collision at the lowest level
					return true;
				}
				else {
					for each child in
						for each child in b
							if doSAT(a.children[ai], b.children[bi])
								return true
								//no children collide
								return false
				}
			}
	}

}

bool Collisions::overlap(float* a, float* b) {

	return a[0] - b[1] < 0 || a[1] - b[0] < 0;

}

float* Collisions::projectBox(Box a, glm::vec3 axis) {
	float min = glm::dot(axis, glm::vec3(a.top, a.left, a.front));
	float max = min;

	for (int i = 1; i < 8; i++) {
		float curr = glm::dot(axis, glm::vec3(a.top, a.left, a.front));
		if (curr > max) {
			max = curr;
		}
		else if (curr < min) {
			min = curr;
		}
	}

	return float[2]{ min, max };
}

vector<glm::vec3> Collisions::getAxes(Box* a, Box* b, glm::mat4 MVa) {

	vector<glm::vec3> axes = vector<glm::vec3>();

	v1 = glm::vec3(a->box->left, 0, 0);
	v2 = glm::vec3(a->box->right, 0, 0);
	axis1 = (v1 - v2);

	v1 = glm::vec3(0, a->box->top, 0);
	v2 = glm::vec3(0, a->box->bottom, 0);
	axis2 = (v1 - v2);

	v1 = glm::vec3(0, 0, a->box->front);
	v2 = glm::vec3(0, 0, a->box->back);
	axis3 = (v1 - v2);

	v1 = glm::vec3(b->box->left, 0, 0);
	v2 = glm::vec3(b->box->right, 0, 0);
	axis4 = (v1 - v2)*MVa;

	v1 = glm::vec3(0, b->box->top, 0);
	v2 = glm::vec3(0, b->box->bottom, 0);
	axis5 = (v1 - v2)*MVa;

	v1 = glm::vec3(0, 0, b->box->front);
	v2 = glm::vec3(0, 0, b->box->back);
	axis6 = (v1 - v2)*MVa;

	axis7 = glm::cross(a0, b0);
	axis8 = glm::cross(a0, b0);
	axis8 = glm::cross(a0, b0);
	axis10 = glm::cross(a0, b0);
	axis11 = glm::cross(a0, b0);
	axis12 = glm::cross(a0, b0);
	axis13 = glm::cross(a0, b0);
	axis14 = glm::cross(a0, b0);
	axis15 = glm::cross(a0, b0);

	axes.push_back(axis1);
	axes.push_back(axis2);
	axes.push_back(axis3);
	axes.push_back(axis4);
	axes.push_back(axis5);
	axes.push_back(axis6);
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