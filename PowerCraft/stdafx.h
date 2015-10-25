#pragma once

#ifdef WIN32
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <stdlib.h>
#include <memory.h>
#include <iostream>
#include <fstream>
#include <string>

#define GLEW_STATIC
#include <Gl/glew.h>
#include <GLFW/glfw3.h>

#ifdef WIN32
#include <GL/GLU.h>
#endif
