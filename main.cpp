#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <thread>

#include "BezierCurve.h"
#include "util/RenderUtil.h"


void loop();

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::thread gameThread(loop);
    gameThread.join();

    std::cout << "Goodbye World!" << std::endl;

    return 0;
}

// static members
int screenWidth = 800;
int screenHeight = 600;
double mouseX = -1;
double mouseY = -1;
double mouseScroll = 0;
bool resized = false;
bool keys[GLFW_KEY_LAST];

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    switch (action) {
        case GLFW_PRESS:
            keys[key] = true;
            break;
        case GLFW_RELEASE:
            keys[key] = false;
            break;
        case GLFW_REPEAT:
            break;
        default:
            std::cerr << "Key action \"" << action << "\" not handled" << std::endl;
    }
}

void characterCallback(GLFWwindow *window, unsigned int codepoint) {}

void cursorPosCallback(GLFWwindow *window, double xpos, double ypos) {
    mouseX = xpos;
    mouseY = ypos;
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    mouseScroll = yoffset;
}

void resizeCallback(GLFWwindow *window, int width, int height) {
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, width, height);
    resized = true;
}


void loop() {
    if(!glfwInit()) {
        std::cerr << "Failed to initalize GLFW" << std::endl;
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
#endif

    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "Cell", nullptr, nullptr);
    if(window == nullptr) {
        std::cerr << "Window creation failed" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = true; // for core profile
    if(glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return;
    }

    // enable srgb
    //glEnable(GL_FRAMEBUFFER_SRGB);

    // cull triangles facing away from camera
    //glEnable(GL_CULL_FACE);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Setup keyboard inputs
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCharCallback(window, characterCallback);
    // Setup mouse inputs
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    // Window resize
    glfwSetFramebufferSizeCallback(window, resizeCallback);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    GLuint program = RenderUtil::loadShaderProgram("vertex.shader", "fragment.shader");
    glUseProgram(program);
    GLuint mvpUniform = glGetUniformLocation(program, "MVP");
    glm::mat4 mvpMatrix = glm::ortho(0.f, 1.f*screenWidth, 1.f*screenHeight, 0.f);
    glUniformMatrix4fv(mvpUniform, 1, GL_FALSE, &mvpMatrix[0][0]);

    std::vector<glm::vec2> hull = {
            {120, 160},
            {35, 200},
            {220, 260},
            {220, 40}
    };
    std::vector<unsigned int> hullIndices;
    std::vector<glm::vec4> hullColors;
    for(int i = 0; i < hull.size(); i++) {
        hullIndices.push_back(i);
        hullColors.emplace_back(0, 0, 1, 1);
    }

    BezierCurve bc{hull, 0.01};
    std::vector<glm::vec2> bezier = bc.bezierNaive();
    std::vector<unsigned int> bezierIndices;
    std::vector<glm::vec4> bezierColors;
    for(int i = 0; i < bezier.size(); i++) {
        bezierIndices.push_back(i);
        bezierColors.emplace_back(1, 0, 0, 1);
    }

    GLuint bezierId;
    glGenVertexArrays(1, &bezierId);
    glBindVertexArray(bezierId);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    GLuint bezierBuffer;
    glGenBuffers(1, &bezierBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, bezierBuffer);
    glVertexAttribPointer(
            0,                  // attribute
            2,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void *) 0            // array buffer offset
    );
    glBufferData(GL_ARRAY_BUFFER, bezier.size() * sizeof(glm::vec2), &bezier[0], GL_STATIC_DRAW);
    GLuint bezierColorBuffer;
    glGenBuffers(1, &bezierColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, bezierColorBuffer);
    glVertexAttribPointer(
            1,                  // attribute
            4,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void *) 0            // array buffer offset
    );
    glBufferData(GL_ARRAY_BUFFER, bezierColors.size() * sizeof(glm::vec4), &bezierColors[0], GL_STATIC_DRAW);
    GLuint bezierElements;
    glGenBuffers(1, &bezierElements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierElements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, bezierIndices.size() * sizeof(unsigned int), &bezierIndices[0], GL_STATIC_DRAW);

    GLuint hullId;
    glGenVertexArrays(1, &hullId);
    glBindVertexArray(hullId);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    GLuint hullBuffer;
    glGenBuffers(1, &hullBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, hullBuffer);
    glVertexAttribPointer(
            0,                  // attribute
            2,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void *) 0            // array buffer offset
    );
    glBufferData(GL_ARRAY_BUFFER, hull.size() * sizeof(glm::vec2), &hull[0], GL_STATIC_DRAW);
    GLuint hullColorBuffer;
    glGenBuffers(1, &hullColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, hullColorBuffer);
    glVertexAttribPointer(
            1,                  // attribute
            4,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void *) 0            // array buffer offset
    );
    glBufferData(GL_ARRAY_BUFFER, hullColors.size() * sizeof(glm::vec4), &hullColors[0], GL_STATIC_DRAW);
    GLuint hullElements;
    glGenBuffers(1, &hullElements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, hullElements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, hullIndices.size() * sizeof(unsigned int), &hullIndices[0], GL_STATIC_DRAW);

    std::cout << "Loop start" << std::endl;
    do {
        glClear(GL_COLOR_BUFFER_BIT);

        // resize if flag set
        if (resized) {
            resized = false;
        }

        glBindVertexArray(bezierId);
        glDrawElements(GL_LINE_STRIP, bezierIndices.size(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(hullId);
        glDrawElements(GL_LINE_STRIP, hullIndices.size(), GL_UNSIGNED_INT, nullptr);

        // reset mouse wheel position
        mouseScroll = 0;

        // glfw: swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    if (hullId != 0)
        glDeleteVertexArrays(1, &hullId);
    if (hullBuffer != 0)
        glDeleteBuffers(1, &hullBuffer);
    if (hullElements != 0)
        glDeleteBuffers(1, &hullElements);

    if (bezierId != 0)
        glDeleteVertexArrays(1, &bezierId);
    if (bezierBuffer != 0)
        glDeleteBuffers(1, &bezierBuffer);
    if (bezierElements != 0)
        glDeleteBuffers(1, &bezierElements);


    glfwTerminate();
}
