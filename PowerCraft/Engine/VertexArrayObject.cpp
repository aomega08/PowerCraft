#include <stdafx.h>
#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject() {
	glGenVertexArrays(1, &_id);
}

VertexArrayObject::~VertexArrayObject() {
	glDeleteVertexArrays(1, &_id);
}

void VertexArrayObject::Bind() {
	glBindVertexArray(_id);
}
