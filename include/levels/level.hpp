#pragma once

#include <vector>


struct Entity {
  std::vector<int> points;
};

class Level {
public:
  std::vector<Entity> entities;

	Level();
	Level(std::vector<Entity> entities);
	~Level();
	
	void printLevelDeets();
};
