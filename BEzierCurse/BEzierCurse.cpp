#include <stdio.h>
#include <iostream>
#include <GL/glut.h>
#include<algorithm>
#include<math.h>
using namespace std;
#define WL 800
#define WW 600

GLboolean mouseLeftDown = false;
GLint k = -1;
GLfloat mousex;
GLfloat mousey;

struct data {
    GLfloat x;
    GLfloat y;
}point[5] = { {10,10},{10,100},{100,100},{100,10} ,{150,5} };//是比例不是实际坐标

//初始化
void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);                           //窗口背景色设置为白色
    glMatrixMode(GL_PROJECTION);                                    //指定设置投影参数
    gluOrtho2D(0.0, 200.0, 0.0, 150.0);                             //设置投影参数
}

//鼠标点击调回函数
void pickTarget(int button, int state, int x, int y) {
    mousex = x;
    mousey = y;
    switch (button) {
    case GLUT_LEFT_BUTTON: 
        if (state == GLUT_DOWN) { 
            GLfloat dis = 0;
            mouseLeftDown = true;
            for (int i = 0; i < 6; i++) {
                dis = sqrt((point[i].x / 200.0 * WL - (GLfloat)x) * (point[i].x / 200.0 * WL - (GLfloat)x) + (((150 - point[i].y) / 150 * WW-(GLfloat)y) * ((150 - point[i].y) / 150 * WW- (GLfloat)y)));
                if (dis < 5.0f) {
                    k = i;
                    break;
                }
            }
        }
        else if (state == GLUT_UP) {
            mouseLeftDown = false;
            k = -1;
        }
        break;
    default:break;
    }
    glutPostRedisplay();
}

//鼠标移动调回函数
void mouseMotion(int x, int y) {
    if (mouseLeftDown && k != -1) {
        cout << x <<";;;"<< y << endl;
        GLfloat mouseX = x / (GLfloat)WL * 200.0f;
        GLfloat mouseY = 150.0f - y / (GLfloat)WW * 150.0f;
        cout << mouseX << " change " << mouseY << endl;
        point[k].x = mouseX;
        point[k].y = mouseY;
        glutPostRedisplay();
    }

}

//窗口调回函数
void bezierDisplay(void) {
    glClear(GL_COLOR_BUFFER_BIT);                                   //背景色设置为窗口颜色
    glPointSize(6);
    glColor3f(0.0, 0.0, 1.0);                                       //控制点的颜色
    
    
    for (int i = 0; i < 6; i++) {
        glBegin(GL_POINTS);
        glVertex2f(point[i].x, point[i].y);
        glEnd();
    }
    
    glBegin(GL_LINES);
        glVertex2f(point[0].x, point[0].y);
        glVertex2f(point[1].x, point[1].y);
        glVertex2f(point[1].x, point[1].y);
        glVertex2f(point[2].x, point[2].y);
        glVertex2f(point[2].x, point[2].y);
        glVertex2f(point[3].x, point[3].y);
        glVertex2f(point[3].x, point[3].y);
        glVertex2f(point[4].x, point[4].y);
    glEnd();
                                                         //清空openGL命令缓冲区，执行openGL命令
    glColor3f(0.60, 0.38, 0.10);

    glBegin(GL_LINE_STRIP);
        for (int i = 1; i <= 200; i++)
        {
            GLfloat t = i / 200.0f;
            GLfloat b0 = pow(1 - t, 4.0);
            GLfloat b1 = 4.0 * t * pow(1 - t, 3.0);
            GLfloat b2 = 6.0 * t * t * pow(1 - t, 2.0);
            GLfloat b3 = 4.0 * t * t * t* (1 - t);
            GLfloat b4 = t * t * t * t;
            //运用4次Bezier曲线
            GLfloat x = point[0].x * b0 + point[1].x * b1 + point[2].x * b2 + point[3].x * b3 + point[4].x * b4;
            GLfloat y = point[0].y * b0 + point[1].y * b1 + point[2].y * b2 + point[3].y * b3 + point[4].y * b4;

            glVertex2f(x, y);
        }
    glEnd();
    glFlush();
    
}






int main(int argc, char* argv[])
{   
    
    glutInit(&argc, argv);                                          //初始化GLUT库
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);                    //初始化窗口显示
   
    glutInitWindowPosition(300, 300);
    glutInitWindowSize(WL, WW);
    glutCreateWindow("Bezier曲线");

    init();
    glutDisplayFunc(bezierDisplay);
    
    glutMouseFunc(pickTarget);
    glutMotionFunc(mouseMotion);
    
    glutMainLoop();

    return 0;
}
