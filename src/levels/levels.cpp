#import "levels/level.hpp"

Level::Level() {

}

Level::Level(std::vector<Entity> entities;) {
  this->entities = entities;
}

Level::~Level() {

}

Level::printLevelDeets() {
  for (int i = 0; i < this->entities->length(); i++){
    curr_entity = this->entities[i];
      for (int i = 0; i < curr_entity.points.length(); i++){
        std::cout << curr_entity.points[i];
      }
    std::cout << endl;
  }
}
