#include "Player.h"


Player::Player(){

    for(auto& bullet: bullets)
    {
        bullet = Entity(-100, -100, textureBullet, 1);
        bullet.setSpeed(0.0f, 0.02f);
    }
}


void Player::update()
{

    for(auto& bulet: bullets)
    {
        bulet.move();
    }

    if(MoveRight){
        this->ship.setAccel(this->currentAccel, 0.0f);
        this->ship.accelerate();
    }

    if(MoveLeft){
        this->ship.setAccel(-this->currentAccel,0.0f);
        this->ship.accelerate();
    }

    // Nao deixa a nave passar da velocidade maxima
    if(abs(this->ship.getSpeed()[0]) > maxSpeed)
        this->ship.setSpeed(ship.getSpeed()[0] * maxSpeed / abs(ship.getSpeed()[0]), 0.0f);

    
    this->ship.moveInBounds();

    if(Fire && this->currentfireCooldown==0){
        this->fire();
        this->currentfireCooldown = this->fireCooldown;
    }

    // Reseta os inputs
    Fire = false;
    MoveLeft = false;
    MoveRight = false;

}


void Player::draw()
{
    this->ship.draw();
}


void Player::drawBullets()
{
    for(auto& bulet: bullets)
        {
            bulet.draw();
        }
}


void Player::keyUpdate(GLFWwindow *window)
{
    // Coisas dependentes de tempo
    this->currentfireCooldown--;
    if(this->currentfireCooldown<0)
        this->currentfireCooldown=0;
    

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        this->MoveLeft = true;
        
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        this->MoveRight = true;
        
    }
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
       
    }

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
       this->Fire = true;
    }
}


void Player::fire()
{
    this->countCurrentBullet = this->countCurrentBullet % 10;

    this->bullets[countCurrentBullet].setPosition(this->ship.getPosition()[0], this->ship.getPosition()[1]+0.1f);
    this->countCurrentBullet++;
}


float* Player::getPosition()
{
    return this->ship.getPosition();
}

