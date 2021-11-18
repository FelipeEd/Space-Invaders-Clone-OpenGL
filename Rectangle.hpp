#pragma once

#include <glm/glm.hpp>
#include "stb_image.h"
#include "Shader.hpp"


// variaveis globais do main
extern unsigned int WIDTH;
extern unsigned int HEIGHT;

extern unsigned int vao;
extern unsigned int vbo;

extern float white[4];


// Um objeto que tem como responsabilidade desenhar uma textura animada em uma posição dada
// responsavel por controlar as animações (Interface direta com openGL)
class Rectangle
{  

private:

// Dimensões do retangulo
float scale[2];

// Cor RGBA, padrão branco
float color[4] = {1.0f,1.0f,1.0f,1.0f};

// Indice de uma textura previamente alocada
unsigned int texture;
int nFrames = 1;
int currentFrame = 0;

// Shader usado para desenhar o retangulo
Shader shader;

public:

Rectangle();
Rectangle(float h, float w, unsigned int intexture, int nframes);

// Usa openGL para desenhar o retangulo, setando as uniforms.
void draw(float position[2]);
// Seta o frame atual da animação (começa em 0) 
void setCurrentFrame(int n);
// Seta a cor a ser multiplicada pela textura
void setColor(float newcolor[4]);

};