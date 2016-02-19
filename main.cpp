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
void funKeyboard(int key, int x, int y);
void funMouse(int key, int state, int x, int y);
void funMotion(int x, int y);
void drawNumber();
void drawSphere(GLfloat radius, char color, bool wired = true);
void drawPieza(GLfloat width, GLfloat height);
void drawCube();

// Variables globales
GLfloat desZ = -5.0, rotY = 0.0, rotX = 0.0;

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
    glutSpecialFunc(funKeyboard);
    glutMouseFunc(funMouse);
    glutMotionFunc(funMotion);

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

void funKeyboard(int key, int x, int y) {

    switch (key) {
    case GLUT_KEY_UP:
        rotX = rotX - 5.0f;
        break;
    case GLUT_KEY_DOWN:
        rotX = rotX + 5.0f;
        break;
    case GLUT_KEY_RIGHT:
        rotY = rotY + 5.0f;
        break;
    case GLUT_KEY_LEFT:
        rotY = rotY - 5.0f;
        break;
    default:
        desZ = -5.0f;
        rotY = 0.0f;
        rotX = 0.0f;
        break;
    }

    glutPostRedisplay();
}

void funMouse(int key, int state, int x, int y)
{
    if (state == GLUT_UP)
        return;

    oldX = x;
    oldY = y;
    /*if (x > 1000 / 2.0f)
        rotY += 5.0f;
    else if (x < 1000 / 2.0f)
        rotY -= 5.0f;

    if (y > 500 / 2.0f)
        rotX += 5.0f;
    else if (y < 500.0f)
        rotX -= 5.0f;

    printf("x: %u, y: %u \n", x, y);*/
}

void funMotion(int x, int y)
{
    rotY -= float(oldX - x) / 10.0f;
    rotX -= float(oldY - y) / 10.0f;

    oldY = y;
    oldX = x;
    
    printf("x: %d, y: %d \n", x, y);

    glutPostRedisplay();
}

void funDisplay() {

    // Borramos el buffer de color y el de profundidad
    glClearColor(SCREEN_COLOR);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Posicionamos la cámara (V)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt();

    GLfloat initialDist = -2.0f;

    // Dibujamos los objetos (M)
    /*glPushMatrix();
    {
        glTranslatef(initialDist, 0.0, -8.0);
        glRotatef(rotY, 0.0, 1.0, 0.0);
        glTranslatef(0.0, 0.0, 1.0);
        drawNumber();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(initialDist, 0.0, -8.0);
        glRotatef(rotY, 0.0, 1.0, 0.0);
        drawSphere(2.0f, 'y');
    }
    glPopMatrix();

    initialDist += 4.0f;
    
    glPushMatrix();
    {
        glTranslatef(initialDist, 0.0, -8.0);
        glRotatef(rotY, 0.0, 1.0, 0.0);
        drawSphere(0.5f, 'b');
    }
    glPopMatrix();
    
    initialDist += 1.5f;

    glPushMatrix();
    {
        glTranslatef(initialDist, 0.0, -8.0);
        glRotatef(rotY, 0.0, 1.0, 0.0);
        drawSphere(0.1f, 'w');
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(initialDist, 0.0, -8.0);
        glRotatef(rotY, 0.0, 1.0, 0.0);
        drawPieza(3.0f, 1.0f);
    }
    glPopMatrix();*/
    
    glPushMatrix();
    {
        glTranslatef(0.0, 0.0, -5.0);
        glRotatef(rotY, 0.0, 1.0, 0.0);
        glRotatef(rotX, 1.0, 0.0, 0.0);
        drawCube();
    }
    glPopMatrix();

    // Intercambiamos los buffers
    glutSwapBuffers();
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

void drawSphere(GLfloat radius, char color, bool wired)
{
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

    if (wired)
        glutWireSphere(radius, 20, 20);
    else
        glutSolidSphere(radius, 20, 20);
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
