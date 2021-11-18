#pragma once


#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Rectangle.hpp"



// Um Objeto genérico do programa, possui posição, velocidade e aceleração
// Deve ser usado para construir coissas mais complexas que dependam de imput
class Entity
{

public:

Rectangle rect;

// Vetores normalizados para NCD
float position[2] = {-100,-100};
float speed[2] = {0.0f, 0.0f};
float accel[2] = {0.0f, 0.0f};


Entity(){};
// Entidades quadradas
Entity(int posx, int posy, unsigned int texture, int nframes);
// Entidades retangulares
Entity(int posx, int posy, float h, float w, unsigned int texture, int nframes);


// Chama a função draw do retangulo associado a entidade
void draw();
// Altera a velocidade baseada na aceleração
void accelerate();
// Move o retangulo baseado na velocidade atual
void move();
// Move o retangulo baseado na velocidade atual mantendo na tela
void moveInBounds();


// Define a Aceleração atual
void setAccel(float acx, float acy);
// Define a Velocidade atual
void setSpeed(float spx, float spy);
// Define a Posição atual
void setPosition(float posx, float posy);


// Retorna a velocidade atual
float* getSpeed();
// Retorna a Posição atual
float* getPosition();

};