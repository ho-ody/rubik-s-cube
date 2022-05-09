#pragma once
#include <glm/glm.hpp>

class Tile
{
public:
	glm::vec3 position; //relative to center
	glm::vec3 color;
	
	Tile(glm::vec3 = glm::vec3(0.0, 0.0, 0.0), glm::vec3 = glm::vec3(0.3,0.3,0.3));
};
