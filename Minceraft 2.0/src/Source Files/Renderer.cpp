#include "../Header Files/Renderer.h"

namespace Renderer {

	static Shader* basic_shader = nullptr;
	static Shader* world_shader = nullptr;

	static Mesh* test = nullptr;
	
	bool init() {
		std::string vert_path = "Assets/Shaders/BasicShader.vert";
		std::string frag_path = "Assets/Shaders/BasicShader.frag";
		basic_shader = new Shader(vert_path, frag_path);

		vert_path = "Assets/Shaders/WorldShader.vert";
		frag_path = "Assets/Shaders/WorldShader.frag";
		world_shader = new Shader(vert_path, frag_path);

		test = new Mesh();
		//test->AddRect(glm::vec2(-0.5), glm::vec2(1), glm::vec2(0.0), glm::vec2(1.0));
		std::array<std::array<glm::vec2, 2>, 6> block_uvs = BlockTypes::getBlockUVs(4);
		float vertices[] = {
			// front
			-0.5, -0.5, -0.5, block_uvs[0][0].x, block_uvs[0][1].y, // bottom left
			0.5, 0.5, -0.5, block_uvs[0][1].x, block_uvs[0][0].y, // top right
			-0.5, 0.5, -0.5, block_uvs[0][0].x, block_uvs[0][0].y, // top left
			-0.5, -0.5, -0.5, block_uvs[0][0].x, block_uvs[0][1].y, // bottom left
			0.5, -0.5, -0.5, block_uvs[0][1].x, block_uvs[0][1].y, // bottom right
			0.5, 0.5, -0.5, block_uvs[0][1].x, block_uvs[0][0].y, // top right
			// back
			0.5, -0.5, 0.5, block_uvs[1][0].x, block_uvs[1][1].y,
			-0.5, 0.5, 0.5, block_uvs[1][1].x, block_uvs[1][0].y,
			0.5, 0.5, 0.5, block_uvs[1][0].x, block_uvs[1][0].y,
			0.5, -0.5, 0.5, block_uvs[1][0].x, block_uvs[1][1].y,
			-0.5, -0.5, 0.5, block_uvs[1][1].x, block_uvs[1][1].y,
			-0.5, 0.5, 0.5, block_uvs[1][1].x, block_uvs[1][0].y,
			// left
			-0.5, -0.5, 0.5, block_uvs[2][0].x, block_uvs[2][1].y,
			-0.5, 0.5, -0.5, block_uvs[2][1].x, block_uvs[2][0].y,
			-0.5, 0.5, 0.5, block_uvs[2][0].x, block_uvs[2][0].y,
			-0.5, -0.5, 0.5, block_uvs[2][0].x, block_uvs[2][1].y,
			-0.5, -0.5, -0.5, block_uvs[2][1].x, block_uvs[2][1].y,
			-0.5, 0.5, -0.5, block_uvs[2][1].x, block_uvs[2][0].y,
			// right
			0.5, -0.5, -0.5, block_uvs[3][0].x, block_uvs[3][1].y,
			0.5, 0.5, 0.5, block_uvs[3][1].x, block_uvs[3][0].y,
			0.5, 0.5, -0.5, block_uvs[3][0].x, block_uvs[3][0].y,
			0.5, -0.5, -0.5, block_uvs[3][0].x, block_uvs[3][1].y,
			0.5, -0.5, 0.5, block_uvs[3][1].x, block_uvs[3][1].y,
			0.5, 0.5, 0.5, block_uvs[3][1].x, block_uvs[3][0].y,
			// top
			-0.5, 0.5, -0.5, block_uvs[4][0].x, block_uvs[4][1].y,
			0.5, 0.5, 0.5, block_uvs[4][1].x, block_uvs[4][0].y,
			-0.5, 0.5, 0.5, block_uvs[4][0].x, block_uvs[4][0].y,
			-0.5, 0.5, -0.5, block_uvs[4][0].x, block_uvs[4][1].y,
			0.5, 0.5, -0.5, block_uvs[4][1].x, block_uvs[4][1].y,
			0.5, 0.5, 0.5, block_uvs[4][1].x, block_uvs[4][0].y,
			// bottom
			-0.5, -0.5, 0.5, block_uvs[5][0].x, block_uvs[5][1].y,
			0.5, -0.5, -0.5, block_uvs[5][1].x, block_uvs[5][0].y,
			-0.5, -0.5, -0.5, block_uvs[5][0].x, block_uvs[5][0].y,
			-0.5, -0.5, 0.5, block_uvs[5][0].x, block_uvs[5][1].y,
			0.5, -0.5, 0.5, block_uvs[5][1].x, block_uvs[5][1].y,
			0.5, -0.5, -0.5, block_uvs[5][1].x, block_uvs[5][0].y
		};
		test->GetMesh()->insert(test->GetMesh()->end(), std::begin(vertices), std::end(vertices));
		//test->GetMesh()->insert(test->GetMesh()->end(), std::begin(mesh), std::end(mesh));

		test->GenVAO();

		return true;
	}

	void render() {
		glEnable(GL_DEPTH_TEST);
		world_shader->use();
		
		glm::mat4 matrix = Camera::getMatrix();
		world_shader->setMat4("MVP", matrix);
		
		static float color = 0.0f;
		color += 0.001f;
		if (color >= 0.7f) {
			color = 0.0f;
		}

		world_shader->setVec3("shadeColor", glm::vec3(1.0f, 1.0f, 1.0f));
		BlockTypes::getAtlas()->Use();

		test->Render();
		ChunkRenderer::render();

		glDisable(GL_DEPTH_TEST);
		basic_shader->use();
		glm::mat4 ortho = glm::ortho(0.0f, (float)(Window::getSize().x), (float)(Window::getSize().y), 0.0f);
		basic_shader->setMat4("MVP", ortho);
		Mesh cat;
		cat.AddRect(glm::vec2(0.0f, 0.0f), glm::vec2(300.0f, 300.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.125f, 0.125f));
		cat.GenVAO();
		//cat.Render();
	}
}