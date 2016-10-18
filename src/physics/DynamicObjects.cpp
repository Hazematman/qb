#include <glm/glm.hpp>
#include <cmath>


#include "physics/DynamicObjects.hpp"
#include "physics/OBB.hpp"


DynamicObjects::DynamicObjects() : Objects() {
	
}


DynamicObjects::~DynamicObjects() {
	
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
