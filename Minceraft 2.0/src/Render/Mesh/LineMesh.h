#pragma once

#include "../../Include.h"
#include "Mesh.h"
#include "../../Physics/Shape.h"

struct LineVertex {
	LineVertex(glm::vec3 pos, glm::vec3 color)
		: x(pos.x), y(pos.y), z(pos.z), r(color.r), g(color.g), b(color.b) 
	{

	}
	float x, y, z;
	float r, g, b;
};

class LineMesh : public Mesh<LineVertex> {
public:
	void render();
	void addLine(glm::vec3 start, glm::vec3 end, glm::vec3 color);
	void addRect(const Rectangle& rect, glm::vec3 color);
	void generateBuffers();
};