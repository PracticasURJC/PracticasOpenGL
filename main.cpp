#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cstdio>
#include <cmath>

#define SCREEN_SIZE     1000, 500
#define SCREEN_POSITION   50,  50
#define SCREEN_COLOR     0.0, 0.0, 0.0, 0.0

void initFunc();
void funReshape(int w, int h);
void funDisplay();
void drawTriangulo(GLfloat s, char color);
void funKeyboardUp(unsigned char key, int x, int y);
void funSpecial(int key, int x, int y);
void funMouse(int key, int state, int x, int y);
void funMotion(int x, int y);
void funMotionPassive(int x, int y);
void funMouseWheel(int wheel, int direction, int x, int y);
void drawNumber();
void drawSphere(GLfloat radius, char color, bool wired = true);
void drawPieza(GLfloat width, GLfloat height);
void drawCube();
void drawPlane(GLfloat width, GLfloat height);

// Variables globales
GLfloat desZ = -5.0, rotY = 0.0, rotX = 0.0, posX = 0.0, posY = 0.0;

GLfloat cameraPos[3]    = { 0.0, 0.0, 5.0 };
GLfloat lookat[3]       = { 0.0, 0.0, -5.0 };
GLfloat up[3]           = { 0.0,  1.0,  0.0 };

int oldX = 0, oldY = 0;

int main(int argc, char** argv) {

    // Inicializamos OpenGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Inicializamos la Ventana
    glutInitWindowSize(SCREEN_SIZE);
    glutInitWindowPosition(SCREEN_POSITION);
    glutCreateWindow("Sesion 3.1");

    // Inicializaciones específicas
    initFunc();

    // Configuración CallBacks
    glutReshapeFunc(funReshape);
    glutDisplayFunc(funDisplay);
    glutKeyboardUpFunc(funKeyboardUp);
    glutSpecialFunc(funSpecial);
    glutMouseFunc(funMouse);
    glutMotionFunc(funMotion);
    //glutPassiveMotionFunc(funMotionPassive);
    glutIdleFunc(funDisplay);
    glutMouseWheelFunc(funMouseWheel);

    // Bucle principal
    glutMainLoop();

    // Destrucción de objetos

    return(0);
}


void initFunc() {

    // Inicializamos GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        printf("Error: %s\n", glewGetErrorString(err));
    }
    printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    // Configuracion de parametros fijos
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_OFFSET_FILL);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glPolygonOffset(1.0, 1.0);
    glShadeModel(GL_FLAT);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void funReshape(int w, int h) {

    // Configuramos el Viewport
    glViewport(0, 0, w, h);

    // Configuracion del modelo de proyeccion (P)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 3.0, 50.0);
}

void funKeyboardUp(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
        cameraPos[1] = cameraPos[1] + 1.0f;
        lookat[1] = lookat[1] + 1.0f;
        break;
    case 's':
        cameraPos[1] = cameraPos[1] - 1.0f;
        lookat[1] = lookat[1] - 1.0f;
        break;
    case 'a':
        cameraPos[0] = cameraPos[0] - 1.0f;
        lookat[0] = lookat[0] - 1.0f;
        break;
    case 'd':
        cameraPos[0] = cameraPos[0] + 1.0f;
        lookat[0] = lookat[0] + 1.0f;
        break;
    case ' ':
        cameraPos[2] = cameraPos[2] - 1.0f;
        lookat[2] = lookat[2] - 1.0f;
        break;
    }

    printf("KEYBOARD: key: %c, x: %d, y: %d \n", key, x, y);
}

void funSpecial(int key, int x, int y)
{

    switch (key)
    {
    case GLUT_KEY_UP:
        cameraPos[1] = cameraPos[1] + 1.0f;
        lookat[1] = lookat[1] + 1.0f;
        break;
    case GLUT_KEY_DOWN:
        cameraPos[1] = cameraPos[1] - 1.0f;
        lookat[1] = lookat[1] - 1.0f;
        break;
    case GLUT_KEY_RIGHT:
        cameraPos[0] = cameraPos[0] + 1.0f;
        lookat[0] = lookat[0] + 1.0f;
        break;
    case GLUT_KEY_LEFT:
        cameraPos[0] = cameraPos[0] - 1.0f;
        lookat[0] = lookat[0] - 1.0f;
        break;
    default:
        break;
    }

    //glutPostRedisplay();
    printf("KEYBOARD SPECIAL: key: %d, x: %d, y: %d \n", key, x, y);
}

void funMouse(int key, int state, int x, int y)
{
    oldX = x;
    oldY = y;

    printf("MOUSE: key: %d, state: %d, x: %d, y: %d \n", key, state, x, y);
}

void funMotionPassive(int x, int y)
{
    cameraPos[0] -= float(oldX - x) / 10.0f;
    lookat[0] = float(oldX - x) / 10.0f;
    cameraPos[1] += float(oldY - y) / 10.0f;
    lookat[1] = float(oldY - y) / 10.0f;
    
    oldX = x;
    oldY = y;
    
    printf("PASSIVE: x: %d, y: %d \n", x, y);

    glutPostRedisplay();
}

void funMotion(int x, int y)
{
    cameraPos[0] -= float(oldX - x) / 100.0f;
    lookat[0] = float(oldX - x) / 100.0f;
    cameraPos[1] += float(oldY - y) / 100.0f;
    lookat[1] = float(oldY - y) / 100.0f;
    
    oldX = x;
    oldY = y;

    printf("MOTION: x: %d, y: %d \n", x, y);

    glutPostRedisplay();
}

void funMouseWheel(int wheel, int direction, int x, int y)
{
    cameraPos[2] -= direction * 0.3;

    printf("MOUSEWHEEL: wheel: %d, direction: %d, x: %d, y: %d \n", wheel, direction, x, y);
}

GLfloat initialX = -2.0;
GLfloat yellowSphereInit[3] = {0.0, 2.0, -8.0};
GLfloat blueSphereInit[3] = {4.0, 2.0, -8.0};
GLfloat whiteSphereInit[3] = {-1.5, 2.0, -8.0};

void funDisplay() {

    // Borramos el buffer de color y el de profundidad
    glClearColor(SCREEN_COLOR);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Posicionamos la cámara (V)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    rotY += GLfloat(0.02);

 // Posicionamos la cámara (V)
    gluLookAt( cameraPos[0], cameraPos[1], cameraPos[2],
                  lookat[0],    lookat[1],    lookat[2],
                      up[0],        up[1],        up[2]);
    
    // Dibujamos los objetos (M)
    glPushMatrix();
    {
        glTranslatef(yellowSphereInit[0], yellowSphereInit[1] - 5.0, yellowSphereInit[2]);
        glRotatef(90.0, 0.0, 1.0, 0.0);
        glRotatef(90.0, 1.0, 0.0, 0.0);
        drawPlane(20.0, 20.0);
    }
    glPopMatrix();

    // Dibujamos los objetos (M)
    glPushMatrix();
    {
        glTranslatef(yellowSphereInit[0], yellowSphereInit[1], yellowSphereInit[2]);
        glRotatef(rotY, 0.0, 1.0, 0.0);
        glRotatef(rotX, 1.0, 0.0, 0.0);
        glTranslatef(0.0, 0.0, 1.0);
        drawNumber();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(yellowSphereInit[0], yellowSphereInit[1], yellowSphereInit[2]);
        glRotatef(rotY, 0.0, 1.0, 0.0);
        glRotatef(rotX, 1.0, 0.0, 0.0);
        drawSphere(2.0f, 'y');
    }
    glPopMatrix();
    
    glPushMatrix();
    {
        glTranslatef(yellowSphereInit[0], yellowSphereInit[1], yellowSphereInit[2]);
        glRotatef(rotY, 0.0, 1.0, 0.0);
        glRotatef(rotX, 1.0, 0.0, 0.0);
        glTranslatef(blueSphereInit[0], 0.0, 0.0);
        drawSphere(0.5f, 'b');
    }
    glPopMatrix();
    
    glPushMatrix();
    {
        glTranslatef(yellowSphereInit[0], yellowSphereInit[1], yellowSphereInit[2]);
        glRotatef(rotY, 0.0, 1.0, 0.0);
        glRotatef(rotX, 1.0, 0.0, 0.0);
        glTranslatef(blueSphereInit[0], 0.0, 0.0);
        glRotatef(rotY, 0.0, 1.0, 0.0);
        glRotatef(rotX, 1.0, 0.0, 0.0);
        glTranslatef(whiteSphereInit[0], 0.0, 0.0);
        drawSphere(0.1f, 'w');
    }
    glPopMatrix();

    /*glPushMatrix();
    {
        glTranslatef(initialDist, 0.0, -8.0);
        glRotatef(rotY, 0.0, 1.0, 0.0);
        drawPieza(3.0f, 1.0f);
    }
    glPopMatrix();
    
    glPushMatrix();
    {
        glTranslatef(posX, posY, -5.0);
        glRotatef(rotY, 0.0, 1.0, 0.0);
        glRotatef(rotX, 1.0, 0.0, 0.0);
        drawCube();
    }
    glPopMatrix();
    
    glPushMatrix();
    {
        glTranslatef(initialDist, 0.0, -8.0);
        glRotatef(rotY, 0.0, 1.0, 0.0);
        drawSphere(1.0f, 'y');
    }
    glPopMatrix();*/

    // Intercambiamos los buffers
    glutSwapBuffers();
    glutPostRedisplay();
}

void drawTriangulo(GLfloat s, char color) {

    switch (color) {
    case 'r':
        glColor3f(1.0, 0.0, 0.0);
        break;
    case 'g':
        glColor3f(0.0, 1.0, 0.0);
        break;
    case 'b':
        glColor3f(0.0, 0.0, 1.0);
        break;
    default:
        glColor3f(1.0, 1.0, 1.0);
    }

    glBegin(GL_TRIANGLES);
    glVertex3f(-s, -s, 0.0); // v1
    glVertex3f(s, -s, 0.0); // v2
    glVertex3f(0.0, s, 0.0); // v3
    glEnd();
}

void drawNumber()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(5.0f);

    glBegin(GL_LINE_LOOP);
    {
        glVertex3f(0.4f, 1.7f, 0.0f);
        glVertex3f(0.4f, -1.7f, 0.0f);
        glVertex3f(-0.4f, -1.7f, 0.0f);
        glVertex3f(-0.4f, 0.4f, 0.0f);
        glVertex3f(-1.0f, 0.4f, 0.0f);
        glVertex3f(-0.4f, 1.7f, 0.0f);
    }
    glEnd();
}

void drawPlane(GLfloat width, GLfloat height)
{
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(width, height, 0.0f);
        glVertex3f(width, -height, 0.0f);
        glVertex3f(-width, -height, 0.0f);
        
        glVertex3f(-width, -height, 0.0f);
        glVertex3f(-width, height, 0.0f);
        glVertex3f(width, height, 0.0f);
    }
    glEnd();
}

void drawSphere(GLfloat radius, char color, bool wired)
{
    glEnable(GL_CULL_FACE);
    switch (color)
    {
    case 'r':
        glColor3f(1.0, 0.0, 0.0);
        break;
    case 'g':
        glColor3f(0.0, 1.0, 0.0);
        break;
    case 'b':
        glColor3f(0.0, 0.0, 1.0);
        break;
    case 'y':
        glColor3f(1.0, 1.0, 0.0);
        break;
    case 'w':
    default:
        glColor3f(1.0, 1.0, 1.0);
    }

    glLineWidth(1.0f);

    glEnable(GL_POLYGON_OFFSET_FILL);
    //glutSolidSphere(radius, 20, 20);

    glDisable(GL_POLYGON_OFFSET_FILL);
    glutWireSphere(radius, 20, 20);

    /*if (wired)
        glutWireSphere(radius, 20, 20);
    else
        glutSolidSphere(radius, 20, 20);*/

    glDisable(GL_CULL_FACE);
}

void drawPieza(GLfloat width, GLfloat height)
{
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glLineWidth(1.0f);

    glBegin(GL_TRIANGLES);
    {
        glVertex3f(width, height, 0.0f);
        glVertex3f(width, -height, 0.0f);
        glVertex3f(-width, -height, 0.0f);
        
        glVertex3f(-width, -height, 0.0f);
        glVertex3f(-width, height, 0.0f);
        glVertex3f(width, height, 0.0f);
    }
    glEnd();
}

void drawCube()
{
    glLineWidth(1.0f);
    glBegin(GL_TRIANGLES);
    {
        glColor3f(1.0f, 1.0f, 1.0f);

        glVertex3f(1.0, 1.0, -1.0f);
        glVertex3f(-1.0, -1.0, -1.0f);
        glVertex3f(1.0, -1.0, -1.0f);

        glVertex3f(-1.0, -1.0, -1.0f);
        glVertex3f(1.0, 1.0, -1.0f);
        glVertex3f(-1.0, 1.0, -1.0f);
        
        glColor3f(0.0f, 1.0f, 0.0f);

        glVertex3f(1.0, 1.0, 1.0f);
        glVertex3f(-1.0, -1.0, 1.0f);
        glVertex3f(1.0, -1.0, 1.0f);
        
        glVertex3f(-1.0, -1.0, 1.0f);
        glVertex3f(1.0, 1.0, 1.0f);
        glVertex3f(-1.0, 1.0, 1.0f);
        
        glColor3f(1.0f, 0.0f, 0.0f);

        glVertex3f(1.0, 1.0, -1.0f);
        glVertex3f(1.0, -1.0, -1.0f);
        glVertex3f(1.0, -1.0, 1.0f);
        
        glVertex3f(1.0, -1.0, 1.0f);
        glVertex3f(1.0, 1.0, 1.0f);
        glVertex3f(1.0, 1.0, -1.0f);
        
        glColor3f(0.0f, 0.0f, 1.0f);

        glVertex3f(-1.0, 1.0, -1.0f);
        glVertex3f(-1.0, -1.0, -1.0f);
        glVertex3f(-1.0, -1.0, 1.0f);
        
        glVertex3f(-1.0, -1.0, 1.0f);
        glVertex3f(-1.0, 1.0, 1.0f);
        glVertex3f(-1.0, 1.0, -1.0f);
        
        glColor3f(0.5f, 0.5f, 1.0f);

        glVertex3f(1.0, 1.0, -1.0f);
        glVertex3f(-1.0, 1.0, 1.0f);
        glVertex3f(-1.0, 1.0, -1.0f);
        
        glVertex3f(1.0, 1.0, 1.0f);
        glVertex3f(-1.0, 1.0, 1.0f);
        glVertex3f(1.0, 1.0, -1.0f);
        
        glColor3f(0.5f, 1.0f, 0.5f);

        glVertex3f(1.0, -1.0, -1.0f);
        glVertex3f(-1.0, -1.0, 1.0f);
        glVertex3f(-1.0, -1.0, -1.0f);
        
        glVertex3f(1.0, -1.0, 1.0f);
        glVertex3f(-1.0, -1.0, 1.0f);
        glVertex3f(1.0, -1.0, -1.0f);
    }
    glEnd();
}
