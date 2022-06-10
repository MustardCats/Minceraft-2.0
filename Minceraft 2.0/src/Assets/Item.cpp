#include "Item.h"

int Item::MAX_COUNT = 100;

namespace ItemManager {
	
	static robin_hood::unordered_map<std::string, int> name_to_id;
	static std::vector<std::string> id_to_name;
	
	bool init() {
		return true;
	}

	int getID(std::string name) {
		return name_to_id[name];
	}

	std::string getName(int id) {
		return id_to_name[id];
	}
}