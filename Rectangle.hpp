#pragma once

#include <glm/glm.hpp>
#include "stb_image.h"
#include "Shader.hpp"


// variaveis globais do main
extern unsigned int WIDTH;
extern unsigned int HEIGHT;

extern unsigned int vao;
extern unsigned int vbo;


class Rectangle
{  
public:

// Position
float position[2];
float height,width;
float *color;
unsigned int texture;
Shader shader;

Rectangle(float posx, float posy, float h, float w,float color[4], unsigned int intexture);
void draw();


};