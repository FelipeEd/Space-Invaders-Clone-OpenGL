#pragma once


#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "Entity.h"


extern unsigned int texturePlayer;
extern unsigned int textureBullet;

class Player
{

private:

Entity ship = Entity(WIDTH/2, (int)HEIGHT*0.9f, texturePlayer, 3);

Entity bullets[10];

// Aceleração maxima da nave
float currentAccel = 0.001f;
float maxSpeed = 0.02f;

// Cuidar do cooldow das armas
int countCurrentBullet = 0;
int fireCooldown = 20;
int currentfireCooldown = 0;


// Estado atual do player (depende dos imputs)
bool Fire = false;
bool MoveLeft = false;
bool MoveRight = false;

public:

Player();
// Roda a cada iteração do main principal
void update();
// Desenha o nave do player
void draw();
// Desenha os projeteis do player
void drawBullets();
// Notifica o player do status de cada input possivel
void keyUpdate(GLFWwindow *window);
// Dispara as balas da nave
void fire();

// Retorna a posição do player
float* getPosition();

};