#include <jellyfish/Jellyfish.hpp>

AlienSquad::~AlienSquad()
{
    delete[] this->Aliens;
}

AlienSquad::AlienSquad(Alien parentAlien, float posx, float posy, int n, int m, int timer)
{
    // Dimenções do cluster
    empty = Entity(posx, posy);
    this->n = n;
    this->m = m;
    this->Aliens = new Alien[n * m];
    this->empty.setSpeed(this->speed.x, this->speed.y);
    this->fireCooldown = timer;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            Aliens[i * m + j] = parentAlien;
            Aliens[i * m + j].ship.setPosition(parentAlien.ship.getPosition().x + clampDist * j, parentAlien.ship.getPosition().y - clampDist * 16.0 / 9.0 * i);
            Aliens[i * m + j].initPos = Aliens[i * m + j].ship.getPosition();
        }
    }
    // Colocando o centro do squad no centro da tropa
    this->empty.setPosition(Aliens[(n * m) / 2].ship.getPosition().x, Aliens[(n * m) / 2].ship.getPosition().y);
}

void AlienSquad::update(Player &player)
{
    this->move();
    this->squadFire();
    for (int i = 0; i < n * m; i++)
    {
        this->Aliens[i].gun.interact(player);
    }

    if (rand() % 1000 < this->Aliens[0].TravelingChance)
    {
        int index = rand() % (n * m);
        this->Aliens[index].traveling = true;
    }
}

void AlienSquad::squadFire()
{
    int alienIndex = rand() % (n * m);
    //std::cout << "index " << alienIndex << "cooldown " << this->curentfireCooldown << std::endl;
    if (this->Aliens[alienIndex].ship.active && this->curentfireCooldown == 0)
    {
        this->Aliens[alienIndex].fire();
        this->curentfireCooldown = this->fireCooldown;
    }
    if (this->curentfireCooldown > 0)
        this->curentfireCooldown--;
}

void AlienSquad::move()
{
    float moveRoom = 0.5f;
    int countAlive = 0;
    // movimeno lateral
    if (this->empty.getPosition().x > moveRoom)
        this->empty.setSpeed(-this->speed.x, 0.0f);

    if (this->empty.getPosition().x < -moveRoom)
        this->empty.setSpeed(this->speed.x, 0.0f);

    this->empty.move();

    for (int i = 0; i < n * m; i++)
    {
        this->Aliens[i].ship.setSpeed(this->empty.getSpeed().x, this->empty.getSpeed().y);
        this->Aliens[i].move(this->empty);
        if (this->Aliens[i].ship.active)
            countAlive++;
    }

    if (countAlive == 0)
    {
        this->Alive = false;
    }
}

void AlienSquad::draw()
{
    for (int i = 0; i < n * m; i++)
    {
        this->Aliens[i].draw();
    }
}

void AlienSquad::drawHitbox()
{
    for (int i = 0; i < n * m; i++)
    {
        if (this->Aliens[i].ship.active)
        {
            this->Aliens[i].ship.hitbox.draw();
        }
        this->Aliens[i].gun.drawBulletsHitBoxes();
    }
}

void AlienSquad::debug()
{
    std::cout << std::fixed;
    std::cout << "AlienSquad ::" << std::endl;
    for (int i = 0; i < n * m; i++)
    {
        if (this->Aliens[i].ship.active)
        {
            std::cout << std::fixed;
            std::cout << "    Alien " << this->Aliens[i].type
                      << " ::"
                      << " Pos ::"
                      << " x: " << this->Aliens[i].ship.getPosition().x
                      << " y: " << this->Aliens[i].ship.getPosition().y
                      << " ::"
                      << " Speed :: "
                      << " x: " << this->Aliens[i].ship.getSpeed().x
                      << " y: " << this->Aliens[i].ship.getSpeed().y << std::endl;
            this->Aliens[i].gun.debug();
        }
    }
}
