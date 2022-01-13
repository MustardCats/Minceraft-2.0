#pragma once

#include "Include.h"

class TextureAtlas {
public:
	TextureAtlas();
	TextureAtlas(std::string file_name);
	void Load(std::string file_name);
	int GetTileSize();
	void SetTileSize(int tile_size);
	int GetTextureSize();
	void SetTextureSize(int texture_size);
	glm::vec2 GetSize();
	glm::vec2 GetUVCoords(int x, int y);
	void Use();
	glm::vec2 GetNextPosition(glm::vec2 cur_pos);
	glm::vec2 GetUVSize();
private:
	int tile_size;
	int texture_size;
	glm::vec2 uv_size;
	GLuint texture_id;
	glm::vec2 dimensions;
};