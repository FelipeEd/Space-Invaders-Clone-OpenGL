#pragma once

#include <glad/glad.h>
#include "stb_image.h"
#include <iostream>

unsigned int createTexture(const char* textureName);
void createBuffers(unsigned int &vao, unsigned int &vbo);