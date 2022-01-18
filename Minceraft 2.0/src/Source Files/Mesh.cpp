#include "../Header Files/Mesh.h"

Mesh::Mesh() {

}

Mesh::~Mesh() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}

void Mesh::Render() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, mesh.size() * 0.6f);
}

void Mesh::GenVAO() {
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh.size(), mesh.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		5 * sizeof(GLfloat),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		5 * sizeof(float),                // stride
		(void*)(3 * sizeof(GLfloat))        // array buffer offset
	);
	glEnableVertexAttribArray(1);
}

std::vector<GLfloat>* Mesh::GetMesh() {
	return &mesh;
}

void Mesh::AddRect(glm::vec2 pos, glm::vec2 size, glm::vec2 uv_pos, glm::vec2 uv_size) {
	float new_mesh[] = {
		pos.x, pos.y + size.y, 0.0f, uv_pos.x, uv_pos.y + uv_size.y, // bottom left
		pos.x + size.x, pos.y + size.y, 0.0f, uv_pos.x + uv_size.x, uv_pos.y + uv_size.y, // bottom right
		pos.x + size.x, pos.y, 0.0f, uv_pos.x + uv_size.x, uv_pos.y, // top right
		pos.x, pos.y + size.y, 0.0f, uv_pos.x, uv_pos.y + uv_size.y, // bottom left
		pos.x + size.x, pos.y, 0.0f, uv_pos.x + uv_size.x, uv_pos.y, // top right
		pos.x, pos.y, 0.0f, uv_pos.x, uv_pos.y, // top left
	};
	mesh.insert(mesh.end(), std::begin(new_mesh), std::end(new_mesh));
}

void Mesh::Clear() {
	mesh.clear();
}