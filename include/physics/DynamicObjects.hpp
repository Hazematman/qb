#pragma once

#include <unordered_map>
#include <vector>


class OBB;

class DynamicObjects {
	std::unordered_map<unsigned int, OBB> objects; 

	int testCollision(unsigned int id1, unsigned int id2);
public:
	DynamicObjects();
	~DynamicObjects();
};
