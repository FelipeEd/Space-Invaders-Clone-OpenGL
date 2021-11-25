#include <jellyfish/Jellyfish.hpp>

Guns::Guns(int type)
{
    this->type = type;
    // Alien ammo 1
    if (type == 0)
    {
        this->maxAmmo = 99999;
        this->currentAmmo = maxAmmo;
        this->fireCooldown = 0;
        this->damage = 50.0f;
        for (auto &bullet : bullets)
        {
            bullet = Entity(-100, -100, 0.2f, 0.2f, textureBullet2, 1);
            bullet.sprite.setColor(red);
            bullet.active = false;
            bullet.setSpeed(0.0f, -0.005f);
        }
    }
    // Alien ammo 2
    if (type == 2)
    {
        this->maxAmmo = 99999;
        this->currentAmmo = maxAmmo;
        this->fireCooldown = 0;
        this->damage = 20.0f;
        for (auto &bullet : bullets)
        {
            bullet = Entity(-100, -100, 0.1f, 0.1f, textureBullet3, 1);

            bullet.active = false;
            bullet.setSpeed(0.0f, -0.02f);
        }
    }
    // Alien ammo 3
    if (type == 4)
    {
        this->maxAmmo = 99999;
        this->currentAmmo = maxAmmo;
        this->fireCooldown = 0;
        this->damage = 10.0f;
        for (auto &bullet : bullets)
        {
            bullet = Entity(-100, -100, 0.2f, 0.2f, textureBullet4, 1);

            bullet.active = false;
            bullet.setSpeed(0.0f, -0.02f);
        }
    }
    // Alien ammo 4
    if (type == 6)
    {
        this->maxAmmo = 99999;
        this->currentAmmo = maxAmmo;
        this->fireCooldown = 0;
        this->damage = 5.0f;
        for (auto &bullet : bullets)
        {
            bullet = Entity(-100, -100, 0.05f, 0.05f, textureBullet3, 1);
            bullet.sprite.setColor(blue);
            bullet.active = false;
            bullet.setSpeed(0.0f, -0.04f);
        }
    }

    // Player ammo 1
    if (type == 1)
    {
        this->maxAmmo = 99999;
        this->currentAmmo = maxAmmo;
        this->fireCooldown = 20;
        this->damage = 20;
        for (auto &bullet : bullets)
        {
            bullet = Entity(-100, -100, 0.1f, 0.1f, textureBullet1, 1);
            bullet.sprite.setColor(blue);
            bullet.active = false;
            bullet.setSpeed(0.0f, 0.03f);
        }
    }
    if (type == 3)
    {
        this->maxAmmo = 10;
        this->currentAmmo = maxAmmo;
        this->fireCooldown = 30;
        this->damage = 40;
        for (auto &bullet : bullets)
        {
            bullet = Entity(-100, -100, 0.2f, 0.2f, textureBullet1, 1);
            bullet.sprite.setColor(magenta);
            bullet.active = false;
            bullet.setSpeed(0.0f, 0.01f);
        }
    }
}

void Guns::fire(Point pos)
{
    if (this->currentfireCooldown == 0 && this->currentAmmo > 0)
    {

        // coloca o projetil na posição desejada (normalmente a frente da nave)
        this->bullets[countCurrentBullet].setPosition(pos.x, pos.y);
        this->bullets[countCurrentBullet].active = true;
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
    {
        this->currentfireCooldown = 0;
    }
}

void Guns::moveBullets()
{
    for (auto &bullet : bullets)
    {
        bullet.move();
        // Se sair da tela desativar
        if (!(abs(bullet.getPosition().x) <= 1.0f && abs(bullet.getPosition().y) <= 1.0f))
        {
            bullet.active = false;
        }
    }
}

void Guns::interact(AlienSquad &aliens)
{

    for (auto &bullet : this->bullets)
    {
        if (bullet.active)
        {
            for (int i = 0; i < aliens.n * aliens.m; i++)
            {
                if (aliens.Aliens[i].ship.active)
                {
                    if (bullet.hitbox.Hit(aliens.Aliens[i].ship.hitbox))
                    {

                        aliens.Aliens[i].takeDamage(this->damage);
                        this->onHit(bullet);
                        if (aliens.Aliens[i].ship.active == false)
                        {
                            Score += 100 / aliens.Aliens[i].type;
                        }
                    }
                }
            }
        }
    }
}

void Guns::interact(Player &player)
{
    for (auto &bullet : this->bullets)
    {
        if (bullet.active)
        {
            if (bullet.hitbox.Hit(player.ship.hitbox))
            {
                player.takeDamage(this->damage);
                this->onHit(bullet);
            }
        }
    }
}

void Guns::onHit(Entity &currentBullet)
{

    currentBullet.active = false;
}

void Guns::drawBullets()
{
    for (auto &bullet : bullets)
    {
        bullet.draw();
    }
}

void Guns::drawBulletsHitBoxes()
{
    for (auto &bullet : bullets)
    {
        bullet.hitbox.draw();
    }
}

void Guns::debug()
{
    for (auto &bullet : this->bullets)
    {
        if (bullet.active)
        {
            std::cout << std::fixed;
            std::cout << "    Bullet :: Type " << this->type
                      << " ::"
                      << " Pos ::"
                      << " x: " << bullet.getPosition().x
                      << " y: " << bullet.getPosition().y
                      << " ::"
                      << " Speed :: "
                      << " x: " << bullet.getSpeed().x
                      << " y: " << bullet.getSpeed().y << std::endl;
        }
    }
}
