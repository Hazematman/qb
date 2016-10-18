#pragma once

#include <vector>
#include <string>

#include "physics/Objects.hpp"


class Level : public Objects {
	
	void writeLevelData(std::string path);

public:
	Level();
	~Level();

	
};
