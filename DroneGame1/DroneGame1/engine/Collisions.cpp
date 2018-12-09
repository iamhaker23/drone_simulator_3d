#include "Collisions.h"


bool Collisions::doSAT(ThreeDModel* a, ThreeDModel* b, glm::mat4 MVa, glm::mat4 MVb, bool chooser) {

	vector<Octree*> colliders = doSAT(a->octree, b->octree, MVa, MVb);

	return ((int)colliders.size() == 2);
}

glm::vec3 Collisions::doSAT(ThreeDModel* a, ThreeDModel* b, glm::mat4 MVa, glm::mat4 MVb) {

	glm::vec3 hitPoint = glm::vec3(0);

	vector<Octree*> colliders = doSAT(a->octree, b->octree, MVa, MVb);
	
	//cout << "COLLIDERS:" << (int)colliders.size() << endl;

	if ((int)colliders.size() == 2) {
		//cout << "COLLISION" << endl;

		Octree* aOct = colliders[0];
		Octree* bOct = colliders[1];

		glm::vec3 aLargestExtent = glm::vec3(aOct->box->verts[21], aOct->box->verts[22], aOct->box->verts[23]) - glm::vec3(aOct->box->verts[0], aOct->box->verts[1], aOct->box->verts[2]);
		glm::vec3 bLargestExtent = glm::vec3(bOct->box->verts[21], bOct->box->verts[22], bOct->box->verts[23]) - glm::vec3(bOct->box->verts[0], bOct->box->verts[1], bOct->box->verts[2]);

		glm::mat4 aLargestExtentWorld = glm::translate(glm::mat4(1.f), aLargestExtent);
		glm::mat4 bLargestExtentWorld = glm::translate(glm::mat4(1.f), bLargestExtent);

		glm::mat4 hitPointM = bLargestExtentWorld + (aLargestExtentWorld - bLargestExtentWorld/2.f);
		hitPoint = glm::vec3(hitPointM[3][0], hitPointM[3][1], hitPointM[3][2]);
		//hitPoint = glm::vec3(1.0f, 1.0f, 1.0f);
		return hitPoint;
	}
	else {
		return hitPoint;
	}

}


vector<Octree*> Collisions::doSAT(Octree* a, Octree* b, glm::mat4 MVa, glm::mat4 MVb) {
	
	//int count = 0;

	vector<Octree*> colliders = vector<Octree*>();

	if (SAT(a->box, b->box, MVa, MVb)) {

		//cout << "HIT Level_" << a->getLevel() << " " << count << endl;

		if (a->getLevel() != MAX_DEPTH) {
			for (int aChild = 0; aChild < 8; aChild++) {
				for (int bChild = 0; bChild < 8; bChild++) {
					Octree* aChildOct = a->getChild(aChild);
					Octree* bChildOct = b->getChild(bChild);
					if (aChildOct != NULL && aChildOct->VertexListSize != 0 && bChildOct != NULL && bChildOct->VertexListSize != 0) {

						//cout << "A: " << aChild << " B:" << bChild << endl;
						//count++;

						colliders = doSAT(aChildOct, bChildOct, MVa, MVb);
						if ((int)colliders.size() > 0) {
							
							return colliders;
						}
					}
				}
			}
		}
		else {

			//cout << "LOWEST HIT Level_" << a->getLevel() << " " << count << endl;

			colliders.push_back(a);
			colliders.push_back(b);
			return colliders;
		}
	}
	//cout << "NO HIT Level_" << a->getLevel() << " "<<count << endl;

	//worst case near miss on octree leaf
	return colliders;

}

bool Collisions::SAT(Box* a, Box* b, glm::mat4 MVa, glm::mat4 MVb) {
	
	
	
	vector<glm::vec3> axes = getAxes(a, b, MVa, MVb);
	//for each axis
	for (int i = 0; i < (int)axes.size(); i++) {
		glm::vec3 axis = axes[i];
		
		if (glm::vec3(0) == axis) continue;
		axis = glm::normalize(axis);

		float aproj[2] = { 0, 0 };
		float bproj[2] = { 0, 0 };
		projectBox(aproj, a, axis, MVa);
		projectBox(bproj, b, axis, MVb);
		if (!overlap(aproj, bproj)) {
			//cout << "SEP ------- AXIS(" << i << ") (" << axis.x << "," << axis.y << "," << axis.z << ") : "  << aproj[0] << "-" << aproj[1] << "AND " << bproj[0] << "-" << bproj[1] << endl;
			return false;
		}
		else {
			//cout << "NOSEP ----- AXIS(" << i << ") (" << axis.x << "," << axis.y << "," << axis.z << ") : " << aproj[0] << "-" << aproj[1] << "AND " << bproj[0] << "-" << bproj[1] << endl;
		}
	}
	return true;
}

bool Collisions::overlap(float a[], float b[]){
	float coverage = ((a[1] > b[1]) ? a[1] : b[1]) - ((a[0] < b[0]) ? a[0] : b[0]);
	float summation = (a[1] - a[0]) + (b[1] - b[0]);
	return coverage <= summation;

}

void Collisions::projectBox(float minMax[], Box* a, glm::vec3 axis, glm::mat4 MVa) {


	glm::mat4 point = MVa * glm::translate(glm::mat4(1.f), glm::vec3(a->verts[0], a->verts[1], a->verts[2]));
	float min = glm::dot(axis, glm::vec3(point[3][0], point[3][1], point[3][2]));
	float max = min;

	for (int i = 3; i < NumberOfVertexCoords; i+=3) {

		glm::mat4 point = MVa * glm::translate(glm::mat4(1.f), glm::vec3(a->verts[i], a->verts[i+1], a->verts[i+2]));
		float curr = glm::dot(axis, glm::vec3(point[3][0], point[3][1], point[3][2]));

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

vector<glm::vec3> Collisions::getAxes(Box* a, Box* b, glm::mat4 MVa, glm::mat4 MVb) {

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