#include <jellyfish/Jellyfish.hpp>

Entity::Entity(int posx, int posy, unsigned int texture, int nframes)
{
    this->position[0] = pixelXToNDC(posx);
    this->position[1] = pixelYToNDC(posy);
    rect = Rectangle(1.0f, 1.0f, texture, nframes);
}

Entity::Entity(int posx, int posy, float h, float w, unsigned int texture, int nframes)
{
    rect = Rectangle(h, w, texture, nframes);
}

void Entity::draw()
{
    this->rect.draw(this->position);
}

void Entity::accelerate()
{
    this->speed[0] += this->accel[0];
    this->speed[1] += this->accel[1];
}

void Entity::move()
{
    this->position[0] += this->speed[0];
    this->position[1] += this->speed[1];
}

void Entity::moveInBounds()
{
    float newPosX = this->position[0] + this->speed[0];
    float newPosY = this->position[1] + this->speed[1];

    if (abs(newPosX) < 1 && abs(newPosY) < 1)
    {
        this->position[0] = newPosX;
        this->position[1] = newPosY;
    }
    else
    {
        // Resetar as velocidades e aceleração caso bater no canto da tela
        this->accel[0] = 0.0f;
        this->accel[1] = 0.0f;
        this->speed[0] = 0.0f;
        this->speed[1] = 0.0f;
    }
}

void Entity::setAccel(float acx, float acy)
{
    this->accel[0] = acx;
    this->accel[1] = acy;
}

void Entity::setSpeed(float spx, float spy)
{
    this->speed[0] = spx;
    this->speed[1] = spy;
}

void Entity::setPosition(float posx, float posy)
{
    this->position[0] = posx;
    this->position[1] = posy;
}

float *Entity::getSpeed()
{
    return this->speed;
}

float *Entity::getPosition()
{
    return this->position;
}
