#include "../Header Files/ChunkMesh.h"

ChunkMesh::ChunkMesh(Chunk* chunk) {
	this->chunk = chunk;
	mesh = new Mesh();
	p = glm::ivec3(chunk->x, chunk->y, chunk->z);
}

ChunkMesh::~ChunkMesh() {
	delete mesh;
}

void ChunkMesh::generate() {
	std::vector<GLfloat>* vertices = mesh->GetMesh();
	for (int x = 0; x < c_length; x++) {
		for (int y = 0; y < c_height; y++) {
			for (int z = 0; z < c_width; z++) {
				createCubeMesh(vertices, x, y, z);
			}
		}
	}
	mesh->GenVAO();
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

void ChunkMesh::createCubeMesh(std::vector<GLfloat>* vertices, int x, int y, int z) {
	if (chunk == nullptr) {
		std::cout << "WHAT\n";
	}
	glm::vec3 pos = glm::vec3(chunk->x * c_length + x, chunk->y * c_height + y, chunk->z * c_width + z);
	std::array<std::array<glm::vec2, 2>, 6> block_uvs = BlockTypes::getBlockUVs(chunk->at(x, y, z)->id);
	if (chunk->at(x, y, z)->id == 0)
		return;
	// front
	if ((z > 0 && chunk->at(x, y, z - 1)->id == 0) || (adj_chunks[0] != nullptr && true)) {
		float front[] = {
			pos.x - 0.5, pos.y - 0.5, pos.z - 0.5, block_uvs[0][0].x, block_uvs[0][1].y, // bottom left
			pos.x + 0.5, pos.y + 0.5, pos.z - 0.5, block_uvs[0][1].x, block_uvs[0][0].y, // top right
			pos.x - 0.5, pos.y + 0.5, pos.z - 0.5, block_uvs[0][0].x, block_uvs[0][0].y, // top left
			pos.x - 0.5, pos.y - 0.5, pos.z - 0.5, block_uvs[0][0].x, block_uvs[0][1].y, // bottom left
			pos.x + 0.5, pos.y - 0.5, pos.z - 0.5, block_uvs[0][1].x, block_uvs[0][1].y, // bottom right
			pos.x + 0.5, pos.y + 0.5, pos.z - 0.5, block_uvs[0][1].x, block_uvs[0][0].y, // top right
		};
		mesh->GetMesh()->insert(mesh->GetMesh()->begin(), std::begin(front), std::end(front));
	}
	// back
	if ((z < 16 && chunk->at(x, y, z + 1)->id == 0) || (adj_chunks[1] != nullptr && true)) {
		float back[] = {
			pos.x + 0.5, pos.y - 0.5, pos.z + 0.5, block_uvs[1][0].x, block_uvs[1][1].y, // bottom left
			pos.x - 0.5, pos.y + 0.5, pos.z + 0.5, block_uvs[1][1].x, block_uvs[1][0].y, // top right
			pos.x + 0.5, pos.y + 0.5, pos.z + 0.5, block_uvs[1][0].x, block_uvs[1][0].y, // top left
			pos.x + 0.5, pos.y - 0.5, pos.z + 0.5, block_uvs[1][0].x, block_uvs[1][1].y, // bottom left
			pos.x - 0.5, pos.y - 0.5, pos.z + 0.5, block_uvs[1][1].x, block_uvs[1][1].y, // bottom right
			pos.x - 0.5, pos.y + 0.5, pos.z + 0.5, block_uvs[1][1].x, block_uvs[1][0].y, // top right
		};
		mesh->GetMesh()->insert(mesh->GetMesh()->begin(), std::begin(back), std::end(back));
	}
	// left
	if ((x > 0 && chunk->at(x - 1, y, z)->id == 0) || (adj_chunks[2] != nullptr && true)) {
		float left[] = {
			pos.x - 0.5, pos.y - 0.5, pos.z + 0.5, block_uvs[2][0].x, block_uvs[2][1].y, // bottom left
			pos.x - 0.5, pos.y + 0.5, pos.z - 0.5, block_uvs[2][1].x, block_uvs[2][0].y, // top right
			pos.x - 0.5, pos.y + 0.5, pos.z + 0.5, block_uvs[2][0].x, block_uvs[2][0].y, // top left
			pos.x - 0.5, pos.y - 0.5, pos.z + 0.5, block_uvs[2][0].x, block_uvs[2][1].y, // bottom left
			pos.x - 0.5, pos.y - 0.5, pos.z - 0.5, block_uvs[2][1].x, block_uvs[2][1].y, // bottom right
			pos.x - 0.5, pos.y + 0.5, pos.z - 0.5, block_uvs[2][1].x, block_uvs[2][0].y, // top right
		};
		mesh->GetMesh()->insert(mesh->GetMesh()->begin(), std::begin(left), std::end(left));
	}
	// right
	if ((x < 15 && chunk->at(x + 1, y, z)->id == 0) || (adj_chunks[3] != nullptr && true)) {
		float right[] = {
			pos.x + 0.5, pos.y - 0.5, pos.z - 0.5, block_uvs[3][0].x, block_uvs[3][1].y, // bottom left
			pos.x + 0.5, pos.y + 0.5, pos.z + 0.5, block_uvs[3][1].x, block_uvs[3][0].y, // top right
			pos.x + 0.5, pos.y + 0.5, pos.z - 0.5, block_uvs[3][0].x, block_uvs[3][0].y, // top left
			pos.x + 0.5, pos.y - 0.5, pos.z - 0.5, block_uvs[3][0].x, block_uvs[3][1].y, // bottom left
			pos.x + 0.5, pos.y - 0.5, pos.z + 0.5, block_uvs[3][1].x, block_uvs[3][1].y, // bottom right
			pos.x + 0.5, pos.y + 0.5, pos.z + 0.5, block_uvs[3][1].x, block_uvs[3][0].y, // top right
		};
		mesh->GetMesh()->insert(mesh->GetMesh()->begin(), std::begin(right), std::end(right));
	}
	// top
	if ((y < 15 && chunk->at(x, y + 1, z)->id == 0) || (adj_chunks[4] != nullptr && adj_chunks)) {
		float top[] = {
			pos.x - 0.5, pos.y + 0.5, pos.z - 0.5, block_uvs[4][0].x, block_uvs[4][1].y, // bottom left
			pos.x + 0.5, pos.y + 0.5, pos.z + 0.5, block_uvs[4][1].x, block_uvs[4][0].y, // top right
			pos.x - 0.5, pos.y + 0.5, pos.z + 0.5, block_uvs[4][0].x, block_uvs[4][0].y, // top left
			pos.x - 0.5, pos.y + 0.5, pos.z - 0.5, block_uvs[4][0].x, block_uvs[4][1].y, // bottom left
			pos.x + 0.5, pos.y + 0.5, pos.z - 0.5, block_uvs[4][1].x, block_uvs[4][1].y, // bottom right
			pos.x + 0.5, pos.y + 0.5, pos.z + 0.5, block_uvs[4][1].x, block_uvs[4][0].y, // top right
		};
		mesh->GetMesh()->insert(mesh->GetMesh()->begin(), std::begin(top), std::end(top));
	}
	// bottom
	if ((y > 0 && chunk->at(x, y - 1, z )->id == 0) || (adj_chunks[5] != nullptr && adj_chunks)) {
		float bottom[] = {
			pos.x - 0.5, pos.y - 0.5, pos.z + 0.5, block_uvs[5][0].x, block_uvs[5][1].y, // bottom left
			pos.x + 0.5, pos.y - 0.5, pos.z - 0.5, block_uvs[5][1].x, block_uvs[5][0].y, // top right
			pos.x - 0.5, pos.y - 0.5, pos.z - 0.5, block_uvs[5][0].x, block_uvs[5][0].y, // top left
			pos.x - 0.5, pos.y - 0.5, pos.z + 0.5, block_uvs[5][0].x, block_uvs[5][1].y, // bottom left
			pos.x + 0.5, pos.y - 0.5, pos.z + 0.5, block_uvs[5][1].x, block_uvs[5][1].y, // bottom right
			pos.x + 0.5, pos.y - 0.5, pos.z - 0.5, block_uvs[5][1].x, block_uvs[5][0].y, // top right
		};
		mesh->GetMesh()->insert(mesh->GetMesh()->begin(), std::begin(bottom), std::end(bottom));
	}
}
