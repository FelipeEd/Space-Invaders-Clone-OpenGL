#include <jellyfish/Jellyfish.hpp>

Player::Player()
{
}

void Player::update()
{

    float xdist = pixelXToNDC(mouseX) - this->getPosition()[0];

    // Verifica se o mouse esta a direita ou esquerda do player
    if (xdist > 0)
    {
        MoveRight = true;
    }
    else if (xdist < 0)
    {
        MoveLeft = true;
    }

    // Obsoleto
    if (MoveRight)
    {
        this->ship.setSpeed(this->maxSpeed * abs(xdist), 0.0f);
    }

    if (MoveLeft)
    {
        this->ship.setSpeed(-this->maxSpeed * abs(xdist), 0.0f);
    }

    //this->ship.accelerate();

    // Nao deixa a nave passar da velocidade maxima
    if (abs(this->ship.getSpeed()[0]) > maxSpeed)
    {
        this->ship.setSpeed(ship.getSpeed()[0] * maxSpeed / abs(ship.getSpeed()[0]), 0.0f);
    }

    this->gun.moveBullets();
    this->ship.moveInBounds();

    if (Fire)
    {
        this->gun.fire(this->getPosition()[0], this->getPosition()[1] + 0.001f);
    }

    // Reseta os inputs
    Fire = false;
    MoveLeft = false;
    MoveRight = false;
}

void Player::draw()
{
    this->gun.drawBullets();
    this->ship.draw();
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

float *Player::getPosition()
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
