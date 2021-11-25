#include <jellyfish/Jellyfish.hpp>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, bool &gameIsPaused, bool &mouseMiddleisPressed, bool &rIsPressed);

// Global
// Tamanho da janela
unsigned int WIDTH = 1600; //1280;
unsigned int HEIGHT = 900; //720;

unsigned int vao;
unsigned int vbo;

// Texturas
unsigned int texturePlayer;
unsigned int textureBullet;
unsigned int textureBullet1;
unsigned int textureBullet2;
unsigned int textureBullet3;
unsigned int textureBullet4;

unsigned int textureHitBox;

// Colors
float black[4] = {0.0f, 0.0f, 0.0f, 1.0f};
float white[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float blue[4] = {0.0f, 1.0f, 1.0f, 1.0f};
float red[4] = {1.0f, 0.0f, 0.0f, 1.0f};
float magenta[4] = {1.0f, 1.0f, 0.0f, 1.0f};

FILE *f1;
char laser1[100000];

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

    textureBullet1 = createTexture("bin/assets/Bullet1.png");
    textureBullet2 = createTexture("bin/assets/Bullet2.png");
    textureBullet3 = createTexture("bin/assets/Bullet3.png");
    textureBullet4 = createTexture("bin/assets/Bullet4.png");

    textureHitBox = createTexture("bin/assets/Hitbox.png");

    unsigned int texture1 = createTexture("bin/assets/Nave.png");
    unsigned int textureAlien1 = createTexture("bin/assets/Alien1.png");
    unsigned int textureAlien2 = createTexture("bin/assets/Alien2.png");
    unsigned int textureAlien3 = createTexture("bin/assets/Alien3.png");
    unsigned int textureAlien4 = createTexture("bin/assets/Alien4.png");
    //unsigned int texture4 = createTexture("bin/assets/Alien1-256.png");
    unsigned int textureBackground = createTexture("bin/assets/Background.png");

    // Audio
    f1 = fopen("bin\\assets\\laser1.wav", "r");
    fread(&laser1, sizeof(char), 100000, f1);
    fclose(f1);

    mciSendString("open \"bin\\assets\\SpaceTheme.mp3\" type mpegvideo alias mp3", NULL, 0, NULL);
    mciSendString("play mp3 repeat", NULL, 0, NULL);

    while (!glfwWindowShouldClose(window))
    {

        Player player;

        Alien alien1(-1.0f, 0.8f, textureAlien1, 1, 1);
        Alien alien2(-1.0f, 0.65f, textureAlien2, 1, 2);
        Alien alien3(-1.0f, 0.50f, textureAlien3, 1, 3);
        Alien alien4(-0.85f, 0.05f, textureAlien4, 1, 4);

        AlienSquad wave1(alien1, 0.0f, 0.0f, 1, 5, 150);
        AlienSquad wave2(alien2, 0.0f, 0.0f, 1, 5, 60);
        AlienSquad wave3(alien3, 0.0f, 0.0f, 3, 5, 30);
        AlienSquad wave4(alien4, 0.0f, 0.0f, 1, 3, 20);

        Entity background(0, 0, 2.0f * WIDTH / HEIGHT, 2.0f, textureBackground, 1);
        //Entity test(0.5f, 0, texturePlayer, 3);

        double prevTime = 0.0;
        double crntTime = 0.0;
        double timeDiff;
        unsigned int counter = 0;

        bool gameIsPaused = false;
        bool mouseRightIsPressed = false;
        bool mouseMiddleIsPressed = false;
        bool rIsPressed = false;
        bool Reset = false;
        bool Won = false;
        int pauseCooldown = 20;

        // Loop da aplicação
        while (!Reset && player.ship.active && !glfwWindowShouldClose(window))
        {
            Reset = false;
            // Input handling
            processInput(window, mouseRightIsPressed, mouseMiddleIsPressed, rIsPressed);

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

                if (rIsPressed && pauseCooldown == 0)
                {
                    Reset = true;
                    print("RESET");
                    pauseCooldown = 20;
                }

                if (pauseCooldown > 0)
                    pauseCooldown--;

                mouseRightIsPressed = false;

                player.keyUpdate(window);
            }

            // Real loop ------------------------------------------------------------------------------------------

            // Define a cor de fundo da janela
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            // Limpa algum buffer específico
            glClear(GL_COLOR_BUFFER_BIT);

            background.draw();
            wave1.update(player);
            wave2.update(player);
            wave3.update(player);
            wave4.update(player);

            player.gun.interact(wave1);
            player.gun.interact(wave2);
            player.gun.interact(wave3);
            player.gun.interact(wave4);

            if (!wave1.Alive && !wave2.Alive && !wave3.Alive && !wave4.Alive)
            {
                Won = true;
                break;
            }

            player.update();

            wave1.draw();
            wave2.draw();
            wave3.draw();
            wave4.draw();

            player.draw();

            // Main do jogo Pausado
            if (gameIsPaused)
            {
                player.ship.hitbox.draw();
                player.gun.drawBulletsHitBoxes();

                wave1.drawHitbox();
                wave2.drawHitbox();
                wave3.drawHitbox();
                wave4.drawHitbox();
            }
            // Faz a troca do framebuffer antigo para o novo (double buffer)
            glfwSwapBuffers(window);

            // Render ---------------------------------------------------------------------------------------------

            while (gameIsPaused && !glfwWindowShouldClose(window))
            {
                mouseMiddleIsPressed = false;
                processInput(window, mouseRightIsPressed, mouseMiddleIsPressed, rIsPressed);

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
                        wave2.debug();
                        wave3.debug();
                        wave4.debug();

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

        if (Won)
        {
            print("Voce Ganhou !!! ");
            std::cout << "Score " << player.gun.Score << std::endl;
        }
        else
        {
            print("Voce Perdeu :( ");
            std::cout << "Score " << player.gun.Score << std::endl;
        }
    }
    // Desalocar memória dos objetos instanciados
    glfwTerminate(); // Faz a limpeza dos recursos utilizados pelo GLFW
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window, bool &mouseRightIsPressed, bool &mouseMiddleIsPressed, bool &rIsPressed)
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
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        print("R pressed");
        rIsPressed = true;
    }
}

// Função executada toda vez que há um redimensionamento de janela
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{

    glViewport(0, 0, width, height);
}
