#ifndef OCTREE_H
#define OCTREE_H

#include "graphics/block/Block.h"

#include <vector>

class Octree{
public:
	std::vector<Block> blocks;

	bool has_children = false;
private:
};

#endif

