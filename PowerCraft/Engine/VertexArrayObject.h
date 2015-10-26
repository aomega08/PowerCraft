#pragma once

#include "../stdafx.h"

class VertexArrayObject {
public:
	VertexArrayObject();
	~VertexArrayObject();

	void Bind();

private:
	GLuint _id;
};
