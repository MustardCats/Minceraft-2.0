#pragma once

#include "Include.h"
#include "ChunkManager.h"
#include "Mesh.h"
#include "BlockTypes.h"

class ChunkMesh {
public:
	ChunkMesh(Chunk* chunk);
	~ChunkMesh();
	bool generate(int num_rows);
	Chunk* getChunk();
	void render();
	glm::ivec3 pos();
	void setAdjChunk(Chunk* mesh, int index);
	void clearAdjChunk(int index);
	void clear();
	Mesh* getMesh();
	bool isGenerated();
	void findAdjacentChunks();
private:
	void createCubeMesh(std::vector<GLfloat>* vertices, int x, int y, int z);

	Chunk* chunk;
	Chunk* adj_chunks[6];
	Mesh* mesh;
	glm::ivec3 p;
	bool is_generated;
	int cur_generating_y;
};