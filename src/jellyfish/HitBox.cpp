#include <jellyfish/Jellyfish.hpp>

HitBox::HitBox(Point pos, float w, float h)
{
    this->center = pos;
    this->size = {w * HEIGHT / WIDTH, h};

    aux = Sprite(w, h, textureHitBox, 1);
}

bool HitBox::Hit(HitBox target)
{
    bool horizontal = false;
    bool vertical = false;

    horizontal = abs(this->center.x - target.center.x) < (this->size.x + target.size.x) / 2.0f;
    vertical = abs(this->center.y - target.center.y) < (this->size.y + target.size.y) / 2.0f;

    return horizontal && vertical;
}

void HitBox::setPosition(Point pos)
{
    this->center = pos;
}

void HitBox::draw()
{
    aux.draw(this->center);
}
