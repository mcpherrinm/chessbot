#include <GL/glfw.h>
#include <float.h>
#include <math.h>
#include <stdio.h>

#define M_PI 3.14159265358979323846

float p[4][2] = { 250.0, 100.0, 0,0,0,0,0,0 };
float theta[4] = { 0.3, 0.6, M_PI, 0.0 };
float length[3] = {234.0, 300.0, 60.0};
int grip = 0;

#define DT 0.008
#define WIDTH 1290
#define HEIGHT 720
#define DELAY 0.01
#define SPEED 1000

float min(float a, float b) { return a > b ? b : a; }

void solve(float tx, float ty) {
  float minerror = 10000000.0;
  for (float x = -M_PI/2.0; x < M_PI/2.0; x += DT) {
    for (float y = x; y < min(M_PI, x + 13.0*M_PI/16.0); y+=DT) {
      float error = fabs(tx - (p[0][0] + sin(x)*length[0] + sin(y)*length[1] + sin(theta[2])*length[2]))
                  + fabs(ty - (p[0][1] + cos(x)*length[0] + cos(y)*length[1] + cos(theta[2])*length[2]));
      if (error < minerror) {
        minerror = error;
        theta[0] = x;
        theta[1] = y;
      }
    }
  }
  p[1][0] = p[0][0] + sin(theta[0])*length[0];
  p[1][1] = p[0][1] + cos(theta[0])*length[0];
  p[2][0] = p[1][0] + sin(theta[1])*length[1];
  p[2][1] = p[1][1] + cos(theta[1])*length[1];
  p[3][0] = p[2][0] + sin(theta[2])*length[2];
  p[3][1] = p[2][1] + cos(theta[2])*length[2];

}

void sayshit() {
  static float old_theta0 = FLT_MAX;
  static float old_theta1 = FLT_MAX;
  static float old_theta2 = FLT_MAX;
  static float old_theta3 = FLT_MAX;
  static float old_grip = FLT_MAX;
  float theta0 = 1500.0 - 800*theta[0]/M_PI*2.0;
  float theta1 = 1450.0 + 800.0*((theta[1]-theta[0])/M_PI*2.0 - 1.0);
  float theta2 = 1600.0 + 250.0*((theta[2] - theta[1] - theta[0])/M_PI*2.0 - 1.0);
  float theta3 = 1500.0 + 1000.0*theta[3]/M_PI*2.0;
  if (theta0 != old_theta0) printf("#1 P%d S%d\r", (int)theta0, SPEED);
  if (theta1 != old_theta1) printf("#2 P%d S%d\r", (int)theta1, SPEED);
  if (theta3 != old_theta3) printf("#0 P%d S%d\r", (int)theta3, SPEED);

  if (old_grip != grip) {
    if (grip) printf("#4 P900\r");
    else      printf("#4 P1800 S1000\r");
  }
  old_theta0 = theta0;
  old_theta1 = theta1;
  old_theta2 = theta2;
  old_theta3 = theta3;
  old_grip = grip;
  fflush(stdout);
}

int main() {
  glfwInit();
  glfwOpenWindow(WIDTH, HEIGHT, 8,8,8,8,0,0,GLFW_WINDOW);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
  glMatrixMode(GL_MODELVIEW);

  int running = 1;
  double t0 = 0.0;
  while (running) {
    int x, y;
    glfwGetMousePos(&x, &y);
    y = HEIGHT - y;
    theta[3] = (double)y/(double)HEIGHT*2.0-1.0;
    solve(x, p[0][1]+20);
    //solve(x, y);
    double t = glfwGetTime();
    if (t - t0 > DELAY) {
      grip = !glfwGetMouseButton(GLFW_MOUSE_BUTTON_1);
      t0 = t;
      sayshit();
    }

    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINES);
    glColor3f(1,0,0);
    glVertex2f(p[0][0], p[0][1]);
    glVertex2f(p[1][0], p[1][1]);
    glColor3f(0,1,0);
    glVertex2f(p[1][0], p[1][1]);
    glVertex2f(p[2][0], p[2][1]);
    glColor3f(0,0,1);
    glVertex2f(p[2][0], p[2][1]);
    glVertex2f(p[3][0], p[3][1]);
    glEnd();
    glfwSwapBuffers();
    running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
  }
  theta[0] = -0.840797;
  theta[1] = 1.129202;
  grip = 1;
  sayshit();
}
