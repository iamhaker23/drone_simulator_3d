#include "Collisions.h"

glm::vec3 Collisions::doSAT(int collisionType, ThreeDModel* a, ThreeDModel* b, glm::mat4 MVa, glm::mat4 MVb, float ascale, float bscale) {
	

	//Assumes root octree is the same as bounding box...

	bool goToMaxDepth = (collisionType != 1);
	vector<Octree*> colliders = doSAT(goToMaxDepth, a->octree, b->octree, MVa, MVb);
	if ((int)colliders.size() == 2) {
		if (collisionType != 3) {

			Octree* bOct = colliders[1];
			glm::vec3 centreOfBOct = glm::vec3(bOct->box->verts[0], bOct->box->verts[1], bOct->box->verts[2]) + ((glm::vec3(bOct->box->verts[21], bOct->box->verts[22], bOct->box->verts[23]) - glm::vec3(bOct->box->verts[0], bOct->box->verts[1], bOct->box->verts[2])) / 2.0f);
			Octree* aOct = colliders[0];
			glm::vec3 centreOfAOct = glm::vec3(aOct->box->verts[0], aOct->box->verts[1], aOct->box->verts[2]) + ((glm::vec3(aOct->box->verts[21], aOct->box->verts[22], aOct->box->verts[23]) - glm::vec3(aOct->box->verts[0], aOct->box->verts[1], aOct->box->verts[2])) / 2.0f);

			return centreOfBOct - centreOfAOct;
		}
		else {
			Octree* aOct = colliders[0];
			Octree* bOct = colliders[1];

			int aTris = aOct->PrimitiveListSize;
			int bTris = bOct->PrimitiveListSize;

			//cout << "Triangle test:" << aTris * bTris << endl;

			for (int aTriIdx = 0; aTriIdx < aTris; aTriIdx++) {
				//cout << aOct->PrimitiveList[a] << "," << aOct->PrimitiveList[a + 1] << "," << aOct->PrimitiveList[a + 2] << endl;
				for (int bTriIdx = 0; bTriIdx < bTris; bTriIdx++) {
					//cout << aOct->PrimitiveList[a] << "," << aOct->PrimitiveList[a + 1] << "," << aOct->PrimitiveList[a + 2] << endl;
					glm::vec3 hitPoint = triSAT(a, b, aTriIdx, bTriIdx, MVa, MVb, ascale, bscale);
					if (hitPoint.x != 0 && hitPoint.y != 0 && hitPoint.z != 0) {
						
						return hitPoint;

					}

				}
			}
		}
	}
	
	return glm::vec3(0);
}

glm::vec3 Collisions::triSAT(ThreeDModel* a, ThreeDModel* b, int aTriIdx, int bTriIdx, glm::mat4 MVa, glm::mat4 MVb, float ascale, float bscale) {
	
	vector<glm::vec3> aTri = vector<glm::vec3>();
	vector<glm::vec3> bTri = vector<glm::vec3>();

	aTri.push_back(glm::vec3(a->theVerts[a->theFaces[aTriIdx].thePoints[0]].x, a->theVerts[a->theFaces[aTriIdx].thePoints[0]].y, a->theVerts[a->theFaces[aTriIdx].thePoints[0]].z));
	aTri.push_back(glm::vec3(a->theVerts[a->theFaces[aTriIdx].thePoints[1]].x, a->theVerts[a->theFaces[aTriIdx].thePoints[1]].y, a->theVerts[a->theFaces[aTriIdx].thePoints[1]].z));
	aTri.push_back(glm::vec3(a->theVerts[a->theFaces[aTriIdx].thePoints[2]].x, a->theVerts[a->theFaces[aTriIdx].thePoints[2]].y, a->theVerts[a->theFaces[aTriIdx].thePoints[2]].z));

	bTri.push_back(glm::vec3(b->theVerts[b->theFaces[bTriIdx].thePoints[0]].x, b->theVerts[b->theFaces[bTriIdx].thePoints[0]].y, b->theVerts[b->theFaces[bTriIdx].thePoints[0]].z));
	bTri.push_back(glm::vec3(b->theVerts[b->theFaces[bTriIdx].thePoints[1]].x, b->theVerts[b->theFaces[bTriIdx].thePoints[1]].y, b->theVerts[b->theFaces[bTriIdx].thePoints[1]].z));
	bTri.push_back(glm::vec3(b->theVerts[b->theFaces[bTriIdx].thePoints[2]].x, b->theVerts[b->theFaces[bTriIdx].thePoints[2]].y, b->theVerts[b->theFaces[bTriIdx].thePoints[2]].z));

	glm::mat4 aTriMat0 = glm::translate(glm::mat4(1.f), aTri[0]) * MVa;
	glm::mat4 aTriMat1 = glm::translate(glm::mat4(1.f), aTri[1]) * MVa;
	glm::mat4 aTriMat2 = glm::translate(glm::mat4(1.f), aTri[2]) * MVa;

	glm::mat4 bTriMat0 = glm::translate(glm::mat4(1.f), bTri[0]) * MVb;
	glm::mat4 bTriMat1 = glm::translate(glm::mat4(1.f), bTri[1]) * MVb;
	glm::mat4 bTriMat2 = glm::translate(glm::mat4(1.f), bTri[2]) * MVb;

	aTri[0] = glm::vec3(aTriMat0[3][0], aTriMat0[3][1], aTriMat0[3][2]);
	aTri[1] = glm::vec3(aTriMat1[3][0], aTriMat1[3][1], aTriMat1[3][2]);
	aTri[2] = glm::vec3(aTriMat2[3][0], aTriMat2[3][1], aTriMat2[3][2]);
	
	bTri[0] = glm::vec3(bTriMat0[3][0], bTriMat0[3][1], bTriMat0[3][2]);
	bTri[1] = glm::vec3(bTriMat1[3][0], bTriMat1[3][1], bTriMat1[3][2]);
	bTri[2] = glm::vec3(bTriMat2[3][0], bTriMat2[3][1], bTriMat2[3][2]);
	
	glm::vec3 avgDistVec = ((aTri[0] - bTri[0]) + (aTri[1] - bTri[1]) + (aTri[2] - bTri[2])) / 3.f;
	float avgDistSqr = (avgDistVec.x*avgDistVec.x) + (avgDistVec.y*avgDistVec.y) + (avgDistVec.z*avgDistVec.z);

	glm::vec3 aSizeAvg = aTri[0] - aTri[1] - aTri[2];
	glm::vec3 bSizeAvg = bTri[0] - bTri[1] - bTri[2];

	float avgSizeRatio = ((aSizeAvg.x*aSizeAvg.x) + (aSizeAvg.y*aSizeAvg.y) + (aSizeAvg.z*aSizeAvg.z))
		+ ((bSizeAvg.x*bSizeAvg.x) + (bSizeAvg.y*bSizeAvg.y) + (bSizeAvg.z*bSizeAvg.z));
	avgDistSqr /= avgSizeRatio/2.f;

	//avgDistSqr /= (ascale*a->boundingBox.getLargestExtent()) * (bscale*b->boundingBox.getLargestExtent());

	//cout << "AVG TRI" << avgDistSqr << endl;

	if (avgDistSqr <= 0.001f) {
		return ((aTri[0] + aTri[1] + aTri[2]) / 3.f) + (avgDistVec/2.f);
	}
	return glm::vec3(0.f);

	/*
	vector<glm::vec3> axes = getAxesTri(aTri, bTri, MVa, MVb);

	for (int i = 0; i < (int)axes.size(); i++) {
		glm::vec3 axis = axes[i];

		if (glm::vec3(0) == axis) continue;
		axis = glm::normalize(axis);

		float aproj[2] = { 0, 0 };
		float bproj[2] = { 0, 0 };
		projectTris(aproj, aTri, axis, MVa);
		projectTris(bproj, bTri, axis, MVb);

		if (!overlap(aproj, bproj)) {
			//cout << "SEP ------- AXIS(" << i << ") (" << axis.x << "," << axis.y << "," << axis.z << ") : "  << aproj[0] << "-" << aproj[1] << "AND " << bproj[0] << "-" << bproj[1] << endl;
			return false;
		}
		else {
			//cout << "NOSEP ----- AXIS(" << i << ") (" << axis.x << "," << axis.y << "," << axis.z << ") : " << aproj[0] << "-" << aproj[1] << "AND " << bproj[0] << "-" << bproj[1] << endl;
		}
	}
	return true;
	*/
}

void Collisions::projectTris(float minMax[], vector<glm::vec3> aTri, glm::vec3 axis, glm::mat4 MVa) {

	glm::mat4 point = MVa * glm::translate(glm::mat4(1.f), aTri[0]);
	float min = glm::dot(axis, glm::vec3(point[3][0], point[3][1], point[3][2]));
	float max = min;

	for (int i = 1; i < 3; i++) {

		glm::mat4 point = MVa * glm::translate(glm::mat4(1.f), aTri[i]);
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


vector<glm::vec3> Collisions::getAxesTri(vector<glm::vec3> a, vector<glm::vec3> b, glm::mat4 MVa, glm::mat4 MVb) {

	vector<glm::vec3> axes = vector<glm::vec3>();

	glm::vec3 v1 = a[0];
	glm::vec3 v2 = a[1];
	glm::mat4 a0m = glm::translate(glm::mat4(1.f), (v1 - v2));

	v1 = a[1];
	v2 = a[2];
	glm::mat4 a1m = glm::translate(glm::mat4(1.f), (v1 - v2));

	v1 = a[2];
	v2 = a[0];
	glm::mat4 a2m = glm::translate(glm::mat4(1.f), (v1 - v2));

	glm::vec3 a0 = glm::vec3(a0m[3][0], a0m[3][1], a0m[3][2]);
	glm::vec3 a1 = glm::vec3(a1m[3][0], a1m[3][1], a1m[3][2]);
	glm::vec3 a2 = glm::vec3(a2m[3][0], a2m[3][1], a2m[3][2]);

	v1 = b[0];
	v2 = b[1];
	glm::mat4 b0m = glm::translate(glm::mat4(1.f), (v1 - v2))*MVa;

	v1 = b[1];
	v2 = b[2];
	glm::mat4 b1m = glm::translate(glm::mat4(1.f), (v1 - v2))*MVa;

	v1 = b[2];
	v2 = b[0];
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

vector<Octree*> Collisions::doSAT(bool goToMaxDepth, Octree* a, Octree* b, glm::mat4 MVa, glm::mat4 MVb) {
	return Collisions::doSAT(goToMaxDepth, a, b, MVa, MVb, false);
}
vector<Octree*> Collisions::doSAT(bool goToMaxDepth, Octree* a, Octree* b, glm::mat4 MVa, glm::mat4 MVb, bool overrideBoundingBoxSAT) {
	
	//int count = 0;

	vector<Octree*> colliders = vector<Octree*>();

	if (a->VertexListSize > 0 && b->VertexListSize > 0 && (overrideBoundingBoxSAT || SAT(a->box, b->box, MVa, MVb))) {

		//cout << "HIT Level_" << a->getLevel() << " " << count << endl;

		//if (goToMaxDepth) cout << "Go To Max Depth" << endl;
		if (goToMaxDepth && a->getLevel() != MAX_DEPTH) {
			
			Octree* aIntersects = NULL;

			//find lowest-level octrees which intersects with b
			for (int aChild = 0; aChild < 8; aChild++) {
				Octree* aChildOct = a->getChild(aChild);
				if (aChildOct != NULL && aChildOct->VertexListSize > 0) {
					bool isAChildIntersecting = SAT(aChildOct->box, b->box, MVa, MVb);
					if (isAChildIntersecting) {// && aIntersection[0]->getLevel() == MAX_DEPTH) {
						aIntersects = aChildOct;
					}
				}
			}

			if (aIntersects!= NULL) {
				//find lowest-level octrees which intersect with a
				for (int bChild = 0; bChild < 8; bChild++) {
					Octree* bChildOct = b->getChild(bChild);
					if (bChildOct != NULL && bChildOct->VertexListSize > 0) {
						if (SAT(aIntersects->box, bChildOct->box, MVa, MVb)) {
							//colliders.push_back(aIntersects[aIntersectIdx]);
							//colliders.push_back(bIntersects[bIntersectIdx]);
							//return colliders;
							vector<Octree*> possibleHit = doSAT(goToMaxDepth, aIntersects, bChildOct, MVa, MVb, true);
							if ((int)possibleHit.size() > 0) {
								return possibleHit;
							}
						}
					}
				}
			}
			else {
				return colliders;
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
	glm::mat4 a0m = glm::translate(glm::mat4(1.f), (v1 - v2));

	v1 = glm::vec3(0, a->verts[1], 0);
	v2 = glm::vec3(0, a->verts[22], 0);
	glm::mat4 a1m = glm::translate(glm::mat4(1.f), (v1 - v2));

	v1 = glm::vec3(0, 0, a->verts[2]);
	v2 = glm::vec3(0, 0, a->verts[23]);
	glm::mat4 a2m = glm::translate(glm::mat4(1.f), (v1 - v2));

	glm::vec3 a0 = glm::vec3(a0m[3][0], a0m[3][1], a0m[3][2]);
	glm::vec3 a1 = glm::vec3(a1m[3][0], a1m[3][1], a1m[3][2]);
	glm::vec3 a2 = glm::vec3(a2m[3][0], a2m[3][1], a2m[3][2]);

	
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