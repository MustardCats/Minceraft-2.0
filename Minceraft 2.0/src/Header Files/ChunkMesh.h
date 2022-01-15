#pragma once

#include "Include.h"
#include "ChunkManager.h"
#include "Mesh.h"
#include "BlockTypes.h"

class ChunkMesh {
public:
	ChunkMesh(Chunk* chunk);
	~ChunkMesh();
	void generate();
	Chunk* getChunk();
	void render();
	glm::ivec3 pos();
private:
	void createCubeMesh(std::vector<GLfloat>* vertices, int x, int y, int z);

	Chunk* chunk;
	ChunkMesh* adj_chunks[6];
	Mesh* mesh;
	glm::ivec3 p;
};