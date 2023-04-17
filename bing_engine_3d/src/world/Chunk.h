#ifndef CHUNK_H
#define CHUNK_H

#include "../graphics/block/Block.h"

class Chunk {
public:
    static const int CHUNK_SIZE = 16;
    static const int NUM_BLOCKS = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

    Block blocks[NUM_BLOCKS];

    Chunk(glm::vec3 pos) : pos(pos) {
        // Initialize blocks
        for (int x = 0; x < CHUNK_SIZE; x++) {
            for (int y = 0; y < CHUNK_SIZE; y++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    int index = x * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + z;
                    glm::vec3 blockPos = glm::vec3(x, y, z) + pos;
                    blocks[index] = Block(blockPos);
                }
            }
        }
    }

    void draw(Shader shader) {
        for (int i = 0; i < NUM_BLOCKS; i++) {
            blocks[i].render(shader);
        }
    }

private:
    glm::vec3 pos;
};

#endif