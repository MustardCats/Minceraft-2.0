#include "../Header Files/Renderer.h"

namespace Renderer {

	static Shader* basic_shader = nullptr;
	static Shader* world_shader = nullptr;

	static Mesh* test = nullptr;

	static TextureAtlas* world_atlas = nullptr;

	bool init() {
		std::string vert_path = "Assets/Shaders/BasicShader.vert";
		std::string frag_path = "Assets/Shaders/BasicShader.frag";
		basic_shader = new Shader(vert_path, frag_path);

		vert_path = "Assets/Shaders/WorldShader.vert";
		frag_path = "Assets/Shaders/WorldShader.frag";
		world_shader = new Shader(vert_path, frag_path);

		test = new Mesh();
		//test->AddRect(glm::vec2(-0.5), glm::vec2(1), glm::vec2(0.0), glm::vec2(1.0));
		float vertices[] = {
			// front
			-0.5, -0.5, -0.5, 0.0, 1.0, // bottom left
			0.5, 0.5, -0.5, 1.0, 0.0, // top right
			-0.5, 0.5, -0.5, 0.0, 0.0, // top left
			-0.5, -0.5, -0.5, 0.0, 1.0, // bottom left
			0.5, -0.5, -0.5, 1.0, 1.0, // bottom right
			0.5, 0.5, -0.5, 1.0, 0.0, // top right
			// back
			0.5, -0.5, 0.5, 0.0, 1.0,
			-0.5, 0.5, 0.5, 1.0, 0.0,
			0.5, 0.5, 0.5, 0.0, 0.0,
			0.5, -0.5, 0.5, 0.0, 1.0,
			-0.5, -0.5, 0.5, 1.0, 1.0,
			-0.5, 0.5, 0.5, 1.0, 0.0,
			// left
			-0.5, -0.5, 0.5, 0.0, 1.0,
			-0.5, 0.5, -0.5, 1.0, 0.0,
			-0.5, 0.5, 0.5, 0.0, 0.0,
			-0.5, -0.5, 0.5, 0.0, 1.0,
			-0.5, -0.5, -0.5, 1.0, 1.0,
			-0.5, 0.5, -0.5, 1.0, 0.0,
			// right
			0.5, -0.5, -0.5, 0.0, 1.0,
			0.5, 0.5, 0.5, 1.0, 0.0,
			0.5, 0.5, -0.5, 0.0, 0.0,
			0.5, -0.5, -0.5, 0.0, 1.0,
			0.5, -0.5, 0.5, 1.0, 1.0,
			0.5, 0.5, 0.5, 1.0, 0.0,
			// top
			-0.5, 0.5, -0.5, 0.0, 1.0,
			0.5, 0.5, 0.5, 1.0, 0.0,
			-0.5, 0.5, 0.5, 0.0, 0.0,
			-0.5, 0.5, -0.5, 0.0, 1.0,
			0.5, 0.5, -0.5, 1.0, 1.0,
			0.5, 0.5, 0.5, 1.0, 0.0,
			// bottom
			-0.5, -0.5, 0.5, 0.0, 1.0,
			0.5, -0.5, -0.5, 1.0, 0.0,
			-0.5, -0.5, -0.5, 0.0, 0.0,
			-0.5, -0.5, 0.5, 0.0, 1.0,
			0.5, -0.5, 0.5, 1.0, 1.0,
			0.5, -0.5, -0.5, 1.0, 0.0
		};
		test->GetMesh()->insert(test->GetMesh()->end(), std::begin(vertices), std::end(vertices));
		//test->GetMesh()->insert(test->GetMesh()->end(), std::begin(mesh), std::end(mesh));

		test->GenVAO();

		std::string world_atlas_path = "Assets/Textures/epic_win.png";
		world_atlas = new TextureAtlas(world_atlas_path);
		world_atlas->addTexture("Assets/Textures/epic_win.png");

		return true;
	}

	void render() {
		world_shader->use();
		
		glm::mat4 matrix = Camera::getMatrix();
		world_shader->setMat4("MVP", matrix);
		world_shader->setVec3("color", glm::vec3(1.0, 1.0, 1.0));
		world_atlas->Use();

		test->Render();
		return;
		basic_shader->use();
		glm::mat4 ortho = glm::ortho(0.0f, (float)(Window::getSize().x), (float)(Window::getSize().y), 0.0f);
		basic_shader->setMat4("MVP", ortho);
		Mesh cat;
		cat.AddRect(glm::vec2(0.0f, 0.0f), glm::vec2(300.0f, 300.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 1.0f));
		cat.GenVAO();
		//cat.Render();
	}
}