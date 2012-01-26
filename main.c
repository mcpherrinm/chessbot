#include <GL/glfw.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "al5d.h"


float p[4][2] = {{ 350.0, 200.0}, {0,0},{0,0},{0,0} };
float theta[4] = { 0.3, 0.6, M_PI, 0.0 };
float rtheta[4] = { 7*M_PI/8, M_PI/12, M_PI, 0.0 }; // (relative angles) make this match the theta values better
int grip = 0;
arm_state st;
int debug;

#define DT 0.03
#define WIDTH 1280
#define HEIGHT 760
#define DELAY 0.01
#define SPEED 2000

#define GRIP_BIAS 20.0

float min(float a, float b) { return a > b ? b : a; }
float max(float a, float b) { return a < b ? b : a; }

void abs_to_rel(float* t, float* rt) {
    rt[0] = t[0];
    rt[1] = t[1]-t[0] + M_PI;
    rt[2] = t[2]-t[1] + M_PI;
}

void rel_to_abs(float* rt, float* t) {
    t[0] = rt[0];
    t[1] = rt[1]+rt[0] - M_PI;
    t[2] = rt[2]+t[1]- M_PI;
}

// distance from the arm to the mouse
float distance(float rtheta0, float rtheta1, float rtheta2, float x0, float y0) {
    // get absolute angles from relative
    float theta0 = rtheta0;
    float theta1 = rtheta1 - (M_PI - theta0);
    float theta2 = rtheta2 - (M_PI - theta1);

    // x and y values for tip of gripper
    float x = cos(theta0)*length[0] + cos(theta1)*length[1] + cos(theta2)*length[2];
    float y = sin(theta0)*length[0] + sin(theta1)*length[1] + sin(theta2)*length[2];

    // the actual measure
    return pow(x-x0,2) + pow(y-y0,2) + GRIP_BIAS*pow(theta2 + M_PI/2,2);
    // L1 measure
    //return fabs(x-x0) + fabs(y-y0) + GRIP_BIAS*fabs(theta2 + M_PI/2);
}

// for stupid solver
void solveCircles(float l1, float l2, float tx, float ty, float* t1, float* t2) {
    // using cosine law and sine law to find the angles
    float ds = pow(tx,2) + pow(ty,2);

    float rt = acos( (pow(l1,2)+pow(l2,2)-ds)/(2*l1*l2) );
    *t1 = asin(l2*sin(rt)/sqrt(ds));
    *t2 = rt + (*t1) - M_PI;

    float rotate = atan2(ty,tx);
    if (rotate < 0) rotate += M_PI;
    *t1 += rotate;
    *t2 += rotate;
}

// stupid solver
void ssolve(float tx, float ty) {
    float d = sqrt(pow(tx,2) + pow(ty,2));
    float d_constrained = sqrt(pow(tx, 2) + pow(ty+length[2], 2));

    if (d > (length[0]+length[1]+length[2]) ) {
        theta[0] = atan2(ty,tx);
        if (rtheta[0] < 0) rtheta[0] += M_PI;
        theta[1] = theta[0];
        theta[2] = theta[0];
    }
    else if (d_constrained > (length[0]+length[1])) {
        solveCircles(length[0]+length[1], length[2], tx, ty, theta, theta+2);
        theta[1]=theta[0];
    }
    else {
        solveCircles(length[0], length[1], tx, ty + length[2], theta, theta+1);
        theta[2] = -M_PI/2;
    }

    abs_to_rel(theta, rtheta);

    rtheta[0] = min(rtheta[0],M_PI);
    rtheta[0] = max(rtheta[0],0);
    rtheta[1] = min(rtheta[1],M_PI);
    rtheta[1] = max(rtheta[1],M_PI/16);
    rtheta[2] = min(rtheta[2],M_PI);
    rtheta[2] = max(rtheta[2],M_PI/12);

    rel_to_abs(rtheta, theta);
}




void solve(float tx, float ty) {
  float minerror = 10000000.0;
  for (float rt0 = 0; rt0 < M_PI; rt0 += DT) {
    for (float rt1 = M_PI/16; rt1 < M_PI; rt1 += DT) {
      for (float rt2 = M_PI/12; rt2 < M_PI; rt2 += .1) {
        float error = distance(rt0, rt1, rt2, tx, ty);
        if (error < minerror) {
          minerror = error;
          rtheta[0] = rt0;
          rtheta[1] = rt1;
          rtheta[2] = rt2;
        }
      }
    }
  }

  // I should really make this coordinate change into a function
  theta[0] = rtheta[0];
  theta[1] = rtheta[1] - (M_PI - theta[0]);
  theta[2] = rtheta[2] - (M_PI - theta[1]);
}

void calcPoints() {
  p[1][0] = p[0][0] + cos(theta[0])*length[0];
  p[1][1] = p[0][1] + sin(theta[0])*length[0];
  p[2][0] = p[1][0] + cos(theta[1])*length[1];
  p[2][1] = p[1][1] + sin(theta[1])*length[1];
  p[3][0] = p[2][0] + cos(theta[2])*length[2];
  p[3][1] = p[2][1] + sin(theta[2])*length[2];

}

void sayshit() {
  /*float theta0 = calibration[0][0] + calibration[0][1]*(rtheta[0]/M_PI*2 - 1.0);
  float theta1 = calibration[1][0] - calibration[1][1]*(rtheta[1]/M_PI*2.0 - 1.0);
  float theta2 = calibration[2][0] + calibration[2][1]*(rtheta[2]/M_PI*2.0 - 1.0);
  float theta3 = calibration[3][0] + calibration[3][1]*theta[3]/M_PI*2.0;
  if (theta0 != old_theta0) printf("#1 P%d S%d\r", (int)theta0, SPEED);
  if (theta1 != old_theta1) printf("#2 P%d S%d\r", (int)theta1, SPEED);
  if (theta2 != old_theta2) printf("#3 P%d S%d\r", (int)theta2, SPEED);
  if (theta3 != old_theta3) printf("#0 P%d S%d\r", (int)theta3, SPEED);*/

  armSetRotation(&st, 1, rtheta[0]);
  armSetRotation(&st, 2, M_PI-rtheta[1]);
  armSetRotation(&st, 3, rtheta[2]);
  armSetRotation(&st, 0, theta[3]);

  if (grip)
    armSetRotation(&st, 4, M_PI/4.0);
  else
    armSetRotation(&st, 4, M_PI);

  armFlush(&st);
}

void initialize() {
    ssolve(102, 139);
    calcPoints();
    sayshit();
}


int main(int argc, char ** argv) {
  debug = argc == 1;
  char *device = debug ? 0 : "/dev/ttyUSB0";
  if(!armInit(&st, device)) {
    exit(1);
  }
  for(int i=0;i<6;++i)
    armSetSpeed(&st, i, SPEED);

  armSetRotation(&st, 5, M_PI/2);

  glfwInit();
  glfwOpenWindow(WIDTH, HEIGHT, 8,8,8,8,0,0,GLFW_WINDOW);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, WIDTH, 0, HEIGHT, -1, 1);
  glMatrixMode(GL_MODELVIEW);

  int running = 1;
  double t0 = glfwGetTime();
  initialize();
//  sleep(1);
  while (running) {
    int x, y, a;
    glfwGetMousePos(&x, &y);
    a = glfwGetMouseWheel();
    y = HEIGHT - y;
    x-= p[0][0];
    y-= p[0][1];
    theta[3] = (double)a*M_PI/(72) + M_PI/2;
    if (theta[3] < 0) theta[3] = 0;
    if (theta[3] > M_PI) theta[3] = M_PI;
    //solve(x, p[0][1]+20);
    ssolve(x, y);
    calcPoints();
    double t = glfwGetTime();
    if (t - t0 > DELAY) {
      grip = !glfwGetMouseButton(GLFW_MOUSE_BUTTON_1);
      t0 = t;
      sayshit();
      //printf("%f, %f, %f, %f\n", rtheta[0]*180/M_PI,rtheta[1]*180/M_PI,rtheta[2]*180/M_PI,theta[2]*180/M_PI);

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
  if(!debug) armClose(&st);
}
