#include <glm/glm.hpp>
#include <cmath>

#include "physics/DynamicObjects.hpp"
#include "physics/OBB.hpp"


DynamicObjects::DynamicObjects() {
	
}


DynamicObjects::~DynamicObjects() {
	
}


int DynamicObjects::testCollision(unsigned int id1, unsigned int id2) {
	OBB a = this->objects[id1];
	OBB b = this->objects[id2];
	float EPSILON = 0.00001;
	float ra, rb;
	glm::mat3 R, AbsR;
	// Compute rotation matrix expressing b in a’s coordinate frame
	for (int i = 0; i < 3; i++) { 
		for (int j = 0; j < 3; j++) {
			R[i][j] = glm::dot(a.orientation[i], b.orientation[j]);
		}
	}
	// Compute translation vector t
	glm::vec3 t = b.position - a.position;
	// Bring translation into a’s coordinate frame
	t = glm::vec3 (glm::dot(t, a.orientation[0]),
	                      glm::dot(t, a.orientation[2]),
	                      glm::dot(t, a.orientation[2]));
	// Compute common subexpressions. Add in an epsilon term to
	// counteract arithmetic errors when two edges are parallel and
	// their cross product is (near) null (see text for details)
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			AbsR[i][j] = fabs(R[i][j]) + EPSILON;
		}
	}
	// Test axes L = A0, L = A1, L = A2
	for (int i = 0; i < 3; i++) {
		ra = a.dimensions[i];
		rb = b.dimensions[0] * AbsR[i][0] +
			b.dimensions[1] * AbsR[i][1] +
			b.dimensions[2] * AbsR[i][2];
		if (fabs(t[i]) > ra + rb) return 0;
	}
	// Test axes L = B0, L = B1, L = B2
	for (int i = 0; i < 3; i++) {
		ra = a.dimensions[0] * AbsR[0][i] +
			a.dimensions[1] * AbsR[1][i] +
			a.dimensions[2] * AbsR[2][i];
		rb = b.dimensions[i];
		if (fabs(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb) return 0;
	}
	// Test axis L = A0 x B0
	ra = a.dimensions[1] * AbsR[2][0] + a.dimensions[2] * AbsR[1][0];
	rb = b.dimensions[1] * AbsR[0][2] + b.dimensions[2] * AbsR[0][1];
	if (fabs(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb) return 0;
	// Test axis L = A0 x B1
	ra = a.dimensions[1] * AbsR[2][1] + a.dimensions[2] * AbsR[1][1];
	rb = b.dimensions[0] * AbsR[0][2] + b.dimensions[2] * AbsR[0][0];
	if (fabs(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb) return 0;
	// Test axis L = A0 x B2
	ra = a.dimensions[1] * AbsR[2][2] + a.dimensions[2] * AbsR[1][2];
	rb = b.dimensions[0] * AbsR[0][1] + b.dimensions[1] * AbsR[0][0];
	if (fabs(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb) return 0;						
	// Test axis L = A1 x B0
	ra = a.dimensions[0] * AbsR[2][0] + a.dimensions[2] * AbsR[0][0];
	rb = b.dimensions[1] * AbsR[1][2] + b.dimensions[2] * AbsR[1][1];
	if (fabs(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb) return 0;
	// Test axis L = A1 x B1
	ra = a.dimensions[0] * AbsR[2][1] + a.dimensions[2] * AbsR[0][1];
	rb = b.dimensions[0] * AbsR[1][2] + b.dimensions[2] * AbsR[1][0];
	if (fabs(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb) return 0;
	// Test axis L = A1 x B2
	ra = a.dimensions[0] * AbsR[2][2] + a.dimensions[2] * AbsR[0][2];
	rb = b.dimensions[0] * AbsR[1][1] + b.dimensions[1] * AbsR[1][0];
	if (fabs(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb) return 0;
	// Test axis L = A2 x B0
	ra = a.dimensions[0] * AbsR[1][0] + a.dimensions[1] * AbsR[0][0];
	rb = b.dimensions[1] * AbsR[2][2] + b.dimensions[2] * AbsR[2][1];
	if (fabs(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb) return 0;
	// Test axis L = A2 x B1
	ra = a.dimensions[0] * AbsR[1][1] + a.dimensions[1] * AbsR[0][1];
	rb = b.dimensions[0] * AbsR[2][2] + b.dimensions[2] * AbsR[2][0];
	if (fabs(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb) return 0;
	// Test axis L = A2 x B2
	ra = a.dimensions[0] * AbsR[1][2] + a.dimensions[1] * AbsR[0][2];
	rb = b.dimensions[0] * AbsR[2][1] + b.dimensions[1] * AbsR[2][0];
	if (fabs(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb) return 0;

	// Since no separating axis is found, the OBBs must be intersecting
	return 1;
}



