#include "Inventory.h"

Inventory::Inventory(int size) {
	Item null_item = Item(0, 0);
	items = std::vector<Item>(size, null_item);
}

Item Inventory::add(Item item) {
	for (int i = 0; i < items.size(); i++) {
		if (items[i].id == 0 || items[i].id == item.id) {
			items[i].id = item.id;
			item.count = items[i].add(item.count);
			if (item.count == 0) {
				return Item(0, 0);
			}
		}
	}
	return item;
}

Item Inventory::remove(int index) {
	Item item = items[index];
	items[index] = Item(0, 0);
	return item;
}

void Inventory::move(Inventory& other, int index) {
	items[index] = other.add(items[index]);
}

void Inventory::swap(Item& other, int index) {
	if (other.id == items[index].id) {
		// simply swap
		if (other.count == Item::MAX_COUNT || items[index].count == Item::MAX_COUNT) {
			int temp = other.count;
			other.count = items[index].count;
			items[index].count = temp;
		}
		// compact items
		else {
			items[index].count = other.add(items[index].count);
			if (items[index].count == 0) {
				items[index] = Item(0, 0);
			}
		}
	}
	else {
		Item temp = other;
		other = items[index];
		items[index] = temp;
	}
}

std::vector<Item>* Inventory::getItems() {
	return &items;
}

size_t Inventory::size() {
	return items.size();
}
