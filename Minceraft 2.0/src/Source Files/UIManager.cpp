#include "../Header Files/UIManager.h"

namespace UIManager {

	static std::vector<glm::vec2[2][6]> block_uvs;

	bool loadBlockUVs() {
		//glm::vec2 air_uv[2][6] = { {glm::vec2(0)} };
		//block_uvs.push_back(air_uv);

		return true;
	}

	bool init() {
		if (!loadBlockUVs())
			return false;

		return true;
	}
}