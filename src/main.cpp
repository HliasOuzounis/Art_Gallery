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
#include "extra/main_room/main_room.h"
#include "extra/paintings/paintings.h"
#include "extra/player/player.h"

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
GLuint MVPLocation, colorLocation;

Drawable *mainRoom;
vector<Painting *> paintings;
Player *player = new Player();

void createContext()
{
    shaderProgram = loadShaders("src/shaders/vertex.glsl", "src/shaders/fragment.glsl");

    MVPLocation = glGetUniformLocation(shaderProgram, "MVP");
    colorLocation = glGetUniformLocation(shaderProgram, "color");

    float roomRadius = 10.0f,
          roomHeight = 7.5f;

    mainRoom = create_room(roomHeight, roomRadius, 30);
    paintings = createPaintings(6, roomHeight * 0.8, roomHeight * 0.6, roomHeight/2, roomRadius);
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

        // Compute time difference between current and last frame
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);

        // camera
        player->move(window,deltaTime, camera->horizontalAngle);
        camera->position = player->position;
        camera->update();
        mat4 projectionMatrix = camera->projectionMatrix;
        mat4 viewMatrix = camera->viewMatrix;

        glUseProgram(shaderProgram);

        // Draw wire frame triangles or fill: GL_LINE, or GL_FILL
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        // Draw mainRoom
        mat4 mainRoomModelMatrix = glm::mat4(1.0);
        mat4 mainRoomMVP = projectionMatrix * viewMatrix * mainRoomModelMatrix;

        vec3 roomColor = vec3(0.0, 0.0, 0.0);
        mainRoom->bind();
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &mainRoomMVP[0][0]);
        glUniform3f(colorLocation, roomColor.x, roomColor.y, roomColor.z);
        mainRoom->draw();

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Draw paintings
        mat4 paintingsModelMatrix = glm::mat4(1.0);
        mat4 paintingsMVP = projectionMatrix * viewMatrix * paintingsModelMatrix;
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &paintingsMVP[0][0]);

        for (auto & painting : paintings)
        {
            painting->drawable->bind();
            glUniform3f(colorLocation, painting->color.x, painting->color.y, painting->color.z);
            painting->drawable->draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

        lastTime = currentTime;

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
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
    glEnable(GL_CULL_FACE);

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