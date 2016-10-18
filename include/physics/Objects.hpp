#pragma once

#include <unordered_map>
#include <vector>

#include "physics/OBB.hpp"
#include "physics/Objects.hpp"
#include "renderer/model.hpp"


class Objects {
	int idCounter;
protected:
	int getID() { return idCounter++; }

	std::unordered_map<int, OBB> objects; 
	std::unordered_map<int, Model> objectModels; 

	std::unordered_map<int, ModelData> modelData;

  int testCollision(int id1, int id2);

public:
  Objects();
  ~Objects();

	glm::mat4 getObjectTransform(int id);

	// Returns id of model for reuse for future objects
	int addModelData(Vertex *vertices, std::size_t v_size,
	                 short *indices, std::size_t i_size);

	// Must call 'addModelData' before creating object
	// Returns id of object 
	int addObject(int dataID, glm::vec3 pos);
	int addObject(int dataID, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale,
	              glm::vec3 colour);


};
