#include <jellyfish/Jellyfish.hpp>

Player::Player()
{
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
    if (this->ship.getSpeed().x > 0.02)
        this->ship.sprite.setCurrentFrame(1);
    else if (this->ship.getSpeed().x < -0.02)
        this->ship.sprite.setCurrentFrame(2);
    else
        this->ship.sprite.setCurrentFrame(0);

    if (Fire)
    {
        sndPlaySound("bin\\assets\\test.wav", SND_FILENAME | SND_ASYNC);
        Point firepos = this->getPosition();
        firepos.y += 0.001f;
        this->gun.fire(firepos);
    }

    this->gun.moveBullets();
    this->ship.moveInBounds();
    // Reseta os inputs
    Fire = false;
    MoveLeft = false;
    MoveRight = false;
}

void Player::draw()
{
    this->gun.drawBullets();
    if (this->vida >= 50.0f)
        this->ship.draw();
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
    this->gun.countCoolDown();

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        //this->MoveLeft = true;
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
    this->gun.debug();
}
