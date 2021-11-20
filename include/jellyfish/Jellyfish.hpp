#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stbi/stb_image.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>

// variaveis globais do main
extern unsigned int WIDTH;
extern unsigned int HEIGHT;

extern unsigned int vao;
extern unsigned int vbo;

extern float white[4];

extern unsigned int texturePlayer;
extern unsigned int textureBullet;

unsigned int createTexture(const char *textureName);
void createBuffers(unsigned int &vao, unsigned int &vbo);
float pixelXToNDC(int x);
float NDCXToPixel(float x);
float pixelYToNDC(int y);

class Shader;
class Rectangle;
class Entity;
class Guns;
class Player;
class Alien;
class AlienSquad;

void debug(Player player);

// Classe basica para compilar os shaders do openGL
class Shader
{
private:
    // the program ID
    unsigned int id;

public:
    // constructor will read and build the shader
    Shader();
    Shader(const char *vertexPath, const char *fragmentPath);
    // function to use/activate this shader
    void use();
    // utility functions to set values to uniforms in the shader program
    void setBool(const std::string &uniformName, bool value) const;
    void setInt(const std::string &uniformName, int value) const;
    void setFloat(const std::string &uniformName, float value) const;
    void setVec2(const std::string &uniformName, float value[2]) const;
    void setVec4(const std::string &uniformName, float value[4]) const;
};

// Um objeto que tem como responsabilidade desenhar uma textura animada em uma posição dada
// responsavel por controlar as animações (Interface direta com openGL)
class Rectangle
{

private:
    // Dimensões do retangulo
    float scale[2];

    // Cor RGBA, padrão branco
    float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

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

// Um Objeto genérico do programa, possui posição, velocidade e aceleração
// Deve ser usado para construir coissas mais complexas que dependam de imput
class Entity
{

private:
    Rectangle rect;

    // Vetores normalizados para NCD
    float position[2] = {-100, -100};
    float speed[2] = {0.0f, 0.0f};
    float accel[2] = {0.0f, 0.0f};

public:
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
    // Printado quando no modo depuração
    void debug();

    // Autopiloto simples para chegar em um ponto controlando a aceleração

    // Define a Aceleração atual
    void setAccel(float acx, float acy);
    // Define a Velocidade atual
    void setSpeed(float spx, float spy);
    // Define a Posição atual
    void setPosition(float posx, float posy);

    // Retorna a Aceleração atual
    float *getAccel();
    // Retorna a Velocidade atual
    float *getSpeed();
    // Retorna a Posição atual
    float *getPosition();
};

class Guns
{
private:
    int type;
    Entity bullets[10];
    // ciclar no array de balas
    int countCurrentBullet = 0;
    // cooldown fixo da arma
    int fireCooldown = 10;
    // cooldown atual (só dispara quando zeradp)
    int currentfireCooldown = 0;
    // Máximo de munição possivel
    int maxAmmo = 0;
    // Munição atual
    int currentAmmo = 0;

public:
    // Qual dos tipos de arma
    Guns(int type);
    // Dispara a arma
    void fire(float posx, float posy);
    // Reseta o numero de balas
    void reload();
    // Deve ser usada em um update por segundo
    void countCoolDown();
    // Atualiza a posição das balas
    void moveBullets();
    // Desenha as balas na tela
    void drawBullets();
    // Debug
    void debug();
};

class Player
{
private:
    Entity ship = Entity(WIDTH / 2, (int)HEIGHT * 0.9f, texturePlayer, 3);

    Guns gun = Guns(1);

    // Aceleração maxima da nave
    float currentAccel = 0.01f;
    float maxSpeed = 0.1f;

    double mouseX, mouseY;

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
    // Notifica o player do status de cada input possivel e roda a cada frame
    void keyUpdate(GLFWwindow *window);
    void debug();
    // Retorna a posição do player
    float *getPosition();
};

// Classe para um alien individual
class Alien
{
private:
    int hitpoints = 5;
    Entity ship;
    bool Alive = true;
    Guns gun = Guns(0);

public:
    float initPos[2];
    Alien() {}
    Alien(float posx, float posy, unsigned int texture, int nframes);
    void draw();

    void setPosition(float posx, float posy);

    float *getPosition();
};

// Classe que controla o cluster de aliens do mesmo tipo
class AlienSquad
{
private:
    float position[2];
    int n, m;
    float clampDist = 0.2f;
    float deltaX, deltaY;
    Alien *Aliens;

public:
    ~AlienSquad();
    AlienSquad(Alien parentAlien, int pixelx, int pixely, int n, int m);

    void move();
    void draw();
    void debug();
};