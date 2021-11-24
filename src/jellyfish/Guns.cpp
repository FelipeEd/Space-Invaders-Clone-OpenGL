#include <jellyfish/Jellyfish.hpp>

Guns::Guns(int type)
{
    this->type = type;
    // Alien ammo 1
    if (type == 0)
    {
        this->maxAmmo = 9999999;
        this->currentAmmo = maxAmmo;
        this->fireCooldown = 0;
        this->damage = 20.0f;
        for (auto &bullet : bullets)
        {
            bullet = Entity(-100, -100, 0.1f, 0.1f, textureBullet, 1);
            bullet.sprite.setColor(red);
            bullet.active = false;
            bullet.setSpeed(0.0f, -0.02f);
        }
    }

    // Player ammo 1
    if (type == 1)
    {
        this->maxAmmo = 9999999;
        this->currentAmmo = maxAmmo;
        this->fireCooldown = 15;
        this->damage = 20;
        for (auto &bullet : bullets)
        {
            bullet = Entity(-100, -100, 0.1f, 0.1f, textureBullet, 1);
            bullet.active = false;
            bullet.setSpeed(0.0f, 0.03f);
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
        this->currentfireCooldown = 0;
}

void Guns::moveBullets()
{
    for (auto &bullet : bullets)
    {
        bullet.move();
        // Se sair da tela desativar
        if (!(abs(bullet.getPosition().x) <= 1.0f && abs(bullet.getPosition().y) <= 1.0f))
            bullet.active = false;
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
    if (this->type == 0)
        currentBullet.active = false;
    if (this->type == 1)
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
