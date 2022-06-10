#pragma once

#include "../Include.h"
#include "Camera.h"
#include "ChunkRenderer.h"
#include "DebugRenderer.h"
#include "UIRenderer.h"
#include "../Assets/BlockTypes.h"
#include "../Render/Mesh/Mesh.h"
#include "../Window/Shader.h"
#include "../Window/Window.h"
#include "../World/Chunk.h"
#include "../World/ChunkManager.h"

namespace Renderer {
	bool init();
	void render();
}