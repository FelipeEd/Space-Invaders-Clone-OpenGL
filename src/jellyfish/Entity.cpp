#include <jellyfish/Jellyfish.hpp>

Entity::Entity(float posx, float posy)
{
    this->position.x = posx;
    this->position.y = posy;
    hitbox = HitBox(this->getPosition(), 0.01f, 0.01f);
    this->active = false;
}

Entity::Entity(float posx, float posy, unsigned int texture, int nframes)
{
    this->position.x = posx;
    this->position.y = posy;
    sprite = Sprite(0.25f, 0.25f, texture, nframes);
    hitbox = HitBox(this->getPosition(), 0.25f * 0.7f, 0.25f * 0.7f);
}

Entity::Entity(float posx, float posy, float w, float h, unsigned int texture, int nframes)
{
    this->position.x = posx;
    this->position.y = posy;
    sprite = Sprite(w, h, texture, nframes);
    hitbox = HitBox(this->getPosition(), w * 0.7f, h * 0.7f);
}

void Entity::draw()
{
    if (this->active)
        this->sprite.draw(this->position);
}

void Entity::move()
{
    this->position.x += this->speed.x;
    this->position.y += this->speed.y;
    hitbox.setPosition(this->position);
}

void Entity::moveInBounds()
{
    float newPosX = this->position.x + this->speed.x;
    float newPosY = this->position.y + this->speed.y;

    if (abs(newPosX) < 1 && abs(newPosY) < 1)
    {
        this->move();
    }
    else
    {
        // Resetar as velocidades e aceleração caso bater no canto da tela
        this->speed.x = 0.0f;
        this->speed.y = 0.0f;
    }
}

void Entity::debug()
{
    if (this->active)
    {
        std::cout << std::fixed;
        std::cout << "    Entity ::" << std::endl;
        std::cout << "        Position :: "
                  << " x: " << this->getPosition().x
                  << " y: " << this->getPosition().y << std::endl;
        std::cout << "        Speed    :: "
                  << " x: " << this->getSpeed().x
                  << " y: " << this->getSpeed().y << std::endl;
    }
}

void Entity::setSpeed(float spx, float spy)
{
    this->speed.x = spx;
    this->speed.y = spy;
}

void Entity::setPosition(float posx, float posy)
{
    this->position.x = posx;
    this->position.y = posy;
    hitbox.setPosition(this->position);
}

Point Entity::getSpeed()
{
    return this->speed;
}

Point Entity::getPosition()
{
    return this->position;
}

Point Entity::getScale()
{
    return this->sprite.scale;
}
