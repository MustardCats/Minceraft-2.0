#include "Renderer.h"

namespace Renderer {

	static Shader* basic_shader = nullptr;
	static Shader* chunk_shader = nullptr;
	static Shader* color_shader = nullptr;
	
	bool init() {
		std::string vert_path = "Assets/Shaders/BasicShader.vert";
		std::string frag_path = "Assets/Shaders/BasicShader.frag";
		basic_shader = new Shader(vert_path, frag_path);

		vert_path = "Assets/Shaders/WorldShader.vert";
		frag_path = "Assets/Shaders/WorldShader.frag";
		chunk_shader = new Shader(vert_path, frag_path);

		vert_path = "Assets/Shaders/ColorShader.vert";
		frag_path = "Assets/Shaders/ColorShader.frag";
		color_shader = new Shader(vert_path, frag_path);

		return true;
	}

	void render() {
		chunk_shader->use();
		glm::mat4 matrix = Camera::getMatrix();
		chunk_shader->setMat4("MVP", matrix);
		BlockTypes::getAtlas()->Use();
		ChunkRenderer::render();
		
		color_shader->use();
		color_shader->setMat4("MVP", matrix);
		DebugRenderer::render();

		glDisable(GL_DEPTH_TEST);
		basic_shader->use();
		glm::mat4 ortho_matrix = glm::ortho(0.0f, (float)Window::getSize().x, (float)Window::getSize().y, 0.0f);
		basic_shader->setMat4("MVP", ortho_matrix);
		UIRenderer::render();
		glEnable(GL_DEPTH_TEST);
	}
}