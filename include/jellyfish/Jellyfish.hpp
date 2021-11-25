#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stbi/stb_image.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include <chrono>
#include <thread>
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono;
#include <windows.h>
#include <mmsystem.h>
//these two headers are already included in the <Windows.h> header
#pragma comment(lib, "Winmm.lib")

// variaveis globais do main
extern unsigned int WIDTH;
extern unsigned int HEIGHT;

extern unsigned int vao;
extern unsigned int vbo;

extern float white[4];
extern float red[4];
extern float blue[4];
extern float magenta[4];

extern unsigned int texturePlayer;
extern unsigned int textureBullet1;
extern unsigned int textureBullet2;
extern unsigned int textureBullet3;
extern unsigned int textureBullet4;

extern unsigned int textureHitBox;

extern unsigned int Score;

extern char laser1[30000];

typedef struct Point
{
    float x;
    float y;

    bool operator==(Point b)
    {
        if (x == b.x && y == b.y)
            return true;
        else
            return false;
    }
} Point;

class Shader;
class Sprite;
class HitBox;
class Entity;
class Guns;
class Player;
class Alien;
class AlienSquad;

extern Shader shaderProgram;

unsigned int createTexture(const char *textureName);
void createBuffers(unsigned int &vao, unsigned int &vbo, Shader &shaderProgram);
float pixelXToNDC(int x);
float NDCXToPixel(float x);
float pixelYToNDC(int y);
void print(const char *msg);
void DisplayScore(Sprite numbers);
void SaveScore(unsigned int Score);

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
    void setVec2(const std::string &uniformName, Point value) const;
    void setVec4(const std::string &uniformName, float value[4]) const;
    void Delete() const;
};

// Um objeto que tem como responsabilidade desenhar uma textura animada em uma posição dada
// responsavel por controlar as animações (Interface direta com openGL)
class Sprite
{

private:
    // Cor RGBA, padrão branco
    float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    // Indice de uma textura previamente alocada
    unsigned int texture;
    int nFrames = 1;
    int currentFrame = 0;

    // Shader usado para desenhar o retangulo
    Shader shader;

public:
    // Dimensões do retangulo
    Point scale;
    Sprite();
    Sprite(float w, float h, unsigned int intexture, int nframes);

    // Usa openGL para desenhar o retangulo, setando as uniforms.
    void draw(Point position);
    // Seta o frame atual da animação (começa em 0)
    void setCurrentFrame(int n);
    // Seta a cor a ser multiplicada pela textura
    void setColor(float newcolor[4]);
};

// Classe que cuida das colissões
class HitBox
{
private:
    // Do canto esquerdo superior no sentido horário
    Point center, size;

public:
    Sprite aux;
    HitBox() {}
    // Recebe a posição altura e largura
    HitBox(Point pos, float w, float h);
    // Verifica colisão com outra hitbox
    bool Hit(HitBox target);

    void setPosition(Point pos);
    void draw();
};

// Um Objeto genérico do programa, possui posição, velocidade e aceleração
// Deve ser usado para construir coissas mais complexas que dependam de imput
class Entity
{

private:
    // Vetores normalizados para NCD
    Point position = {-100, -100};
    Point speed = {0.0f, 0.0f};

public:
    bool active = true;
    Sprite sprite;
    HitBox hitbox;

    Entity(){};
    // Invisivel
    Entity(float posx, float posy);
    // Entidades quadradas
    Entity(float posx, float posy, unsigned int texture, int nframes);
    // Entidades retangulares
    Entity(float posx, float posy, float w, float h, unsigned int texture, int nframes);

    // Chama a função draw do retangulo associado a entidade
    void draw();
    // Move o retangulo baseado na velocidade atual
    void move();
    // Move o retangulo baseado na velocidade atual mantendo na tela
    void moveInBounds();
    // Printado quando no modo depuração
    void debug();

    // Autopiloto simples para chegar em um ponto controlando a aceleração

    void setSpeed(float spx, float spy);
    // Define a Posição atual
    void setPosition(float posx, float posy);

    // Retorna a Velocidade atual
    Point getSpeed();
    // Retorna a Posição atual
    Point getPosition();
    // Retorna a Scale
    Point getScale();
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

    // Máximo de munição possivel
    int maxAmmo = 0;
    // Munição atual
    int currentAmmo = 0;
    // Dano por hit
    float damage;

public:
    // cooldown atual (só dispara quando zeradp)
    int currentfireCooldown = 0;
    Guns() {}
    // Qual dos tipos de arma
    Guns(int type);
    // Dispara a arma
    void fire(Point pos);
    // Reseta o numero de balas
    void reload();
    // Deve ser usada em um update por segundo
    void countCoolDown();
    // Atualiza a posição das balas
    void moveBullets();
    // Detecta colisoes com o squad de aliens
    void interact(AlienSquad &aliens);
    // Detecta colisões com entidades quaisquer
    void interact(Player &Player);
    // O que faze quando colidir
    void onHit(Entity &currentBullet);
    // Desenha as balas na tela
    void drawBullets();
    // Desenha as hitbox das balas
    void drawBulletsHitBoxes();
    // Debug
    void debug();
};

class Player
{
private:
    float vida = 100.0f;
    // Aceleração maxima da nave
    float currentAccel = 0.01f;
    float maxSpeed = 0.1f;

    double mouseX, mouseY;

    // Estado atual do player (depende dos imputs)
    bool Fire = false;
    bool MoveLeft = false;
    bool MoveRight = false;

    bool changeGun = false;
    int changeGunCooldown = 0;

public:
    Entity ship;
    Guns gun[2];
    int currentGun = 0;

    Player();
    // Roda a cada iteração do main principal
    void update();
    // Desenha o nave do player
    void draw();

    void takeDamage(float hit);
    // Notifica o player do status de cada input possivel e roda a cada frame
    void keyUpdate(GLFWwindow *window);
    void debug();

    // Retorna a posição do player
    Point getPosition();
};

// Classe para um alien individual
class Alien
{
public:
    float vida;
    float vidaMax;
    unsigned int points = 10;
    int type;
    Entity ship;
    Guns gun;
    bool traveling = false;
    bool warped = false;
    int TravelingChance;

    Point initPos;
    Alien() {}
    Alien(float posx, float posy, unsigned int texture, int nframes, int type);
    void fire();
    void draw();
    void takeDamage(float hit);
    void move(Entity empty);
    void warpArround(Entity empty);
};

// Classe que controla o cluster de aliens do mesmo tipo
class AlienSquad
{
private:
    Entity empty;
    Point speed = {0.002f, 0.0f};
    float clampDist = 0.10f;
    int curentfireCooldown = 0;
    int fireCooldown;
    float deltaX, deltaY;

public:
    int n, m;
    bool Alive = true;
    Alien *Aliens;

    ~AlienSquad();
    // Gera um cluster de aliens copiandos as propriedasdes do parent alien
    AlienSquad(Alien parentAlien, float posx, float posy, int n, int m, int timer);

    void update(Player &player);
    // Faz algum alien atacar;
    void squadFire();

    void move();
    void draw();
    void drawHitbox();
    void debug();
};