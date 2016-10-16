#include <iostream>

#include "levels/level.hpp"


Level::Level() {

}

Level::Level(std::vector<Entity> entities) {
  this->entities = entities;
}

Level::~Level() {

}

void Level::printLevelDeets() {
	for (auto it = entities.begin(); it != entities.end(); ++it){
		for (int i = 0; i < (int)it->points.size(); i++) {
			std::cout << it->points[i];
		}
		std::cout << std::endl;
  }
}
