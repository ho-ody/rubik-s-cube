#include "Block.h"
#include <iostream>

Block::Block() {
	rotation = glm::vec3(0, 0, 0);
	for (int i = 0; i < 6; i++)
		color[i] = glm::vec3(0.1, 0.1, 0.1);
}