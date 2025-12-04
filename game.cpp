#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

struct Ball {
    float x, y;
    float dy;
    float r, g, b;
};

float personX = 0.0f;
float personHeight = 0.3f;
bool gameOver = false;
int score = 0;
std::vector<Ball> balls;

void drawCircle(float x, float y, float r, int segments = 50) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y);
    for (int i = 0; i <= segments; ++i) {
        float theta = 2.0f * 3.1415926f * i / segments;
        glVertex2f(x + r * cosf(theta), y + r * sinf(theta));
    }
    glEnd();
}

void drawPerson(float x) {
    glColor3f(1, 1, 1); // white
    float y = -0.8f;

    // Head
    drawCircle(x, y + personHeight + 0.05f, 0.05f);

    // Body
    glBegin(GL_LINES);
    glVertex2f(x, y + personHeight);
    glVertex2f(x, y);
    // Arms
    glVertex2f(x - 0.05f, y + personHeight - 0.05f);
    glVertex2f(x + 0.05f, y + personHeight - 0.05f);
    // Legs
    glVertex2f(x, y);
    glVertex2f(x - 0.05f, y - 0.1f);
    glVertex2f(x, y);
    glVertex2f(x + 0.05f, y - 0.1f);
    glEnd();
}

void drawBalls() {
    for (auto& ball : balls) {
        glColor3f(ball.r, ball.g, ball.b);
        drawCircle(ball.x, ball.y, 0.05f);
    }
}

bool checkCollision(float ballX, float ballY) {
    float px = personX, py = -0.8f + 0.15f; // head center
    float dist = sqrt((px - ballX) * (px - ballX) + (py - ballY) * (py - ballY));
    return dist < 0.1f;
}

void drawText(float x, float y, std::string str) {
    glRasterPos2f(x, y);
    for (char c : str) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    if (gameOver) {
        glColor3f(1, 0, 0);
        drawText(-0.2f, 0.0f, "Game Over");
        drawText(-0.3f, -0.1f, "Final Score: " + std::to_string(score));
    }
    else {
        drawPerson(personX);
        drawBalls();
        glColor3f(1, 1, 0); // yellow
        drawText(-0.95f, 0.9f, "Score: " + std::to_string(score));
    }

    glutSwapBuffers();
}

void update(int value) {
    if (!gameOver) {
        for (auto& ball : balls) {
            ball.y -= ball.dy;

            // Respawn ball at top if it goes below screen
            if (ball.y < -1.1f) {
                ball.y = 1.0f;
                ball.x = -0.9f + static_cast<float>(rand()) / RAND_MAX * 1.8f;
                ball.r = static_cast<float>(rand()) / RAND_MAX;
                ball.g = static_cast<float>(rand()) / RAND_MAX;
                ball.b = static_cast<float>(rand()) / RAND_MAX;
            }

            if (checkCollision(ball.x, ball.y)) {
                gameOver = true;
            }
        }

        // Increase score over time
        score++;
    }

    glutPostRedisplay();
    glutTimerFunc(50, update, 0);
}

void handleKeys(int key, int x, int y) {
    float speed = 0.05f;
    if (key == GLUT_KEY_LEFT && personX > -0.9f) personX -= speed;
    if (key == GLUT_KEY_RIGHT && personX < 0.9f) personX += speed;
}

void initBalls(int count) {
    for (int i = 0; i < count; ++i) {
        Ball ball;
        ball.x = -0.9f + static_cast<float>(rand()) / RAND_MAX * 1.8f;
        ball.y = 0.5f + static_cast<float>(rand()) / RAND_MAX * 0.5f;
        ball.dy = 0.01f + static_cast<float>(rand()) / RAND_MAX * 0.005f;
        ball.r = static_cast<float>(rand()) / RAND_MAX;
        ball.g = static_cast<float>(rand()) / RAND_MAX;
        ball.b = static_cast<float>(rand()) / RAND_MAX;
        balls.push_back(ball);
    }
}

int main(int argc, char** argv) {
    srand(static_cast<unsigned>(time(0)));

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Bouncing Balls game");

    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-1, 1, -1, 1);

    initBalls(6); // 6 colorful falling balls

    glutDisplayFunc(display);
    glutSpecialFunc(handleKeys);
    glutTimerFunc(0, update, 0);

    glutMainLoop();
    return 0;
}
