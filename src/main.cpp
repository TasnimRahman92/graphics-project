#include <glad.h>
#include <glfw3.h>

#include <iostream>
#include <cmath>
#include <cstdlib>


void framebuffer_size_callback(GLFWwindow* window, int width, int height); // adjusts the viewport on window resize.
void processInput(GLFWwindow *window); //handles keyboard input (e.g., ESC to close the window).

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// =========================
// Animation Variables
// =========================
float carX = -1.2f;
float cloudX = -1.5f;

bool autoMove = true;

// Smooth Speed
float carSpeed = 0.0002f;
float cloudSpeed = 0.0001f;

// =========================
// Rain Control
// =========================
bool rainOn = true;
bool isNight = false;

// =========================
// Building Colors
// =========================
float building1R = 0.2f, building1G = 0.3f, building1B = 0.8f;
float building2R = 0.8f, building2G = 0.2f, building2B = 0.3f;
float building3R = 0.2f, building3G = 0.7f, building3B = 0.4f;
float building4R = 0.7f, building4G = 0.4f, building4B = 0.9f;
float building5R = 0.9f, building5G = 0.6f, building5B = 0.2f;

// =========================
// Rain Variables
// =========================
const int rainCount = 300;

float rainX[rainCount];
float rainY[rainCount];
float rainSpeed[rainCount];

// =========================
// Initialize Rain
// =========================
void initRain()
{
    for (int i = 0; i < rainCount; i++)
    {
        rainX[i] = ((rand() % 2000) / 1000.0f) - 1.0f;
        rainY[i] = ((rand() % 2000) / 1000.0f);

        rainSpeed[i] = 0.002f +
                       ((rand() % 100) / 70000.0f);
    }
}

// =========================
// Draw Circle
// =========================
void drawCircle(float x, float y, float radius,
                float r, float g, float b)
{
    glColor3f(r, g, b);

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(x, y);

    for (int i = 0; i <= 100; i++)
    {
        float angle = 2.0f * 3.1416f * i / 100;

        glVertex2f(
            x + cos(angle) * radius,
            y + sin(angle) * radius
        );
    }

    glEnd();
}

// =========================
// Draw Rectangle
// =========================
void drawRectangle(float x1, float y1,
                   float x2, float y2,
                   float r, float g, float b)
{
    glColor3f(r, g, b);

    glBegin(GL_QUADS);

    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);

    glEnd();
}

// =========================
// Draw Buildings
// =========================
void drawBuildings()
{
    drawRectangle(-1.0f, -0.2f, -0.75f, 0.45f,
                  building1R, building1G, building1B);

    drawRectangle(-0.72f, -0.2f, -0.45f, 0.65f,
                  building2R, building2G, building2B);

    drawRectangle(-0.42f, -0.2f, -0.15f, 0.35f,
                  building3R, building3G, building3B);

    drawRectangle(-0.12f, -0.2f, 0.18f, 0.75f,
                  building4R, building4G, building4B);

    drawRectangle(0.22f, -0.2f, 0.55f, 0.50f,
                  building5R, building5G, building5B);

    // Windows
    for (float x = -0.95f; x < -0.78f; x += 0.08f)
    {
        for (float y = -0.1f; y < 0.35f; y += 0.15f)
        {
            drawRectangle(x, y,
                          x + 0.04f, y + 0.07f,
                          1.0f, 1.0f, 0.5f);
        }
    }

    for (float x = -0.67f; x < -0.48f; x += 0.08f)
    {
        for (float y = -0.1f; y < 0.55f; y += 0.15f)
        {
            drawRectangle(x, y,
                          x + 0.04f, y + 0.07f,
                          1.0f, 1.0f, 0.5f);
        }
    }

    for (float x = -0.37f; x < -0.18f; x += 0.08f)
    {
        for (float y = -0.1f; y < 0.25f; y += 0.15f)
        {
            drawRectangle(x, y,
                          x + 0.04f, y + 0.07f,
                          1.0f, 1.0f, 0.5f);
        }
    }

    for (float x = -0.05f; x < 0.14f; x += 0.08f)
    {
        for (float y = -0.1f; y < 0.65f; y += 0.15f)
        {
            drawRectangle(x, y,
                          x + 0.04f, y + 0.07f,
                          1.0f, 1.0f, 0.5f);
        }
    }

    for (float x = 0.28f; x < 0.50f; x += 0.08f)
    {
        for (float y = -0.1f; y < 0.40f; y += 0.15f)
        {
            drawRectangle(x, y,
                          x + 0.04f, y + 0.07f,
                          1.0f, 1.0f, 0.5f);
        }
    }
}

// =========================
// Draw Road
// =========================
void drawRoad()
{
    drawRectangle(-1.0f, -1.0f,
                  1.0f, -0.45f,
                  0.15f, 0.15f, 0.15f);

    for (float x = -1.0f; x < 1.0f; x += 0.3f)
    {
        drawRectangle(x, -0.72f,
                      x + 0.15f, -0.68f,
                      1.0f, 1.0f, 0.0f);
    }
}

// =========================
// Draw Car
// =========================
void drawCar()
{
    glPushMatrix();

    glTranslatef(carX, 0.0f, 0.0f);

    // Body
    drawRectangle(-0.18f, -0.67f,
                  0.18f, -0.55f,
                  1.0f, 0.0f, 0.0f);

    // Top
    glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_POLYGON);

    glVertex2f(-0.10f, -0.55f);
    glVertex2f(-0.05f, -0.45f);
    glVertex2f(0.08f, -0.45f);
    glVertex2f(0.14f, -0.55f);

    glEnd();

    // Glass
    drawRectangle(-0.045f, -0.53f,
                  0.04f, -0.47f,
                  0.5f, 0.9f, 1.0f);

    drawRectangle(0.055f, -0.53f,
                  0.11f, -0.47f,
                  0.5f, 0.9f, 1.0f);

    // Wheels
    drawCircle(-0.10f, -0.68f,
               0.045f,
               0.0f, 0.0f, 0.0f);

    drawCircle(0.10f, -0.68f,
               0.045f,
               0.0f, 0.0f, 0.0f);

    drawCircle(-0.10f, -0.68f,
               0.020f,
               0.8f, 0.8f, 0.8f);

    drawCircle(0.10f, -0.68f,
               0.020f,
               0.8f, 0.8f, 0.8f);

    glPopMatrix();
}

// =========================
// Draw Cloud
// =========================
void drawCloud(float x, float y)
{
    drawCircle(x, y, 0.07f,
               1.0f, 1.0f, 1.0f);

    drawCircle(x + 0.07f, y + 0.03f,
               0.08f,
               1.0f, 1.0f, 1.0f);

    drawCircle(x + 0.15f, y,
               0.07f,
               1.0f, 1.0f, 1.0f);

    drawCircle(x + 0.08f, y - 0.03f,
               0.07f,
               1.0f, 1.0f, 1.0f);
}
void drawStar(float x, float y)
{
    drawCircle(x, y, 0.01f, 1.0f, 1.0f, 1.0f);
}

// =========================
// Draw Rain
// =========================
void drawRain()
{
    glColor3f(0.8f, 0.8f, 1.0f);

    glBegin(GL_LINES);

    for (int i = 0; i < rainCount; i++)
    {
        glVertex2f(rainX[i], rainY[i]);

        glVertex2f(
            rainX[i] - 0.01f,
            rainY[i] - 0.05f
        );
    }

    glEnd();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// =========================
// Keyboard Input
// =========================
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Start Auto Move
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        autoMove = true;

    // Stop Auto Move
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        autoMove = false;

    // Rain ON
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        rainOn = true;

    // Rain OFF
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        rainOn = false;

    // Manual Move
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        carX -= 0.003f;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        carX += 0.003f;

    // Building 1 Color Change
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        building1R = (float)rand() / RAND_MAX;
        building1G = (float)rand() / RAND_MAX;
        building1B = (float)rand() / RAND_MAX;
    }

    // Building 2 Color Change
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        building2R = (float)rand() / RAND_MAX;
        building2G = (float)rand() / RAND_MAX;
        building2B = (float)rand() / RAND_MAX;
    }

    // Building 3 Color Change
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        building3R = (float)rand() / RAND_MAX;
        building3G = (float)rand() / RAND_MAX;
        building3B = (float)rand() / RAND_MAX;
    }

    // Building 4 Color Change
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        building4R = (float)rand() / RAND_MAX;
        building4G = (float)rand() / RAND_MAX;
        building4B = (float)rand() / RAND_MAX;
    }

    // Building 5 Color Change
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        building5R = (float)rand() / RAND_MAX;
        building5G = (float)rand() / RAND_MAX;
        building5B = (float)rand() / RAND_MAX;
    
    }
    // Night Mode ON
if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    isNight = true;

// Day Mode ON
if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    isNight = false;
}

// =========================
// Main Function
// =========================
int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(
        900,
        600,
        "Smooth Rainy City Animation",
        NULL,
        NULL
    );

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glViewport(0, 0, 900, 600);
    // 2D Projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-1, 1, -1, 1, -1, 1);

    glMatrixMode(GL_MODELVIEW);

    // Initialize Rain
    initRain();

    // =========================
    // Main Loop
    // =========================
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        // =========================
        // Smooth Car Animation
        // =========================
        if (autoMove)
        {
            carX += carSpeed;

            if (carX > 1.3f)
                carX = -1.3f;
        }

        // =========================
        // Smooth Cloud Animation
        // =========================
        cloudX += cloudSpeed;

        if (cloudX > 1.5f)
            cloudX = -1.7f;

        // =========================
        // Rain Animation
        // =========================
        if (rainOn)
        {
            for (int i = 0; i < rainCount; i++)
            {
                rainY[i] -= rainSpeed[i];

                if (rainY[i] < -1.0f)
                {
                    rainY[i] = 1.0f;

                    rainX[i] =
                    ((rand() % 2000) / 1000.0f) - 1.0f;
                }
            }
        }

        // =========================
        // Background
        // =========================
        glClearColor(0.20f, 0.20f, 0.28f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glLoadIdentity();
// Dynamic Background (Day/Night/Rain)

// Sky Color
if (isNight)
    drawRectangle(-1.0f, -0.2f, 1.0f, 1.0f, 0.05f, 0.05f, 0.15f);
else if (rainOn)
    drawRectangle(-1.0f, -0.2f, 1.0f, 1.0f, 0.25f, 0.25f, 0.30f);
else
    drawRectangle(-1.0f, -0.2f, 1.0f, 1.0f, 0.52f, 0.80f, 0.95f);

// Ground
drawRectangle(-1.0f, -0.45f, 1.0f, -0.2f, 0.2f, 0.5f, 0.2f);

// Sun / Moon
if (isNight)
{
    drawCircle(0.75f, 0.75f, 0.08f, 0.9f, 0.9f, 0.9f);

    drawStar(-0.8f, 0.9f);
    drawStar(-0.6f, 0.85f);
    drawStar(-0.3f, 0.92f);
    drawStar(0.1f, 0.88f);
    drawStar(0.4f, 0.93f);
    drawStar(0.6f, 0.87f);
}
else if (!rainOn && !isNight)
{
    drawCircle(0.75f, 0.75f, 0.10f, 1.0f, 0.8f, 0.0f);
}

        // Clouds
        // Clouds only when raining (cloudy weather)
if (rainOn && !isNight)
{
    drawCloud(cloudX, 0.80f);
    drawCloud(cloudX + 0.6f, 0.65f);
    drawCloud(cloudX + 1.2f, 0.78f);
}

        // Rain
        if (rainOn)
            drawRain();

        // Buildings
        drawBuildings();

        // Road
        drawRoad();

        // Car
        drawCar();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
} 