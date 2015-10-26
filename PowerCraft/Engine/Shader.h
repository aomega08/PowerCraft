#pragma once

#include "../stdafx.h"

class Shader {
	friend class ShaderProgram;

public:
	Shader(std::string path);
	~Shader();

	void Compile();

protected:
	GLuint _id;

private:
	std::string _path;
};
