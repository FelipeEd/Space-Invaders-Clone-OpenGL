#include <jellyfish/Jellyfish.hpp>

Alien::Alien(float posx, float posy, unsigned int texture, int nframes)
{
    this->initPos[0] = posx;
    this->initPos[1] = posy;
    this->ship = Entity(posx, posy, texture, nframes);
}

void Alien::draw()
{
    if (this->Alive)
    {
        this->ship.draw();
    }
}

void Alien::setPosition(float posx, float posy)
{
    this->ship.setPosition(posx, posy);
}

float *Alien::getPosition()
{
    return this->ship.getPosition();
}