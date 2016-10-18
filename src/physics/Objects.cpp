#include "physics/Objects.hpp"


Objects::Objects() : idCounter(0) {
}

Objects::~Objects() {

}


int Objects::testCollision(int id1, int id2) {
	if (!objects.count(id1) || !objects.count(id2)) {
		return 1;
	} 
	return this->objects[id1].testAgainst(this->objects[id2]);
}


glm::mat4 Objects::getObjectTransform(int id) {
	if (objects.count(id)) {
		return objectModels[id].getTransormMat();
	}
	return glm::mat4(1);
}


int Objects::addModelData(Vertex *vertices, std::size_t v_size,
                                 short *indices, std::size_t i_size)
{
	int newID = getID();
	
	modelData.insert(std::make_pair(newID,
	                                ModelData(vertices, v_size, indices, i_size)));
	return newID;
}


int Objects::addObject(int dataID, glm::vec3 pos, glm::vec3 rot,
                              glm::vec3 scale, glm::vec3 colour) {
	int id = getID();

	Model model = Model(pos, rot, scale, colour, &modelData.find(dataID)->second);
	objectModels.insert(std::make_pair(id, model));
	return id;
}


int Objects::addObject(int dataID, glm::vec3 pos) {
	return addObject(dataID, pos, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0),
	                 glm::vec3(0, 0, 0));
}
