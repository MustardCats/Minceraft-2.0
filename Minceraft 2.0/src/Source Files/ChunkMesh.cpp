#include "../Header Files/ChunkMesh.h"

ChunkMesh::ChunkMesh(Chunk* chunk) {
	this->chunk = chunk;
	mesh = new Mesh();
	p = glm::ivec3(chunk->x, chunk->y, chunk->z);
	for (int i = 0; i < 6; i++) {
		adj_chunks[i] = nullptr;
	}
	is_generated = false;
	cur_generating_y = 0;
}

ChunkMesh::~ChunkMesh() {
	delete mesh;
}

bool ChunkMesh::generate(int num_rows) {
	is_generated = false;
	std::vector<GLfloat>* vertices = mesh->GetMesh();
	findAdjacentChunks();
	for (int x = 0; x < c_length; x++) {
		for (int y = cur_generating_y; y < cur_generating_y + num_rows; y++) {
			for (int z = 0; z < c_width; z++) {
				createCubeMesh(vertices, x, y, z);
			}
		}
	}
	// don't need these
	for (int i = 0; i < 6; i++) {
		adj_chunks[i] = nullptr;
	}
	cur_generating_y += num_rows;
	if (cur_generating_y >= c_height) {
		cur_generating_y = 0;
		mesh->GenVAO();
		is_generated = true;
		return true;
	}
	return false;
}

Chunk* ChunkMesh::getChunk() {
	return chunk;
}

void ChunkMesh::render() {
	mesh->Render();
}

glm::ivec3 ChunkMesh::pos() {
	return p;
}

void ChunkMesh::setAdjChunk(Chunk* mesh, int index) {
	adj_chunks[index] = mesh;
}

void ChunkMesh::clearAdjChunk(int index) {
	adj_chunks[index] = nullptr;
}

void ChunkMesh::clear() {
	mesh->Clear();
	is_generated = false;
	cur_generating_y = 0;
}

Mesh* ChunkMesh::getMesh() {
	return mesh;
}

void ChunkMesh::createCubeMesh(std::vector<GLfloat>* vertices, int x, int y, int z) {
	if (chunk == nullptr) {
		std::cout << "This mesh's chunk is a nullptr when it shouldn't be!\n";
		return;
	}
	if (chunk->at(x, y, z)->id == 0)
		return;
	glm::vec3 pos = glm::vec3(chunk->x * c_length + x, chunk->y * c_height + y, chunk->z * c_width + z);
	//std::cout << chunk->at(x, y, z)->id << "\n";
	std::array<std::array<glm::vec2, 2>, 6> block_uvs = BlockTypes::getBlockUVs(chunk->at(x, y, z)->id);
	bool create = false;
	// front
	if (z > 0) {
		if (chunk->at(x, y, z - 1)->id == 0) {
			create = true;
		}
	}
	else if (adj_chunks[0] != nullptr) {
		if (adj_chunks[0]->at(x, y, c_width - 1)->id == 0) {
			create = true;
		}
	}
	if (create) {
		float front[] = {
			pos.x - 0.5, pos.y - 0.5, pos.z - 0.5, block_uvs[0][0].x, block_uvs[0][1].y, // bottom left
			pos.x + 0.5, pos.y + 0.5, pos.z - 0.5, block_uvs[0][1].x, block_uvs[0][0].y, // top right
			pos.x - 0.5, pos.y + 0.5, pos.z - 0.5, block_uvs[0][0].x, block_uvs[0][0].y, // top left
			pos.x - 0.5, pos.y - 0.5, pos.z - 0.5, block_uvs[0][0].x, block_uvs[0][1].y, // bottom left
			pos.x + 0.5, pos.y - 0.5, pos.z - 0.5, block_uvs[0][1].x, block_uvs[0][1].y, // bottom right
			pos.x + 0.5, pos.y + 0.5, pos.z - 0.5, block_uvs[0][1].x, block_uvs[0][0].y, // top right
		};
		mesh->GetMesh()->insert(mesh->GetMesh()->begin(), std::begin(front), std::end(front));
		create = false;
	}
	// back
	if (z < (c_width - 1)) {
		if (chunk->at(x, y, z + 1)->id == 0) {
			create = true;
		}
	}
	else if (adj_chunks[1] != nullptr) {
		if (adj_chunks[1]->at(x, y, 0)->id == 0) {
			create = true;
		}
	}
	if (create) {
		float back[] = {
			pos.x + 0.5, pos.y - 0.5, pos.z + 0.5, block_uvs[1][0].x, block_uvs[1][1].y, // bottom left
			pos.x - 0.5, pos.y + 0.5, pos.z + 0.5, block_uvs[1][1].x, block_uvs[1][0].y, // top right
			pos.x + 0.5, pos.y + 0.5, pos.z + 0.5, block_uvs[1][0].x, block_uvs[1][0].y, // top left
			pos.x + 0.5, pos.y - 0.5, pos.z + 0.5, block_uvs[1][0].x, block_uvs[1][1].y, // bottom left
			pos.x - 0.5, pos.y - 0.5, pos.z + 0.5, block_uvs[1][1].x, block_uvs[1][1].y, // bottom right
			pos.x - 0.5, pos.y + 0.5, pos.z + 0.5, block_uvs[1][1].x, block_uvs[1][0].y, // top right
		};
		mesh->GetMesh()->insert(mesh->GetMesh()->begin(), std::begin(back), std::end(back));
		create = false;
	}
	// left
	if (x > 0) {
		if (chunk->at(x - 1, y, z)->id == 0) {
			create = true;
		}
	}
	else if (adj_chunks[2] != nullptr) {
		if (adj_chunks[2]->at(c_length - 1, y, z)->id == 0) {
			create = true;
		}
	}
	if (create) {
		float left[] = {
			pos.x - 0.5, pos.y - 0.5, pos.z + 0.5, block_uvs[2][0].x, block_uvs[2][1].y, // bottom left
			pos.x - 0.5, pos.y + 0.5, pos.z - 0.5, block_uvs[2][1].x, block_uvs[2][0].y, // top right
			pos.x - 0.5, pos.y + 0.5, pos.z + 0.5, block_uvs[2][0].x, block_uvs[2][0].y, // top left
			pos.x - 0.5, pos.y - 0.5, pos.z + 0.5, block_uvs[2][0].x, block_uvs[2][1].y, // bottom left
			pos.x - 0.5, pos.y - 0.5, pos.z - 0.5, block_uvs[2][1].x, block_uvs[2][1].y, // bottom right
			pos.x - 0.5, pos.y + 0.5, pos.z - 0.5, block_uvs[2][1].x, block_uvs[2][0].y, // top right
		};
		mesh->GetMesh()->insert(mesh->GetMesh()->begin(), std::begin(left), std::end(left));
		create = false;
	}
	// right
	if (x < (c_length - 1)) {
		if (chunk->at(x + 1, y, z)->id == 0) {
			create = true;
		}
	}
	else if (adj_chunks[3] != nullptr) {
		if (adj_chunks[3]->at(0, y, z)->id == 0) {
			create = true;
		}
	}
	if (create) {
		float right[] = {
			pos.x + 0.5, pos.y - 0.5, pos.z - 0.5, block_uvs[3][0].x, block_uvs[3][1].y, // bottom left
			pos.x + 0.5, pos.y + 0.5, pos.z + 0.5, block_uvs[3][1].x, block_uvs[3][0].y, // top right
			pos.x + 0.5, pos.y + 0.5, pos.z - 0.5, block_uvs[3][0].x, block_uvs[3][0].y, // top left
			pos.x + 0.5, pos.y - 0.5, pos.z - 0.5, block_uvs[3][0].x, block_uvs[3][1].y, // bottom left
			pos.x + 0.5, pos.y - 0.5, pos.z + 0.5, block_uvs[3][1].x, block_uvs[3][1].y, // bottom right
			pos.x + 0.5, pos.y + 0.5, pos.z + 0.5, block_uvs[3][1].x, block_uvs[3][0].y, // top right
		};
		mesh->GetMesh()->insert(mesh->GetMesh()->begin(), std::begin(right), std::end(right));
		create = false;
	}
	// top
	if (y < (c_height - 1)) {
		if (chunk->at(x, y + 1, z)->id == 0) {
			create = true;
		}
	}
	else if (adj_chunks[4] != nullptr) {
		if (adj_chunks[4]->at(x, 0, z)->id == 0) {
			create = true;
		}
	}
	if (create) {
		float top[] = {
			pos.x - 0.5, pos.y + 0.5, pos.z - 0.5, block_uvs[4][0].x, block_uvs[4][1].y, // bottom left
			pos.x + 0.5, pos.y + 0.5, pos.z + 0.5, block_uvs[4][1].x, block_uvs[4][0].y, // top right
			pos.x - 0.5, pos.y + 0.5, pos.z + 0.5, block_uvs[4][0].x, block_uvs[4][0].y, // top left
			pos.x - 0.5, pos.y + 0.5, pos.z - 0.5, block_uvs[4][0].x, block_uvs[4][1].y, // bottom left
			pos.x + 0.5, pos.y + 0.5, pos.z - 0.5, block_uvs[4][1].x, block_uvs[4][1].y, // bottom right
			pos.x + 0.5, pos.y + 0.5, pos.z + 0.5, block_uvs[4][1].x, block_uvs[4][0].y, // top right
		};
		mesh->GetMesh()->insert(mesh->GetMesh()->begin(), std::begin(top), std::end(top));
		create = false;
	}
	// bottom
	if (y > 0) {
		if (chunk->at(x, y - 1, z)->id == 0) {
			create = true;
		}
	}
	else if (adj_chunks[5] != nullptr) {
		if (adj_chunks[5]->at(x, c_height - 1, z)->id == 0) {
			create = true;
		}
	}
	if (create) {
		float bottom[] = {
			pos.x - 0.5, pos.y - 0.5, pos.z + 0.5, block_uvs[5][0].x, block_uvs[5][1].y, // bottom left
			pos.x + 0.5, pos.y - 0.5, pos.z - 0.5, block_uvs[5][1].x, block_uvs[5][0].y, // top right
			pos.x - 0.5, pos.y - 0.5, pos.z - 0.5, block_uvs[5][0].x, block_uvs[5][0].y, // top left
			pos.x - 0.5, pos.y - 0.5, pos.z + 0.5, block_uvs[5][0].x, block_uvs[5][1].y, // bottom left
			pos.x + 0.5, pos.y - 0.5, pos.z + 0.5, block_uvs[5][1].x, block_uvs[5][1].y, // bottom right
			pos.x + 0.5, pos.y - 0.5, pos.z - 0.5, block_uvs[5][1].x, block_uvs[5][0].y, // top right
		};
		mesh->GetMesh()->insert(mesh->GetMesh()->begin(), std::begin(bottom), std::end(bottom));
		create = false;
	}
}

void ChunkMesh::findAdjacentChunks() {
	adj_chunks[0] = ChunkManager::getChunk(p.x, p.y, p.z - 1);
	adj_chunks[1] = ChunkManager::getChunk(p.x, p.y, p.z + 1);
	adj_chunks[2] = ChunkManager::getChunk(p.x - 1, p.y, p.z);
	adj_chunks[3] = ChunkManager::getChunk(p.x + 1, p.y, p.z);
	adj_chunks[4] = ChunkManager::getChunk(p.x, p.y + 1, p.z);
	adj_chunks[5] = ChunkManager::getChunk(p.x, p.y - 1, p.z);
}

bool ChunkMesh::isGenerated() {
	return is_generated;
}