#pragma once

#include "../Include.h"

struct Item {
	Item(int id, int count) : id(id), count(count)
	{

	}
	int add(int count) {
		this->count += count;
		if (this->count > MAX_COUNT) {
			int remainder = this->count - MAX_COUNT;
			this->count -= remainder;
			return remainder;
		}
		return 0;
	}
	bool operator==(Item other) {
		return this->id == other.id;
	}
	int id;
	int count;
	static int MAX_COUNT;
};

struct ItemNamePair {
	int id;
	std::string name;
};

namespace ItemManager {
	bool init();
	int getID(std::string name);
	std::string getName(int id);
}