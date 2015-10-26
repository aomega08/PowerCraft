#include <stdafx.h>
#include "VertexBufferObject.h"

VertexBufferObject::VertexBufferObject() {
	glGenBuffers(1, &_id);
}

VertexBufferObject::~VertexBufferObject() {
	glDeleteBuffers(1, &_id);
}

void VertexBufferObject::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void VertexBufferObject::Upload(void *data, size_t size, int type) {
	glBufferData(GL_ARRAY_BUFFER, size, data, type);
}
