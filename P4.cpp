#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>

// Constants
const double c = 1.0;  // Chord length
const double m = 0.0;  // Maximum camber (percentage of chord)
const double p = 0.0;  // Location of maximum camber (percentage of chord)
const double t = 0.12; // Thickness-to-chord ratio

// Function to calculate the thickness of the airfoil at a given x-coordinate
double thickness(double x) {
    return (t / 0.2) * (0.2969 * sqrt(x / c) - 0.1260 * (x / c) - 0.3516 * pow(x / c, 2) + 0.2843 * pow(x / c, 3) - 0.1015 * pow(x / c, 4));
}

// Function to calculate the camber line of the airfoil at a given x-coordinate
double camberLine(double x) {
    if (x < p * c) {
        return (m / pow(p, 2)) * (2 * p * (x / c) - pow(x / c, 2));
    }
    else {
        return (m / pow(1 - p, 2)) * ((1 - 2 * p) + 2 * p * (x / c) - pow(x / c, 2));
    }
}

// Function to calculate the y-coordinate of the upper surface of the airfoil
double upperSurface(double x) {
    return camberLine(x) + thickness(x) / 2;
}

// Function to calculate the y-coordinate of the lower surface of the airfoil
double lowerSurface(double x) {
    return camberLine(x) - thickness(x) / 2;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "NACA 0012 Airfoil", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-c / 2, c * 1.5, -t / 2, t / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw X and Y axis
        glBegin(GL_LINES);
        glColor3f(1.0, 1.0, 1.0); // White color
        glVertex2f(-1.0, 0.0);
        glVertex2f(1.0, 0.0); // X axis
        glVertex2f(0.0, -1.0);
        glVertex2f(0.0, 1.0); // Y axis
        glEnd();


        // Draw airfoil
        glBegin(GL_LINE_STRIP);
        glColor3f(0, 1.0f, 0);
        for (double x = 0.0; x <= c; x += 0.01) {
            glVertex2f(x, upperSurface(x));
        }
        for (double x = c; x >= 0.0; x -= 0.01) {
            glVertex2f(x, lowerSurface(x));
        }
        glEnd();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}