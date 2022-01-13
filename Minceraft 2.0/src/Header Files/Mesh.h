#pragma once

#include "Include.h"

class Mesh {
public:
	Mesh();
	~Mesh();
	void Render();
	void GenVAO();
	std::vector<GLfloat>* GetMesh();
	void AddRect(glm::vec2 pos, glm::vec2 size, glm::vec2 uv_pos, glm::vec2 uv_size);
	void Clear();
private:
	std::vector<GLfloat> mesh;
	GLuint VAO, VBO;
};