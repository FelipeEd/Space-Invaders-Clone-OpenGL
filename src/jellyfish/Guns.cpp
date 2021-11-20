#include <jellyfish/Jellyfish.hpp>

Guns::Guns(int type)
{
    this->type = type;
    // Alien ammo 1
    if (type == 0)
    {
        this->maxAmmo = 9999999;
        this->currentAmmo = maxAmmo;
        this->fireCooldown = 40;

        for (auto &bullet : bullets)
        {
            bullet = Entity(-100, -100, 0.3f, 0.3f, textureBullet, 1);
            bullet.setSpeed(0.0f, -0.01f);
        }
    }

    // Player ammo 1
    if (type == 1)
    {
        this->maxAmmo = 9999999;
        this->currentAmmo = maxAmmo;
        this->fireCooldown = 15;

        for (auto &bullet : bullets)
        {
            bullet = Entity(-100, -100, 0.5f, 0.5f, textureBullet, 1);
            bullet.setSpeed(0.0f, 0.02f);
        }
    }
}

void Guns::fire(float posx, float posy)
{
    if (this->currentfireCooldown == 0 && this->currentAmmo > 0)
    {
        // coloca o projetil na posição desejada (normalmente a frente da nave)
        this->bullets[countCurrentBullet].setPosition(posx, posy);

        // reseta o cooldown da arma
        this->currentfireCooldown = this->fireCooldown;

        this->currentAmmo--;
        // Ciclar no array de balas
        this->countCurrentBullet++;
        this->countCurrentBullet = this->countCurrentBullet % 10;
    }
}

void Guns::reload()
{
    this->currentAmmo = this->maxAmmo;
}

void Guns::countCoolDown()
{
    this->currentfireCooldown--;
    if (this->currentfireCooldown < 0)
        this->currentfireCooldown = 0;
}

void Guns::moveBullets()
{
    for (auto &bullet : bullets)
    {
        bullet.accelerate();
        bullet.move();
    }
}

void Guns::drawBullets()
{
    for (auto &bulet : bullets)
    {
        bulet.draw();
    }
}

void Guns::debug()
{
    for (auto &bullet : this->bullets)
    {
        float posx = bullet.getPosition()[0];
        float posy = bullet.getPosition()[1];
        if (abs(posx) <= 1.0f && abs(posy) <= 1.0f)
            std::cout << "    Bullet :: Type " << this->type
                      << " ::"
                      << " x: " << posx
                      << " y: " << posy << std::endl;
    }
}
