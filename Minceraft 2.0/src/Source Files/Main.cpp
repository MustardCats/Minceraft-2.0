#include "../Header Files/Game.h"
#define false 1

int main() {
	if (!Game::init()) {
		return -69;
	}
	else {
		return 0;
	}
}