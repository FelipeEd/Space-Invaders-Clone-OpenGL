#include <jellyfish/Jellyfish.hpp>

Alien::Alien(float posx, float posy, unsigned int texture, int nframes)
{
    this->initPos.x = posx;
    this->initPos.y = posy;
    this->ship = Entity(posx, posy, texture, nframes);
    this->vida = 40.0f;
}

void Alien::fire()
{
    this->gun.fire(this->ship.getPosition());
}

void Alien::takeDamage(float hit)
{
    this->vida -= hit;
    if (vida <= 0)
        this->ship.active = false;
}

void Alien::draw()
{
    this->gun.drawBullets();
    if (this->ship.active)
    {
        if (this->vida > 20.0f)
            this->ship.draw();
        else
        {
            this->ship.sprite.setColor(red);
            this->ship.draw();
        }
    }
}

void Alien::move(Entity empty)
{
    if (traveling)
    {
        this->warpArround(empty);
    }
    else
        this->ship.move();
    this->gun.moveBullets();
}

void Alien::warpArround(Entity empty)
{
    this->ship.setSpeed(empty.getSpeed().x, -0.01);
    this->ship.move();

    if (this->ship.getPosition().y <= -1.05)
    {
        this->ship.setPosition(this->ship.getPosition().x, 1.25f);
        this->warped = true;
    }

    if (this->ship.getPosition().y <= this->initPos.y && this->warped)
    {
        this->ship.setPosition(this->ship.getPosition().x, this->initPos.y);
        this->traveling = false;
        this->warped = false;
    }
}
