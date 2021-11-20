#include <jellyfish/Jellyfish.hpp>

AlienSquad::~AlienSquad()
{
    delete this->Aliens;
}

AlienSquad::AlienSquad(Alien parentAlien, int pixelx, int pixely, int n, int m)
{
    this->position[0] = pixelXToNDC(pixelx);
    this->position[1] = pixelYToNDC(pixely);
    this->n = n;
    this->m = m;
    this->Aliens = new Alien[n * m];

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            Aliens[i * m + j] = parentAlien;
            Aliens[i * m + j].setPosition(parentAlien.getPosition()[0] + clampDist * j, parentAlien.getPosition()[1] - clampDist * i);
            Aliens[i * m + j].initPos[0] = Aliens[i * m + j].getPosition()[0];
            Aliens[i * m + j].initPos[1] = Aliens[i * m + j].getPosition()[1];
        }
    }
}

void AlienSquad::move()
{
    this->deltaX += 0.1f;
    for (int i = 0; i < n * m; i++)
    {
        float distx = 2 - n * this->clampDist;

        float oldposx = this->Aliens[i].initPos[0];
        float oldposy = this->Aliens[i].initPos[1];

        float posx = this->Aliens[i].getPosition()[0];
        float posy = this->Aliens[i].getPosition()[1];

        this->Aliens[i].setPosition(oldposx, oldposy + sin((this->deltaX + oldposx) * 700) / 20);
    }
}

void AlienSquad::draw()
{
    for (int i = 0; i < n * m; i++)
    {
        this->Aliens[i].draw();
    }
}

void AlienSquad::debug()
{
    std::cout << std::fixed;
    std::cout << "AlienSquad ::" << std::endl;
    for (int i = 0; i < n * m; i++)
    {
        std::cout << "    Alien ::"
                  << " x: " << this->Aliens[i].getPosition()[0]
                  << " y: " << this->Aliens[i].getPosition()[1] << std::endl;
    }
}
