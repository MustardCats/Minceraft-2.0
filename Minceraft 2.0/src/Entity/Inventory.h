#pragma once

#include "../Include.h"
#include "../Assets/Item.h"

class Inventory {
public:
	Inventory(int size);
	Item add(Item item);
	Item remove(int index);
	void move(Inventory& other, int index);
	void swap(Item& other, int index);
	std::vector<Item>* getItems();
	size_t size();
private:
	std::vector<Item> items;
};