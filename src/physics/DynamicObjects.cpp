#include <glm/glm.hpp>
#include <cmath>


#include "physics/DynamicObjects.hpp"
#include "physics/OBB.hpp"


DynamicObjects::DynamicObjects() : idCounter(0) {
	
}


DynamicObjects::~DynamicObjects() {
	
}


int DynamicObjects::testCollision(int id1, int id2) {
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


int DynamicObjects::collideAllObjects() {
	int success = 1;
	for (auto it1 = objects.begin(); it1 != objects.end(); ++it1) {
		for (auto it2 = next(it1, 1); it2 != objects.end(); ++it2) {
			success = testCollision(it1->first, it2->first);
			if (!success) { break; }
		}
		if (!success) { break; }
	}
	return success;
}


int DynamicObjects::collideAllAgainst(int objID) {
	int success = 1;
	for (auto it = objects.begin(); it != objects.end(); ++it) {
		success = testCollision(objID, it->first);
		if (!success) { break; }
	}
	return success;
}


int DynamicObjects::addModelData(Vertex *vertices, std::size_t v_size,
                                 short *indices, std::size_t i_size)
{
	int newID = getID();
	
	modelData.insert(std::make_pair(newID,
	                                ModelData(vertices, v_size, indices, i_size)));
	return newID;
}


int DynamicObjects::addObject(int dataID, glm::vec3 pos, glm::vec3 rot,
                              glm::vec3 scale, glm::vec3 colour) {
	int id = getID();

	Model model = Model(pos, rot, scale, colour, &modelData.find(dataID)->second);
	objectModels.insert(std::make_pair(id, model));
	return id;
}


int DynamicObjects::addObject(int dataID, glm::vec3 pos) {
	return addObject(dataID, pos, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),
	                 glm::vec3(0, 0, 0));
}


// Set abs transformations -- no collision checking
void DynamicObjects::setPos(int id, glm::vec3 pos) {
	objects[id].position = pos;
	objectModels[id].pos = pos;
}


void DynamicObjects::setRot(int id, glm::vec3 rot) {
	glm::vec3 x = glm::vec3(1, 0, 0);
	glm::vec3 y = glm::vec3(0, 1, 0);
	glm::vec3 z = glm::vec3(0, 0, 1);

	glm::quat qX = glm::quat(rot.x, x);
	glm::quat qY = glm::quat(rot.y, y);
	glm::quat qZ = glm::quat(rot.z, z);
	glm::quat q = qX * qY * qZ;

	objects[id].orientation[0] = q * x;
	objects[id].orientation[1] = q * y;
	objects[id].orientation[2] = q * z;

	objectModels[id].rot = q;
}


void DynamicObjects::setScale(int id, glm::vec3 scale) {
	objects[id].dimensions = scale;
	objectModels[id].scale = scale;
}


// Transform object and check collision against other objects
int DynamicObjects::move(int id, glm::vec3 relPos) {
	glm::vec3 old = objects[id].dimensions;
	objects[id].position += relPos;
	int success = collideAllAgainst(id);
	if (!success) {
		objects[id].position = old;		
	} else {
		objectModels[id].pos = objects[id].position;
	}

	return success;
}


int DynamicObjects::rotate(int id, glm::vec3 relRot) {
	glm::quat q = objectModels[id].rot; 
	
	glm::vec3 x = glm::vec3(1, 0, 0);
	glm::vec3 y = glm::vec3(0, 1, 0);
	glm::vec3 z = glm::vec3(0, 0, 1);

	q = glm::rotate(q, relRot.x, x);
	q = glm::rotate(q, relRot.y, y);
	q = glm::rotate(q, relRot.z, z);

	OBB obj = objects[id];
	glm::vec3 oldX = obj.orientation[0];
	glm::vec3 oldY = obj.orientation[1];
	glm::vec3 oldZ = obj.orientation[2];
	obj.orientation[0] = q * x;
	obj.orientation[1] = q * y;
	obj.orientation[2] = q * z;

	int success = collideAllAgainst(id);
	if (!success) {
		obj.orientation[0] = oldX;
		obj.orientation[1] = oldY;
		obj.orientation[2] = oldZ;
	}	else {
		objectModels[id].rot = q;
	}

	return success;
}


int DynamicObjects::scale(int id, glm::vec3 relScale) {
	glm::vec3 old = objects[id].dimensions;
	objects[id].dimensions += relScale;
	int success = collideAllAgainst(id);
	if (!success) {
		objects[id].dimensions = old;		
	} else {
		objectModels[id].pos = objects[id].dimensions;
	}

	return success;
}

