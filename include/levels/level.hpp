#pragma once

typedef struct {
  std::vector<int> points;
} Entity;

class Level {
public:
  std::vector<Entity> *entities;

	Level();
  Level(Entity entity);
	~Level();
	
	printLevelDeets();
};