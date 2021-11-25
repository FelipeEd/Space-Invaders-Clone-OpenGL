#include <jellyfish/Jellyfish.hpp>

Player::Player()
{
    this->gun[0] = Guns(1);
    this->gun[1] = Guns(3);
    this->gun[2] = Guns(5);
    this->ship = Entity(0, -0.8f, texturePlayer, 3);
}

void Player::update()
{

    float xdist = pixelXToNDC(mouseX) - this->getPosition().x;

    // Verifica se o mouse esta a direita ou esquerda do player
    if (xdist > 0)
    {
        MoveRight = true;
    }
    else if (xdist < 0)
    {
        MoveLeft = true;
    }

    if (MoveRight)
    {
        this->ship.setSpeed(this->maxSpeed * abs(xdist), 0.0f);
    }

    if (MoveLeft)
    {
        this->ship.setSpeed(-this->maxSpeed * abs(xdist), 0.0f);
    }

    // Nao deixa a nave passar da velocidade maxima
    if (abs(this->ship.getSpeed().x) > maxSpeed)
    {
        this->ship.setSpeed(ship.getSpeed().x * maxSpeed / abs(ship.getSpeed().x), 0.0f);
    }

    // Animation control
    if (this->ship.getSpeed().x > 0.01)
    {
        this->ship.sprite.setCurrentFrame(1);
    }
    else if (this->ship.getSpeed().x < -0.01)
    {
        this->ship.sprite.setCurrentFrame(2);
    }
    else
    {
        this->ship.sprite.setCurrentFrame(0);
    }

    if (changeGun && this->changeGunCooldown == 0)
    {
        this->currentGun += 1;
        this->currentGun = this->currentGun % 3;
        this->changeGunCooldown = 20;
    }

    if (Fire)
    {
        if (this->gun[currentGun].currentfireCooldown == 0)
        {
            sndPlaySound(laser1, SND_MEMORY | SND_ASYNC);
        }

        Point firepos = this->getPosition();
        firepos.y += 0.001f;
        this->gun[currentGun].fire(firepos);
    }

    this->gun[0].moveBullets();
    this->gun[1].moveBullets();
    this->gun[2].moveBullets();

    this->ship.moveInBounds();
    // Reseta os inputs
    Fire = false;
    MoveLeft = false;
    MoveRight = false;
    changeGun = false;
}

void Player::draw()
{
    this->gun[0].drawBullets();
    this->gun[1].drawBullets();
    this->gun[2].drawBullets();

    if (this->vida >= 50.0f)
    {
        this->ship.draw();
    }
    else
    {
        this->ship.sprite.setColor(red);
        this->ship.draw();
    }
}

void Player::takeDamage(float hit)
{
    this->vida -= hit;
    if (vida <= 0)
        this->ship.active = false;
}

void Player::keyUpdate(GLFWwindow *window)
{
    // Coisas dependentes de tempo
    this->gun[0].countCoolDown();
    this->gun[1].countCoolDown();
    this->gun[2].countCoolDown();

    if (this->changeGunCooldown > 0)
    {
        this->changeGunCooldown--;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        this->changeGun = true;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        //this->MoveRight = true;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        this->Fire = true;
    }

    glfwGetCursorPos(window, &mouseX, &mouseY);

    //std::cout << mouseX << " " << mouseY << std::endl;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        this->Fire = true;
    }
    //glfwSetCursorPos(window, (width / 2), (height / 2));
    //glfwGetCursorPos(window, &mouseX, &mouseY);
}

Point Player::getPosition()
{
    return this->ship.getPosition();
}

void Player::debug()
{
    std::cout << "Player ::" << std::endl;
    //std::cout << "Player ::"<< this-> << std::endl;
    this->ship.debug();
    this->gun[0].debug();
    this->gun[1].debug();
    this->gun[2].debug();
}
