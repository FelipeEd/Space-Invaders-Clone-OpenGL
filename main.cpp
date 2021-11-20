#include <jellyfish/Jellyfish.hpp>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, bool &gameIsPaused, bool &mouseMiddleisPressed);

// Global
// Tamanho da janela
unsigned int WIDTH = 1600; //1280;
unsigned int HEIGHT = 900; //720;

unsigned int vao;
unsigned int vbo;

// Texturas
unsigned int texturePlayer;
unsigned int textureBullet;

// Colors
float black[4] = {0.0f, 0.0f, 0.0f, 1.0f};
float white[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float cyan[4] = {0.0f, 1.0f, 1.0f, 1.0f};
float red[4] = {1.0f, 0.0f, 0.0f, 1.0f};
float magenta[4] = {1.0f, 1.0f, 0.0f, 1.0f};

int main()
{
    // inicialização e configuração de contexto
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // criação da janela e verificação de erros
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Hello triangle", NULL, NULL);
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
    createBuffers(vao, vbo);
    texturePlayer = createTexture("bin/assets/Nave2SpriteSheet.png");
    textureBullet = createTexture("bin/assets/Bullet.png");
    unsigned int texture1 = createTexture("bin/assets/Nave.png");
    unsigned int texture2 = createTexture("bin/assets/Nave2.png");
    unsigned int texture3 = createTexture("bin/assets/Alien1.png");
    unsigned int texture4 = createTexture("bin/assets/Alien1-256.png");

    Entity rect1(0, 0, texture1, 1);
    Entity rect2(WIDTH / 2, HEIGHT / 2, texture2, 1);
    Entity rect3(WIDTH, HEIGHT, texture1, 1);
    Entity rect4(0, HEIGHT, texture1, 1);
    Entity rect5(WIDTH, 0, texture1, 1);
    Entity rect6(WIDTH / 2, HEIGHT / 2, texture3, 1);
    Entity rect7(WIDTH - 200, 300, texture4, 1);

    Player player;

    Alien alien1(WIDTH / 2, HEIGHT / 2, texture3, 1);
    AlienSquad wave1(alien1, 0, 0, 4, 2);

    double prevTime = 0.0;
    double crntTime = 0.0;
    double timeDiff;
    unsigned int counter = 0;

    bool gameIsPaused = false;
    bool mouseRightIsPressed = false;
    bool mouseMiddleIsPressed = false;
    int pauseCooldown = 20;

    // Loop da aplicação
    while (!glfwWindowShouldClose(window))
    {
        // Input handling
        processInput(window, mouseRightIsPressed, mouseMiddleIsPressed);

        crntTime = glfwGetTime();
        timeDiff = crntTime - prevTime;
        counter++;
        // Coisas dependentes de tempo roda uma vez por frame
        if (timeDiff >= 1.0 / 120.0)
        {

            std::string FPS = std::to_string((1.0 / timeDiff) * counter);
            std::string ms = std::to_string((timeDiff / counter) * 1000);
            std::string newTitle = "Space Invaders - " + FPS + "FPS /" + ms + "ms";
            glfwSetWindowTitle(window, newTitle.c_str());
            prevTime = crntTime;
            counter = 0;

            if (mouseRightIsPressed && pauseCooldown == 0)
            {
                gameIsPaused = !gameIsPaused;
                pauseCooldown = 20;
            }

            if (pauseCooldown > 0)
                pauseCooldown--;

            mouseRightIsPressed = false;

            player.keyUpdate(window);
        }

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

        wave1.move();
        wave1.draw();

        player.update();
        player.draw();

        // Faz a troca do framebuffer antigo para o novo (double buffer)
        glfwSwapBuffers(window);

        // Main do jogo Pausado
        while (gameIsPaused && !glfwWindowShouldClose(window))
        {
            mouseMiddleIsPressed = false;
            processInput(window, mouseRightIsPressed, mouseMiddleIsPressed);

            crntTime = glfwGetTime();
            timeDiff = crntTime - prevTime;
            counter++;
            if (timeDiff >= 1.0 / 120.0)
            {
                std::string FPS = std::to_string((1.0 / timeDiff) * counter);
                std::string ms = std::to_string((timeDiff / counter) * 1000);
                std::string newTitle = "Space Invaders - " + FPS + "FPS /" + ms + "ms";
                glfwSetWindowTitle(window, newTitle.c_str());
                prevTime = crntTime;
                counter = 0;

                if (mouseRightIsPressed && pauseCooldown == 0)
                {
                    gameIsPaused = !gameIsPaused;
                    pauseCooldown = 20;
                }
                if (mouseMiddleIsPressed && pauseCooldown == 0)
                {
                    player.debug();
                    wave1.debug();
                    gameIsPaused = false;
                    pauseCooldown = 20;
                }
                if (pauseCooldown > 0)
                    pauseCooldown--;

                mouseRightIsPressed = false;

                glfwPollEvents();
            }
        }
        if (mouseMiddleIsPressed)
            gameIsPaused = true;

        //------------------------------------------------------------------------------

        // Captura eventos de IO (movimento de mouse, teclas pressionadas, etc)
        glfwPollEvents();
    }

    // Desalocar memória dos objetos instanciados
    glfwTerminate(); // Faz a limpeza dos recursos utilizados pelo GLFW
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window, bool &mouseRightIsPressed, bool &mouseMiddleIsPressed)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        mouseRightIsPressed = true;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
    {
        mouseMiddleIsPressed = true;
    }
}

// Função executada toda vez que há um redimensionamento de janela
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{

    glViewport(0, 0, width, height);
}
