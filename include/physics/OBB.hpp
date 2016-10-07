#pragma once

#include <glm/glm.hpp>

// Oriented Bounding Box
struct OBB {
	glm::vec3 position; // c
	glm::vec3 orientation[3]; // u
	glm::vec3 dimensions; // e

	OBB();
	~OBB();
};
