#include <jellyfish/Jellyfish.hpp>

Alien::Alien(float posx, float posy, unsigned int texture, int nframes, int type)
{
    this->initPos.x = posx;
    this->initPos.y = posy;
    this->ship = Entity(posx, posy, texture, nframes);
    this->type = type;
    // O dourado
    if (this->type == 1)
    {
        this->vidaMax = 80.0f;
        this->vida = this->vidaMax;
        this->gun = Guns(0);
        this->TravelingChance = 20;
    }
    // O verde
    if (this->type == 2)
    {
        this->vidaMax = 60.0f;
        this->vida = this->vidaMax;
        this->gun = Guns(2);
        this->TravelingChance = 10;
    }
    // O roxo
    if (this->type == 3)
    {
        this->vidaMax = 40.0f;
        this->vida = this->vidaMax;
        this->gun = Guns(4);
        this->TravelingChance = 5;
    }
    // O azul
    if (this->type == 4)
    {
        this->vidaMax = 40.0f;
        this->vida = this->vidaMax;
        this->gun = Guns(6);
        this->TravelingChance = 1;
        //this->ship = Entity(posx, posy, 0.1f, 0.1f, texture, nframes);
    }
    // Boss
    if (this->type == 5)
    {
        this->vidaMax = 650.0f;
        this->vida = this->vidaMax;
        this->gun = Guns(8);
        this->TravelingChance = 1;
        this->ship = Entity(posx, posy, 0.7f, 0.7f, texture, nframes);
    }
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
        if (this->vida > this->vidaMax / 2.0)
        {
            this->ship.draw();
        }
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
    {
        this->ship.move();
        this->ship.setPosition(this->ship.getPosition().x, this->initPos.y + sin(this->ship.getPosition().x * 10) / 20);
    }

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
