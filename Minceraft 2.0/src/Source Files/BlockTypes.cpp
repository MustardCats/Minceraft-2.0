#include "../Header Files/BlockTypes.h"

namespace BlockTypes {
	
	static std::string asset_directory;
	static std::unordered_map<std::string, std::array<glm::vec2, 2>> face_uvs;
	static std::vector<std::array<std::array<glm::vec2, 2>, 6>> block_uvs;
	static std::unordered_map<std::string, int> name_to_id;
	static std::vector<BlockAttribute> block_attributes;
	static int max_blocks = 0;

	bool loadAttributes() {
		std::ifstream file = std::ifstream();
		file.open(asset_directory + "/BlockTypes/attributes.txt");
		bool finished = true;
		std::string next_tile;
		std::string line;
		// air tile
		block_attributes.push_back(BlockAttribute());
		while (std::getline(file, line)) {
			if (line[0] == ';') {
				finished = true;
				max_blocks++;
				continue;
			}
			if (finished) {
				finished = false;
				next_tile = line;
				block_attributes.push_back(BlockAttribute());
				block_attributes.at(block_attributes.size() - 1).name = next_tile;
				std::cout << "Loading Tile " << next_tile << "\n";
				continue;
			}
			std::string str = "";
			while (line[0] != ' ' && line.size() > 1) {
				str += line[0];
				line.erase(line.begin() + 0);
			}
			line.erase(line.begin() + 0);
			if (str == "id") {
				name_to_id[next_tile] = std::stoi(line);
				block_attributes.at(block_attributes.size() - 1).id = std::stoi(line);
			}
			else if (str == "drops") {
				block_attributes.at(block_attributes.size() - 1).drops = line;
			}
			else if (str == "solid") {
				block_attributes.at(block_attributes.size() - 1).solid = std::stoi(line);
			}
			else if (str == "harvest") {
				block_attributes.at(block_attributes.size() - 1).harvest = std::stoi(line);
			}
			else if (str == "toughness") {
				block_attributes.at(block_attributes.size() - 1).toughness = std::stoi(line);
			}
		}
		file.close();

		return true;
	}

	bool loadUVFaces() {

		return true;
	}

	bool loadBlockUVMappings() {

		return true;
	}

	bool init(std::string asset_directory) {
		BlockTypes::asset_directory = asset_directory;
		// load block attributes
		if (!loadAttributes()) {
			return false;
		}
		// TODO: replace
		// load uv faces
		if (!loadUVFaces()) {
			return false;
		}
		// load mappings of faces to blocks
		if (!loadBlockUVMappings) {
			return false;
		}

		return true;
	}

	std::array<std::array<glm::vec2, 2>, 6> getBlockUVs(int id) {
		return block_uvs[id];
	}
}