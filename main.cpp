#include <GL/glut.h>
#include <iostream>
#include <cmath>

float hCarX = -0.9, vCarY = -0.9;
float boat1X = -0.9, boat2X = 0.6;
float waveOffset = 0.0;
bool allowHorizontal = true;
float windmillAngle = 0.0f; // for rotating windmill

void drawRoads() {
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -0.1f); glVertex2f(1.0f, -0.1f);
    glVertex2f(1.0f, 0.1f); glVertex2f(-1.0f, 0.1f);
    glEnd();

    glBegin(GL_QUADS);
    glVertex2f(-0.1f, -1.0f); glVertex2f(0.1f, -1.0f);
    glVertex2f(0.1f, 1.0f); glVertex2f(-0.1f, 1.0f);
    glEnd();
}

void drawTrafficLight() {
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.05f, 0.05f); glVertex2f(0.05f, 0.05f);
    glVertex2f(0.05f, 0.2f); glVertex2f(-0.05f, 0.2f);
    glEnd();

    if (allowHorizontal)
        glColor3f(0.0f, 1.0f, 0.0f);
    else
        glColor3f(1.0f, 0.0f, 0.0f);

    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++) {
        float theta = i * 3.14159f / 180;
        glVertex2f(0.0f + 0.02f * cos(theta), 0.13f + 0.02f * sin(theta));
    }
    glEnd();
}

void drawCar(float x, float y, bool horizontal) {
    glColor3f(1.0f, 0.0f, 0.0f);
    if (horizontal) {
        glBegin(GL_QUADS);
        glVertex2f(x, y - 0.04f); glVertex2f(x + 0.2f, y - 0.04f);
        glVertex2f(x + 0.2f, y + 0.04f); glVertex2f(x, y + 0.04f);
        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f);
        for (float offset : {0.03f, 0.15f}) {
            glBegin(GL_POLYGON);
            for (int i = 0; i < 360; ++i)
                glVertex2f(x + offset + 0.015f * cos(i * 3.14159f / 180),
                           y - 0.05f + 0.015f * sin(i * 3.14159f / 180));
            glEnd();
        }
    } else {
        glBegin(GL_QUADS);
        glVertex2f(x - 0.04f, y); glVertex2f(x + 0.04f, y);
        glVertex2f(x + 0.04f, y + 0.2f); glVertex2f(x - 0.04f, y + 0.2f);
        glEnd();

        glColor3f(0.0f, 0.0f, 0.0f);
        for (float offset : {0.03f, 0.15f}) {
            glBegin(GL_POLYGON);
            for (int i = 0; i < 360; ++i)
                glVertex2f(x - 0.05f + 0.015f * cos(i * 3.14159f / 180),
                           y + offset + 0.015f * sin(i * 3.14159f / 180));
            glEnd();
        }
    }
}

void drawRiver() {
    glColor3f(0.0f, 0.5f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, 0.3f); glVertex2f(1.0f, 0.3f);
    glVertex2f(1.0f, 0.6f); glVertex2f(-1.0f, 0.6f);
    glEnd();

    glColor3f(0.8f, 0.9f, 1.0f);
    for (float i = -1.0f; i < 1.0f; i += 0.2f) {
        glBegin(GL_LINE_STRIP);
        for (float j = 0; j <= 1.0f; j += 0.01f) {
            float x = i + j;
            float y = 0.45f + 0.02f * sin(10 * x + waveOffset);
            glVertex2f(x, y);
        }
        glEnd();
    }
}

void drawBoat(float x, float y) {
    glColor3f(0.6f, 0.3f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(x, y); glVertex2f(x + 0.2f, y);
    glVertex2f(x + 0.15f, y + 0.03f); glVertex2f(x + 0.05f, y + 0.03f);
    glEnd();

    glColor3f(0.8f, 0.7f, 0.2f);
    glBegin(GL_QUADS);
    glVertex2f(x + 0.06f, y + 0.03f); glVertex2f(x + 0.14f, y + 0.03f);
    glVertex2f(x + 0.14f, y + 0.06f); glVertex2f(x + 0.06f, y + 0.06f);
    glEnd();
}

void drawWindmill() {
  float cx = 0.75f, cy = -0.75f; // Moved to bottom-right

  // Stand
  glColor3f(0.4f, 0.4f, 0.4f);
  glBegin(GL_QUADS);
  glVertex2f(cx - 0.01f, cy - 0.3f); glVertex2f(cx + 0.01f, cy - 0.3f);
  glVertex2f(cx + 0.01f, cy); glVertex2f(cx - 0.01f, cy);
  glEnd();

  // Hub
  glColor3f(0.7f, 0.7f, 0.7f);
  glBegin(GL_POLYGON);
  for (int i = 0; i < 360; ++i)
      glVertex2f(cx + 0.015f * cos(i * 3.14159f / 180),
                 cy + 0.015f * sin(i * 3.14159f / 180));
  glEnd();

  // Blades
  glPushMatrix();
  glTranslatef(cx, cy, 0.0f);
  glRotatef(windmillAngle, 0.0f, 0.0f, 1.0f);

  for (int i = 0; i < 3; ++i) {
      glRotatef(120.0f, 0.0f, 0.0f, 1.0f);
      glColor3f(0.9f, 0.9f, 0.9f);
      glBegin(GL_TRIANGLES);
      glVertex2f(0.0f, 0.0f);
      glVertex2f(0.0f, 0.15f);
      glVertex2f(0.02f, 0.1f);
      glEnd();
  }

  glPopMatrix();
}


void drawTree(float x, float y) {
    // Trunk
    glColor3f(0.55f, 0.27f, 0.07f);
    glBegin(GL_QUADS);
    glVertex2f(x - 0.01f, y); glVertex2f(x + 0.01f, y);
    glVertex2f(x + 0.01f, y + 0.05f); glVertex2f(x - 0.01f, y + 0.05f);
    glEnd();

    // Leaves
    glColor3f(0.0f, 0.5f, 0.0f);
    for (float offset : {0.0f, 0.015f}) {
        glBegin(GL_POLYGON);
        for (int i = 0; i < 360; ++i)
            glVertex2f(x + 0.03f * cos(i * 3.14159f / 180),
                       y + 0.05f + offset + 0.03f * sin(i * 3.14159f / 180));
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    drawRiver();
    drawBoat(boat1X, 0.35f);
    drawBoat(boat2X, 0.48f);

    drawRoads();
    drawTrafficLight();
    drawCar(hCarX, 0.0f, true);
    drawCar(0.0f, vCarY, false);

    drawTree(-0.8f, -0.3f);
    drawTree(0.6f, 0.15f);
    drawTree(-0.4f, -0.35f);

    drawWindmill();

    glutSwapBuffers();
}

void update(int) {
    if (allowHorizontal) {
        hCarX += 0.005f;
        if (hCarX > 1.2f) hCarX = -1.2f;
    } else {
        vCarY += 0.005f;
        if (vCarY > 1.2f) vCarY = -1.2f;
    }

    boat1X += 0.002f;
    if (boat1X > 1.2f) boat1X = -1.2f;
    boat2X -= 0.002f;
    if (boat2X < -1.2f) boat2X = 1.2f;

    waveOffset += 0.05f;
    windmillAngle += 2.0f;
    if (windmillAngle >= 360.0f) windmillAngle -= 360.0f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void handleKey(unsigned char key, int, int) {
    if (key == 'r') allowHorizontal = false;
    else if (key == 'g') allowHorizontal = true;
}

void init() {
    glClearColor(0.8f, 0.9f, 1.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-1, 1, -1, 1);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(900, 700);
    glutCreateWindow("🚦 Traffic Simulation with River, Boats, Windmill & Trees - by Riya");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(handleKey);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
