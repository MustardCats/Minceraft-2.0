#pragma once

#include "Entity.h"
#include "Inventory.h"
#include "../Assets/BlockTypes.h"
#include "../Physics/Ray.h"
#include "../Render/Camera.h"
#include "../World/ChunkManager.h"

class Player : public Entity {
public:
	Player();
	~Player();
	void update(float delta_time);
	void tryBreakBlock();
	void tryPlaceBlock();
	Inventory* getInventory();
	bool isInvOpen();
	void toggleInv();
	void leftClickInv(glm::ivec2 mouse);
	void rightClickInv(glm::ivec2 mouse);
private:
	Ray* ray;
	bool facing_block;
	bool inside_block;
	glm::ivec3 block_pos;
	Inventory inv;
	Item cursor_item;
	bool inv_open;
};