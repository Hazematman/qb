#pragma once

#include <unordered_map>
#include <vector>

#include "physics/OBB.hpp"
#include "renderer/model.hpp"


class DynamicObjects {
  int idCounter;
  int getID() { return idCounter++; }

	std::unordered_map<int, OBB> objects; 
	std::unordered_map<int, Model> objectModels; 

	std::unordered_map<int, ModelData> modelData;

	int testCollision(int id1, int id2);
	int collideAllAgainst(int objID);
	int collideAllObjects();
public:
	DynamicObjects();
	~DynamicObjects();

	// Returns id of model for reuse for future objects
  int addModelData(Vertex *vertices, std::size_t v_size,
                   short *indices, std::size_t i_size);

  // Must call 'addModelData' before creating object
	// Returns id of object 
	int addObject(int dataID, glm::vec3 pos);
	int addObject(int dataID, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
	              glm::vec3 colour);

	// Transform object and check collision against other objects
	int move(int id, glm::vec3 relRot);
	int rotate(int id, glm::vec3 relRot);
	int scale(int id, glm::vec3 relScale);

	// Set abs transformations -- no collision checking
	void setPos(int id, glm::vec3 pos);
	void setRot(int id, glm::vec3 scale);
	void setScale(int id, glm::vec3 rot);
};

