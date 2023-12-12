// Include C++ headers
#include <iostream>
#include <string>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Shader loading utilities and other
#include <common/camera.h>
#include <common/model.h>
#include <common/shader.h>
#include <common/texture.h>
#include <common/util.h>

// extra helper functions
#include "extra/rooms/main_room.h"
#include "extra/paintings/paintings.h"
#include "extra/player/player.h"
#include "extra/light/light.h"

using namespace std;
using namespace glm;

// Function prototypes
void initialize();
void createContext();
void mainLoop();
void free();

#define W_WIDTH 1024
#define W_HEIGHT 768
#define TITLE "Art Gallery"

// Global variables
GLFWwindow *window;
Camera *camera;
GLuint shaderProgram;
GLuint MLocation, VLocation, PLocation, colorLocation;

Room *currentRoom;
Room *rooms[6];
MainRoom *mainRoom;
vector<Painting *> paintings;
Player *player = new Player();
Light *light = new Light(vec3(0, 5, 0), vec4(1, 1, 1, 1), 1.0f, 10.0f);

enum GameState
{
    MAINROOM,
    ROOM1,
    ROOM2,
    ROOM3,
    ROOM4,
    ROOM5,
};

void change_state();
void set_paintings_visibility();

GameState gameState = MAINROOM;

void createContext()
{
    shaderProgram = loadShaders("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");

    MLocation = glGetUniformLocation(shaderProgram, "M");
    VLocation = glGetUniformLocation(shaderProgram, "V");
    PLocation = glGetUniformLocation(shaderProgram, "P");
    colorLocation = glGetUniformLocation(shaderProgram, "color");

    float roomRadius = 10.0f,
          roomHeight = 7.5f;
    int numPaintings = 5,
        wallPoints = 50;

    rooms[0] = new MainRoom(roomHeight, roomRadius, wallPoints);
    for (int i = 1; i < 6; i++)
    {
        rooms[i] = new SecondaryRoom(roomHeight, roomHeight * 1.5);
    }
    currentRoom = rooms[0];
    // currentRoom = new SecondaryRoom(roomHeight, roomHeight);
    paintings = createPaintings(numPaintings, roomHeight * 0.8, roomHeight * 0.6, roomHeight / 2, roomRadius);
    std::cout << "created paintings" << std::endl;
}

void free()
{
    glDeleteProgram(shaderProgram);
    glfwTerminate();
}

void mainLoop()
{
    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        static double lastTime = glfwGetTime();

        currentRoom = rooms[gameState];

        // Compute time difference between current and last frame
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);

        // camera
        player->move(window, deltaTime, camera->horizontalAngle);
        if (!currentRoom->isInside(player->position))
        {
            player->velocity = vec3(0, player->velocity.y, 0);
            player->position = player->prevPosition;
            player->updatePosition(camera->horizontalAngle, deltaTime);
        }
        player->updateBoundingBox();

        camera->position = player->position + vec3(0, player->height, 0);
        camera->update();
        mat4 projectionMatrix = camera->projectionMatrix;
        mat4 viewMatrix = camera->viewMatrix;

        glUniformMatrix4fv(VLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(PLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

        // Draw wire frame triangles or fill: GL_LINE, or GL_FILL
        glUseProgram(shaderProgram);

        light->position.y = currentRoom->height;
        light->upload_to_shaders(shaderProgram);

        //// Draw bounding box. To be removed
        // Drawable *playerDrawable = new Drawable(player->boundingBox);
        // playerDrawable->bind();
        // mat4 playerM = mat4(1.0f);
        // glUniformMatrix4fv(MLocation, 1, GL_FALSE, &playerM[0][0]);
        // glUniform3f(colorLocation, 1.0f, 0.0f, 0.0f);
        // playerDrawable->draw();

        // Draw currentRoom
        currentRoom->draw(MLocation, colorLocation);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Draw paintings
        for (auto &painting : paintings)
        {
            painting->draw(MLocation, colorLocation);

            if (painting->checkCollision(player))
            {
                cout << "Collision" << endl;
            }
        }

        change_state();
        glfwSwapBuffers(window);
        glfwPollEvents();

        lastTime = currentTime;

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
}

void change_state()
{
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        gameState = ROOM1;
        player->position = vec3(0, 0, 0.5);
        camera->horizontalAngle = -3.14f;
        camera->verticalAngle = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        gameState = ROOM2;
        player->position = vec3(0, 0, 0.5);
        camera->horizontalAngle = -3.14f;
        camera->verticalAngle = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        gameState = ROOM3;
        player->position = vec3(0, 0, 0.5);
        camera->horizontalAngle = -3.14f;
        camera->verticalAngle = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        gameState = ROOM4;
        player->position = vec3(0, 0, 0.5);
        camera->horizontalAngle = -3.14f;
        camera->verticalAngle = 0.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        gameState = ROOM5;
        player->position = vec3(0, 0, 0.5);
        camera->horizontalAngle = -3.14f;
        camera->verticalAngle = 0.0f;
    }
    set_paintings_visibility();
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
    {
        gameState = MAINROOM;
        player->position = vec3(0, 0, 0.0);
        camera->horizontalAngle = 0.0f;
        camera->verticalAngle = 0.0f;

        for (auto &painting : paintings)
        {
            painting->modelMatrix = painting->mainRoomModelMatrix;
            painting->visible = true;
        }
    }
}

void set_paintings_visibility()
{
    if (gameState == MAINROOM)
    {
        return;
    }
    for (auto &painting : paintings)
    {
        painting->visible = false;
    }
    paintings[gameState - 1]->visible = true;
    paintings[gameState - 1]->modelMatrix = paintings[gameState - 1]->sideRoomModelMatrix;
}

void initialize()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        throw runtime_error("Failed to initialize GLFW\n");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(W_WIDTH, W_HEIGHT, TITLE, NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        throw runtime_error(string(string("Failed to open GLFW window.") +
                                   " If you have an Intel GPU, they are not 3.3 compatible." +
                                   "Try the 2.1 version.\n"));
    }
    glfwMakeContextCurrent(window);

    // Start GLEW extension handler
    glewExperimental = GL_TRUE;

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        throw runtime_error("Failed to initialize GLEW\n");
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Hide the mouse and enable unlimited movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, W_WIDTH / 2, W_HEIGHT / 2);

    // Gray background color
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Enable blending for transparency
    // change alpha in fragment shader
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Cull triangles which normal is not towards the camera
    // glEnable(GL_CULL_FACE);

    // Log
    logGLParameters();

    // Create camera
    camera = new Camera(window);
}

int main(void)
{
    try
    {
        initialize();
        createContext();
        mainLoop();
        free();
    }
    catch (exception &ex)
    {
        cout << ex.what() << endl;
        getchar();
        free();
        return -1;
    }
    return 0;
}