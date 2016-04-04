#include <GL/glew.h>
#include <GL/freeglut.h>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <ctime>
#include "RgbImage.h"

#define SCREEN_SIZE     1000, 500
#define SCREEN_POSITION   50,  50
#define SCREEN_COLOR     0.0, 0.0, 0.0, 0.0
#define DOUBLE_CLICK_TIME 250
#define WITH_TEXTURES false
#define SCENE_SPEED 0.01


enum Scenes
{
    SCENE_PLANETS,
    SCENE_PIEZA,
    SCENE_ROOM
};

int scene = SCENE_ROOM;

void initFunc();
void funReshape(int w, int h);
void funDisplay();
void funIdle();
void drawTriangulo(GLfloat s, char color);
void funKeyboardUp(unsigned char key, int x, int y);
void funSpecial(int key, int x, int y);
void funMouse(int key, int state, int x, int y);
void funMotion(int x, int y);
void funMotionPassive(int x, int y);
void funMouseWheel(int wheel, int direction, int x, int y);
void drawNumber();
void drawSphere(GLfloat radius, char color, bool wired = true);
void drawPieza(GLfloat width, GLfloat height, char color);
void drawCube();
void drawPlane(GLfloat width, GLfloat height);
void drawFrame();
void drawRoom();
void drawInterface();
void selectColor(char color);
void initLights();
void initTextures();

GLfloat yellowSphereInit[3]     = { 0.0, 2.0, -8.0 };
GLfloat blueSphereInit[3]       = { 4.0, 2.0, -8.0 };
GLfloat whiteSphereInit[3]      = { -1.5, 2.0, -8.0 };

GLfloat flameHeight             = 1.0;
GLfloat flamePosition[3]        = { 3.0, 2.5, -2.0 };

GLfloat rotSun[3]               = { 0.0, 0.0, 0.0 };

GLfloat rotEarth[3]             = { 0.0, 0.0, 0.0 };
GLfloat translationEarth[3]     = { 30.0, 0.0, 0.0 };
GLfloat translationMoon[3]      = { 45.0, 0.0, 0.0 };

GLfloat translationBlue[3]      = { -6.0, 0.0, 0.0 };
GLfloat translationOrange[3]    = { 6.0, 0.0, 0.0 };
GLfloat rotationOrange[3]       = { 45.0, 0.0, 0.0 };
GLfloat translationGreen[3]     = { 6.0, 0.0, 0.0 };

GLfloat cameraPos[3]            = { 0.0, 0.0, 5.0 };
GLfloat lookat[3]               = { 0.0, 2.0, -8.0 };
GLfloat up[3]                   = { 0.0, 1.0,  0.0 };

GLfloat ambientLightIntensity[]   = { 0.2, 0.2, 0.2, 0.2 };

bool stopped = false;
bool moonIluminated = false;

int oldX = 0, oldY = 0;

int lastClickTime = 0;

float speed = SCENE_SPEED;

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
    glutPassiveMotionFunc(funMotionPassive);
    glutIdleFunc(funIdle);
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
    initLights();
    if (WITH_TEXTURES)
        initTextures();
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    glPolygonOffset(1.0, 1.0);
    glShadeModel(GL_SMOOTH);
    //glEnable(GL_NORMALIZE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    lastClickTime = glutGet(GLUT_ELAPSED_TIME);
}

void initLights()
{
    glEnable(GL_LIGHTING);
    /*glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHT0);
    GLfloat Ia[] = {0.5, 0.5, 0.5, 1.0};
    GLfloat Id[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat Ie[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat Lpos[] = {1.0, 1.0, 1.0, 0.0};//direccional
    glLightfv(GL_LIGHT0, GL_AMBIENT, Ia);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, Id);
    glLightfv(GL_LIGHT0, GL_SPECULAR, Ie);
    glLightfv(GL_LIGHT0, GL_POSITION, Lpos);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
    GLfloat Ldir[] = {-1.0, -1.0, 0.0};
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, Ldir);*/
    // Luz ambiente global
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLightIntensity);
 // Punto de vista infinito = GL_FALSE
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
 // Cálculo de la iluminación en ambas caras
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    
 // Modelo de Sombreado
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    if (scene == SCENE_PLANETS)
    {
        {
            // Brillo del sol
            GLfloat Ia0[] = { 0.5, 0.5, 0.5, 1.0 };
            GLfloat Id0[] = { 1.0, 1.0, 1.0, 1.0 };
            GLfloat Is0[] = { 1.0, 1.0, 0.0, 1.0 }; 
            glLightfv(GL_LIGHT0, GL_AMBIENT , Ia0);
            glLightfv(GL_LIGHT0, GL_DIFFUSE , Id0);
            glLightfv(GL_LIGHT0, GL_SPECULAR, Is0);
	        glLightfv(GL_LIGHT0, GL_POSITION, yellowSphereInit);
            glLightf (GL_LIGHT0, GL_CONSTANT_ATTENUATION , 0.50);
            glLightf (GL_LIGHT0, GL_LINEAR_ATTENUATION   , 0.01);
            glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01);
            glEnable(GL_LIGHT0);
        }
    
        {
            // Brillo de la luna
            GLfloat Ia0[] = { 0.3, 0.3, 0.3, 1.0 };
            GLfloat Id0[] = { 1.0, 1.0, 1.0, 1.0 };
            GLfloat Is0[] = { 1.0, 1.0, 1.0, 1.0 };
            glLightfv(GL_LIGHT1, GL_AMBIENT , Ia0);
            glLightfv(GL_LIGHT1, GL_DIFFUSE , Id0);
            glLightfv(GL_LIGHT1, GL_SPECULAR, Is0);
            glLightf (GL_LIGHT1, GL_CONSTANT_ATTENUATION , 0.90);
            glLightf (GL_LIGHT1, GL_LINEAR_ATTENUATION   , 0.05);
            glLightf (GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.01);
	        glLightfv(GL_LIGHT1, GL_POSITION, whiteSphereInit);
        }
    }
    else if (scene == SCENE_ROOM)
    {
        // Hoguera
        GLfloat Ia0[] = { 0.5, 0.5, 0.5, 1.0 };
        GLfloat Id0[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat Is0[] = { 1.0, 1.0, 0.0, 1.0 }; 
        glLightfv(GL_LIGHT0, GL_AMBIENT , Ia0);
        glLightfv(GL_LIGHT0, GL_DIFFUSE , Id0);
        glLightfv(GL_LIGHT0, GL_SPECULAR, ambientLightIntensity);
	    glLightfv(GL_LIGHT0, GL_POSITION, flamePosition);
        glLightf (GL_LIGHT0, GL_CONSTANT_ATTENUATION , 0.90);
        glLightf (GL_LIGHT0, GL_LINEAR_ATTENUATION   , 0.05);
        glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01);
        glEnable(GL_LIGHT0);
    }

 // Parámetros de la Luz 1 (posicional=bombilla)
    /*GLfloat Ia1[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat Id1[] = { 0.9, 0.9, 0.9, 1.0 };
    GLfloat Is1[] = { 0.8, 0.0, 0.0, 1.0 }; // Brillo en rojo para identificarla mejor
    glLightfv(GL_LIGHT1, GL_AMBIENT , Ia1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE , Id1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, Is1);
    glLightf (GL_LIGHT1, GL_CONSTANT_ATTENUATION , 0.90);
    glLightf (GL_LIGHT1, GL_LINEAR_ATTENUATION   , 0.05);
    glLightf (GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.01);
	GLfloat PL1[] = {-2.0, 1.0,-4.0, 1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, PL1);
    glEnable(GL_LIGHT1);
    
 // Parámetros de la Luz 2 (Spotligth=foco)
    GLfloat Ia2[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat Id2[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat Is2[] = { 0.0, 0.0, 0.9, 1.0 };
    glLightfv(GL_LIGHT2, GL_AMBIENT , Ia2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE , Id2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, Is2); // Brillo en azul para identificarla mejor
    glLightf (GL_LIGHT2, GL_SPOT_EXPONENT, 75.0);
    glLightf (GL_LIGHT2, GL_SPOT_CUTOFF,   15.0);
	GLfloat PL2[] = { 2.0, 0.0,-3.0, 1.0 };
	GLfloat DL2[] = {-2.0, 0.0,-2.0 };
	glLightfv(GL_LIGHT2, GL_POSITION, PL2);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, DL2);
    glEnable(GL_LIGHT2);*/
}

#define NT 3
GLuint textureName[NT];

void initTextures()
{
    glEnable(GL_TEXTURE_2D);
    glGenTextures(NT,textureName);
    
    const char *filename[NT] = { "SunTexture.bmp",
                                 "WaterTexture.bmp",
                                 "common/img/imgMarmol.bmp"};
    for(unsigned i = 0; i < NT; i++)
    {
        // Cargamos la textura
        glBindTexture(GL_TEXTURE_2D, textureName[i]);
        RgbImage texture(filename[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.GetNumCols(), texture.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE, texture.ImageData());
        //gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texture.GetNumCols(), texture.GetNumRows(), GL_RGB, GL_UNSIGNED_BYTE, texture.ImageData());
    
        // Configuramos la textura
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
    }
}

void funReshape(int w, int h) {

    // Configuramos el Viewport
    glViewport(0, 0, w, h);

    // Configuracion del modelo de proyeccion (P)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.1, 50.0);
}

void funKeyboardUp(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'e':
        moonIluminated = !moonIluminated;
        if (moonIluminated)
            glEnable(GL_LIGHT1);
        else
            glDisable(GL_LIGHT1);
        break;
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
    case '+':
        //speed += 0.01f;
        for (int i = 0; i < 3; i++)
            ambientLightIntensity[i] = std::min<GLfloat>(1.0, ambientLightIntensity[i] + 0.05);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLightIntensity);
        break;
    case '-':
        //speed -= 0.01f;
        for (int i = 0; i < 3; i++)
            ambientLightIntensity[i] = std::max<GLfloat>(0.0, ambientLightIntensity[i] - 0.05);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLightIntensity);
        break;
    }

    printf("KEYBOARD: key: %c, x: %d, y: %d \n", key, x, y);
}

void funSpecial(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        //cameraPos[1] = cameraPos[1] + 1.0f;
        //lookat[1] = lookat[1] + 1.0f;
        //translationGreen[0] += 0.5;
        flamePosition[2] += 0.5;
        break;
    case GLUT_KEY_DOWN:
        //cameraPos[1] = cameraPos[1] - 1.0f;
        //lookat[1] = lookat[1] - 1.0f;
        //translationGreen[0] -= 0.5;
        flamePosition[2] -= 0.5;
        break;
    case GLUT_KEY_RIGHT:
        if (!stopped)
        {
            //cameraPos[0] = cameraPos[0] + 1.0f;
            //lookat[0] = lookat[0] + 1.0f;
        }
        else
        {
            /*translationEarth[1] += 100 * speed;
            translationMoon[1] += 100 * speed;
            rotEarth[1] += 100 * speed;*/
        }
        flamePosition[0] += 0.5;
        rotationOrange[2] += 10.0;
        break;
    case GLUT_KEY_LEFT:
        if (!stopped)
        {
            //cameraPos[0] = cameraPos[0] - 1.0f;
            //lookat[0] = lookat[0] - 1.0f;
        }
        else
        {
            /*translationEarth[1] += -100 * speed;
            translationMoon[1] += -100 * speed;
            rotEarth[1] += -100 * speed;*/
        }
        flamePosition[0] -= 0.5;
        rotationOrange[2] -= 10.0;
        break;
    case GLUT_KEY_F1:
        stopped = !stopped;
        break;
    default:
        break;
    }

    printf("KEYBOARD SPECIAL: key: %d, x: %d, y: %d \n", key, x, y);
}

void funMouse(int key, int state, int x, int y)
{
    oldX = x;
    oldY = y;

    if (state == GLUT_UP)
    {
        if ((glutGet(GLUT_ELAPSED_TIME) - lastClickTime) < DOUBLE_CLICK_TIME)
            stopped = !stopped;
        
        lastClickTime = glutGet(GLUT_ELAPSED_TIME);
    }

    printf("MOUSE: key: %d, state: %d, x: %d, y: %d \n", key, state, x, y);
}

void funMotionPassive(int x, int y)
{
    /*cameraPos[0] -= float(oldX - x) / 10.0f;
    lookat[0] = float(oldX - x) / 10.0f;
    cameraPos[1] += float(oldY - y) / 10.0f;
    lookat[1] = float(oldY - y) / 10.0f;*/
    
    oldX = x;
    oldY = y;
    
    printf("PASSIVE: x: %d, y: %d \n", x, y);
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
}

#define ZOOM_RATIO 0.3
#define MAX_ZOOM 0
#define MIN_ZOOM 10

void funMouseWheel(int wheel, int direction, int x, int y)
{
    cameraPos[2] -= GLfloat(direction * ZOOM_RATIO);

    cameraPos[2] = std::min<GLfloat>(MIN_ZOOM, std::max<GLfloat>(MAX_ZOOM, cameraPos[2]));

    printf("MOUSEWHEEL: wheel: %d, direction: %d, x: %d, y: %d, positionZ: %f \n", wheel, direction, x, y, cameraPos[2]);
}

GLfloat initialX = -2.0;

long long year = 0, month = 0, day = 0;

enum
{
    DAYS_IN_YEAR    = 365,
    MONTHS_IN_YEAR  = 12,
    YEAR_IN_YEAR    = 1,
};

void funDisplay()
{
    drawFrame();
}


void funIdle()
{
    if (!stopped)
    {
        year    += YEAR_IN_YEAR;
        month   += MONTHS_IN_YEAR;
        day     += DAYS_IN_YEAR;
    
        translationEarth[1] += YEAR_IN_YEAR % year * speed;
        translationMoon[1] += MONTHS_IN_YEAR % month * speed;
        rotEarth[1] += DAYS_IN_YEAR % day * speed;

        rotSun[0] += 1.0f * speed;
    }

    drawFrame();
}

void drawFrame()
{
    // Borramos el buffer de color y el de profundidad
    glClearColor(SCREEN_COLOR);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Posicionamos la cámara (V)
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //drawInterface();

    // Posicionamos la cámara (V)
    gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2],
                 lookat[0],    lookat[1],    lookat[2],
                     up[0],        up[1],        up[2]);
    
    // Plano base
    /*glPushMatrix();
    {
        glTranslatef(yellowSphereInit[0], yellowSphereInit[1] - 5.0, yellowSphereInit[2]);
        glRotatef(90.0, 0.0, 1.0, 0.0);
        glRotatef(90.0, 1.0, 0.0, 0.0);
        drawPlane(20.0, 20.0);
    }
    glPopMatrix();*/

    // Dibujamos los objetos (M)

    // Numero
    /*glPushMatrix();
    {
        glTranslatef(yellowSphereInit[0], yellowSphereInit[1], yellowSphereInit[2]);
        glRotatef(0, 0.0, 1.0, 0.0);
        glRotatef(0, 1.0, 0.0, 0.0);
        glTranslatef(0.0, 0.0, 3.0);
        drawNumber();
    }
    glPopMatrix();*/
    
    switch (scene)
    {
        case SCENE_PLANETS:
        {
            // Sun
            glPushMatrix();
            {
                glTranslatef(yellowSphereInit[0], yellowSphereInit[1], yellowSphereInit[2]);
                glRotatef(rotSun[0], 1.0, 0.0, 0.0);
	            glLightfv(GL_LIGHT0, GL_POSITION, yellowSphereInit);
                drawSphere(2.0f, 'y');
            }
            glPopMatrix();
    
            // Earth
            glPushMatrix();
            {
                glTranslatef(yellowSphereInit[0], yellowSphereInit[1], yellowSphereInit[2]);
                glRotatef(translationEarth[0], 1.0, 0.0, 0.0);
                glRotatef(translationEarth[1], 0.0, 1.0, 0.0);
                glRotatef(translationEarth[2], 0.0, 0.0, 1.0);
                glTranslatef(blueSphereInit[0], 0.0, 0.0);
                glRotatef(rotEarth[0], 1.0, 0.0, 0.0);
                glRotatef(rotEarth[1], 0.0, 1.0, 0.0);
                drawSphere(0.5f, 'b');
            }
            glPopMatrix();
    
            // Moon
            glPushMatrix();
            {
                glTranslatef(yellowSphereInit[0], yellowSphereInit[1], yellowSphereInit[2]);
                glRotatef(translationEarth[0], 1.0, 0.0, 0.0);
                glRotatef(translationEarth[1], 0.0, 1.0, 0.0);
                glRotatef(translationEarth[2], 0.0, 0.0, 1.0);
                glTranslatef(blueSphereInit[0], 0.0, 0.0);
                glRotatef(translationMoon[0], 1.0, 0.0, 0.0);
                glRotatef(translationMoon[1], 0.0, 1.0, 0.0);
                glRotatef(translationMoon[2], 0.0, 0.0, 1.0);
                glTranslatef(whiteSphereInit[0], 0.0, 0.0);
	            glLightfv(GL_LIGHT1, GL_POSITION, whiteSphereInit);
                drawSphere(0.1f, 'w');
            }
            glPopMatrix();
            break;
        }
        case SCENE_PIEZA:
        {
            glPushMatrix();
            {
                glTranslatef(translationBlue[0], translationBlue[1], translationBlue[2]);
                drawPieza(3.0, 1.0, 'b');
            }
            glPopMatrix();

            glPushMatrix();
            {
                glTranslatef(translationBlue[0], translationBlue[1], translationBlue[2]);
                glTranslatef(translationOrange[0], translationOrange[1], translationOrange[2]);
                glRotatef(rotationOrange[2], 0.0, 0.0, 1.0);
                drawPieza(3.0, 1.0, 'o');
            }
            glPopMatrix();
        
            glPushMatrix();
            {
                glTranslatef(translationBlue[0], translationBlue[1], translationBlue[2]);
                glTranslatef(translationOrange[0], translationOrange[1], translationOrange[2]);
                glRotatef(rotationOrange[2], 0.0, 0.0, 1.0);
                glTranslatef(translationGreen[0], translationGreen[1], translationGreen[2]);
                drawPieza(3.0, 1.0, 'g');
            }
            glPopMatrix();
            break;
        }
        case SCENE_ROOM:
        {
            drawRoom();
            break;
        }
        default:
            break;
    }

    /*glPushMatrix();
    {
        glTranslatef(initialDist, 0.0, -8.0);
        glRotatef(rotY, 0.0, 1.0, 0.0);
        drawPieza(3.0f, 1.0f);
    }
    glPopMatrix();*/
    
    /*glPushMatrix();
    {
        glRotatef(translationEarth[0], 1.0, 0.0, 0.0);
        glRotatef(translationEarth[1], 0.0, 1.0, 0.0);
        drawCube();
    }
    glPopMatrix();*/
    
    /*glPushMatrix();
    {
        glTranslatef(initialDist, 0.0, -8.0);
        glRotatef(rotY, 0.0, 1.0, 0.0);
        drawSphere(1.0f, 'y');
    }
    glPopMatrix();*/

    // Intercambiamos los buffers
    glutSwapBuffers();
}

void drawRoom() {
    
 // Definimos el material de a habitación
    GLfloat Ka[] = { 0.2, 0.2, 0.2, 1.0 };
    GLfloat Kd[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat Ks[] = { 0.8, 0.8, 0.8, 1.0 };
    glMaterialfv(GL_FRONT, GL_AMBIENT  , Ka);
    glMaterialfv(GL_FRONT, GL_DIFFUSE  , Kd);
    glMaterialfv(GL_FRONT, GL_SPECULAR , Ks);
    glMaterialf (GL_FRONT, GL_SHININESS, 50.0);
    
 // Suelo
    glBindTexture(GL_TEXTURE_2D, textureName[0]);
    glBegin(GL_QUADS);
        glNormal3f(0.0, 1.0, 0.0);
        glTexCoord2f(0,0); glVertex3f( -5.0,  0.0,   0.0);
        glTexCoord2f(1,0); glVertex3f(  5.0,  0.0,   0.0);
	    glTexCoord2f(1,1); glVertex3f(  5.0,  0.0, -20.0);
        glTexCoord2f(0,1); glVertex3f( -5.0,  0.0, -20.0);
    glEnd();
    
 // Techo
    glBegin(GL_QUADS);
        glNormal3f(0.0, -1.0, 0.0);
        glVertex3f( -5.0,  5.0, -20.0);
        glVertex3f(  5.0,  5.0, -20.0);
	    glVertex3f(  5.0,  5.0,   0.0);
        glVertex3f( -5.0,  5.0,   0.0);
    glEnd();

 // Pared izquierda
     glBegin(GL_QUADS);
        glNormal3f(1.0, 0.0, 0.0);
        glVertex3f( -5.0,  0.0,   0.0);
        glVertex3f( -5.0,  0.0, -20.0);
	    glVertex3f( -5.0,  5.0, -20.0);
        glVertex3f( -5.0,  5.0,   0.0);
    glEnd();
    
 // Pared derecha
    glBegin(GL_QUADS);
        glNormal3f(-1.0, 0.0, 0.0);
        glVertex3f(  5.0,  0.0, -20.0);
        glVertex3f(  5.0,  0.0,   0.0);
	    glVertex3f(  5.0,  5.0,   0.0);
        glVertex3f(  5.0,  5.0, -20.0);
    glEnd();
    
 // Pared fondo
    glBegin(GL_QUADS);
        glNormal3f(0.0, 0.0, 1.0);
        glVertex3f( -5.0,  0.0, -20.0);
        glVertex3f(  5.0,  0.0, -20.0);
	    glVertex3f(  5.0,  5.0, -20.0);
        glVertex3f( -5.0,  5.0, -20.0);
    glEnd();

    glPushMatrix();
    {
        //selectColor('w');
        glMaterialf (GL_FRONT, GL_SHININESS, 100);
        glTranslatef(0.0, 0.6, -5.0);
        glutSolidTeapot(0.7);
    }
    glPopMatrix();

    glPushMatrix();
    {
        //selectColor('y');
        glTranslatef(flamePosition[0], 0.0, flamePosition[2]);
        glRotatef(-90, 1.0, 0.0, 0.0);
        glutSolidCone(0.25, flameHeight, 20, 20);
	    glLightfv(GL_LIGHT0, GL_POSITION, flamePosition);
    }
    glPopMatrix();

}

void drawInterface()
{
    glDisable(GL_LIGHTING);
    glPushMatrix();
    {
        glTranslatef(0.0, 1.75, -3.1);
        drawPieza(4.0, 0.1, 'p');
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(0.0, -1.75, -3.1);
        drawPieza(4.0, 0.1, 'p');
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(-3.5, 0, -3.1);
        drawPieza(0.1, 2.0, 'p');
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(3.5, 0, -3.1);
        drawPieza(0.1, 2.0, 'p');
    }
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void drawTriangulo(GLfloat s, char color) {

    selectColor(color);

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
    selectColor('w');
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

    selectColor(color);

    glLineWidth(1.0f);
    
    if (WITH_TEXTURES)
        glEnable(GL_TEXTURE_2D);

    glEnable(GL_POLYGON_OFFSET_FILL);
    glutSolidSphere(radius, 200, 200);

    glDisable(GL_POLYGON_OFFSET_FILL);
    //glutWireSphere(radius, 20, 20);

    /*if (wired)
        glutWireSphere(radius, 20, 20);
    else
        glutSolidSphere(radius, 20, 20);*/
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
}

void drawPieza(GLfloat width, GLfloat height, char color)
{
    selectColor(color);

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
    glEnable(GL_POLYGON_OFFSET_FILL);
    glLineWidth(1.0f);
    glBegin(GL_TRIANGLES);
    {
        selectColor('r');

        glVertex3f(1.0, 1.0, -1.0f);
        glVertex3f(-1.0, -1.0, -1.0f);
        glVertex3f(1.0, -1.0, -1.0f);

        glVertex3f(-1.0, -1.0, -1.0f);
        glVertex3f(1.0, 1.0, -1.0f);
        glVertex3f(-1.0, 1.0, -1.0f);
        
        selectColor('b');

        glVertex3f(1.0, 1.0, 1.0f);
        glVertex3f(-1.0, -1.0, 1.0f);
        glVertex3f(1.0, -1.0, 1.0f);
        
        glVertex3f(-1.0, -1.0, 1.0f);
        glVertex3f(1.0, 1.0, 1.0f);
        glVertex3f(-1.0, 1.0, 1.0f);
        
        selectColor('y');

        glVertex3f(1.0, 1.0, -1.0f);
        glVertex3f(1.0, -1.0, -1.0f);
        glVertex3f(1.0, -1.0, 1.0f);
        
        glVertex3f(1.0, -1.0, 1.0f);
        glVertex3f(1.0, 1.0, 1.0f);
        glVertex3f(1.0, 1.0, -1.0f);
        
        selectColor('g');

        glVertex3f(-1.0, 1.0, -1.0f);
        glVertex3f(-1.0, -1.0, -1.0f);
        glVertex3f(-1.0, -1.0, 1.0f);
        
        glVertex3f(-1.0, -1.0, 1.0f);
        glVertex3f(-1.0, 1.0, 1.0f);
        glVertex3f(-1.0, 1.0, -1.0f);
        
        selectColor('p');

        glVertex3f(1.0, 1.0, -1.0f);
        glVertex3f(-1.0, 1.0, 1.0f);
        glVertex3f(-1.0, 1.0, -1.0f);
        
        glVertex3f(1.0, 1.0, 1.0f);
        glVertex3f(-1.0, 1.0, 1.0f);
        glVertex3f(1.0, 1.0, -1.0f);
        
        selectColor('w');

        glVertex3f(1.0, -1.0, -1.0f);
        glVertex3f(-1.0, -1.0, 1.0f);
        glVertex3f(-1.0, -1.0, -1.0f);
        
        glVertex3f(1.0, -1.0, 1.0f);
        glVertex3f(-1.0, -1.0, 1.0f);
        glVertex3f(1.0, -1.0, -1.0f);
    }
    glEnd();
    glDisable(GL_POLYGON_OFFSET_FILL);
}

void selectColor(char color)
{
    if (glIsEnabled(GL_LIGHTING))
    {
        GLfloat empty[] = {0.0, 0.0, 0.0, 0.0};
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, empty);
        glMaterialfv(GL_FRONT, GL_SPECULAR, empty);
        glMaterialf(GL_FRONT, GL_SHININESS, 0);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, empty);
        switch (color)
        {
            case 'r':
            {
                GLfloat Kad[] = {1.0, 0.0, 0.0, 1.0};
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Kad);
                glColor3f(1.0, 0.0, 0.0);
                break;
            }
            case 'g':
            {
                GLfloat Kad[] = {0.0, 1.0, 0.0, 1.0};
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Kad);
                glColor3f(0.0, 1.0, 0.0);
                break;
            }
            case 'o':
            {
                break;
            }
            case 'b':
            {
                glBindTexture(GL_TEXTURE_2D, textureName[1]);
                GLfloat Kad[] = {0.25 , 0.5, 1.0, 1.0};
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Kad);
                GLfloat Ks[] = {0.0, 0.0, 1.0, 1.0};
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Ks);
                GLfloat Ns = 15.0;
                glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, Ns);
                glColor3f(0.0, 0.0, 1.0);
                break;
            }
            case 'y':
            {
                glBindTexture(GL_TEXTURE_2D, textureName[0]);
                GLfloat Kad[] = {1.0, 1.0, 0.0, 1.0};
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Kad);
                GLfloat Ks[] = {0.2, 0.2, 0.2, 1.0};
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Ks);
                GLfloat Ns = 15.0;
                glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, Ns);
                glColor3f(1.0, 1.0, 0.0);
                break;
            }
            case 'p':
            {
                GLfloat Kad[] = {0.5, 0.0, 0.5, 1.0};
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Kad);
                glColor3f(0.5, 0.0, 0.5);
                break;
            }
            case 'w':
            default:
            {
                glBindTexture(GL_TEXTURE_2D, textureName[2]);
                GLfloat Kad[] = {1.0, 1.0, 1.0, 1.0};
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Kad);
                GLfloat Ks[] = {0.2, 0.2, 0.2, 1.0};
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Ks);
                GLfloat Ns = 15.0;
                glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, Ns);
                glColor3f(1.0, 1.0, 1.0);
                break;
            }
        }
        /*GLfloat Ks[] = {1.0, 1.0, 1.0, 1.0};
        glMaterialfv(GL_FRONT, GL_SPECULAR, Ks);

        GLfloat Ns = 15.0;
        glMaterialf (GL_FRONT, GL_SHININESS, Ns);*/
    }
    /*else if (glIsEnabled(GL_TEXTURE_2D))
    {
        switch (color)
        {
        case 'b':
            glBindTexture(GL_TEXTURE_2D, textureName[0]);
            break;
        case 'w':
            glBindTexture(GL_TEXTURE_2D, textureName[0]);
            break;
        case 'y':
            glBindTexture(GL_TEXTURE_2D, textureName[0]);
            break;
        default:
            break;
        }
        //glDisable(GL_TEXTURE_2D);
    }*/
    else
    {
        switch (color)
        {
            case 'r':
            {
                glColor3f(1.0, 0.0, 0.0);
                break;
            }
            case 'g':
            {
                glColor3f(0.0, 1.0, 0.0);
                break;
            }
            case 'o':
            {
                glColor3f(1.0, 0.5, 0.0);
                break;
            }
            case 'b':
            {
                glColor3f(0.0, 0.0, 1.0);
                break;
            }
            case 'y':
            {
                glColor3f(1.0, 1.0, 0.0);
                break;
            }
            case 'p':
            {
                glColor3f(0.5, 0.0, 0.5);
                break;
            }
            case 'w':
            default:
            {
                glColor3f(1.0, 1.0, 1.0);
                break;
            }
        }
    }
}
