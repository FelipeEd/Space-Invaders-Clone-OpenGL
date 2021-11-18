#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// Global
// Tamanho da janela
unsigned int WIDTH = 1280;
unsigned int HEIGHT = 720;

unsigned int vao;
unsigned int vbo;

// Texturas
unsigned int texturePlayer;
unsigned int textureBullet;

// Colors
float black[4] = {0.0f,0.0f,0.0f,1.0f};
float white[4] = {1.0f,1.0f,1.0f,1.0f};
float cyan[4] = {0.0f,1.0f,1.0f,1.0f};
float red[4] = {1.0f,0.0f,0.0f,1.0f};
float magenta[4] = {1.0f,1.0f,0.0f,1.0f};

#include "BuffersAndGlobals.h"
#include "Shader.hpp"
#include "Rectangle.hpp"
#include "Entity.h"
#include "Player.h"


int main()
{
    // inicialização e configuração de contexto
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
    // criação da janela e verificação de erros
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello triangle", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Falha na criacao da janela" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD: Carregamento dos ponteiros de funções para utilizar OpenGL moderno
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Falha na inicializacao do GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSwapInterval(1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    //-------------------------------------------------
    // Buffers e texturas
    

    createBuffers(vao,vbo);
    texturePlayer = createTexture("art//Nave2SpriteSheet.png");
    textureBullet = createTexture("art//Bullet.png");
    unsigned int texture1 = createTexture("art//Nave.png");
    unsigned int texture2 = createTexture("art//Nave2.png");
    unsigned int texture3 = createTexture("art//Alien1.png");
    unsigned int texture4 = createTexture("art//Alien1-256.png");

    Entity rect1(0, 0, texture1, 1);
    Entity rect2(WIDTH/2, HEIGHT/2, texture2, 1);
    Entity rect3(WIDTH, HEIGHT, texture1, 1);
    Entity rect4(0, HEIGHT, texture1, 1);
    Entity rect5(WIDTH, 0, texture1, 1);
    Entity rect6(WIDTH-200, 100, texture3, 1);
    Entity rect7(WIDTH-200, 300, texture4, 1);

    Player player;

    double prevTime = 0.0;
    double crntTime = 0.0;
    double timeDiff;
    unsigned int counter = 0;

    // Loop da aplicação
    while (!glfwWindowShouldClose(window))
    {
        crntTime = glfwGetTime();
        timeDiff = crntTime - prevTime;
        counter++;

        // Coisas dependentes de tempo roda uma vez por frame
        if(timeDiff >= 1.0/120.0)
        {
            std::string FPS = std::to_string((1.0/timeDiff) * counter);
            std::string ms = std::to_string((timeDiff/counter) * 1000);
            std::string newTitle = "Space Invaders - " + FPS + "FPS /" + ms + "ms";
            glfwSetWindowTitle(window, newTitle.c_str());
            prevTime = crntTime;
            counter = 0;

            player.keyUpdate(window);
        }
 
        // Input handling
        processInput(window);

        // Render ----------------------------------------------------------------------
        // Define a cor de fundo da janela
        glClearColor(0.01f, 0.01f, 0.02f, 1.0f);
        // Limpa algum buffer específico
        glClear(GL_COLOR_BUFFER_BIT); // A limpeza do framebuffer de cor será feita com a cor definida em glClearColor


        rect1.draw();
        rect2.draw();
        rect3.draw();
        rect4.draw();
        rect5.draw();
        rect6.draw();
        rect7.draw();

        player.update();
        player.drawBullets();
        player.draw();
        
        
        //std::cout << rect.position[0] <<" "<<rect.position[1] << ", "<< rect2.position[0] <<" "<<rect2.position[1] <<std::endl;
     
        // Faz a troca do framebuffer antigo para o novo (double buffer)
        glfwSwapBuffers(window);

        //------------------------------------------------------------------------------

        // Captura eventos de IO (movimento de mouse, teclas pressionadas, etc)
        glfwPollEvents();

        
    }

    // Desalocar memória dos objetos instanciados
    glfwTerminate(); // Faz a limpeza dos recursos utilizados pelo GLFW
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

}

// Função executada toda vez que há um redimensionamento de janela
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   
    glViewport(0, 0, width, height);
    
}

