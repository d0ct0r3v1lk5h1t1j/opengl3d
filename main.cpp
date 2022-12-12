/*! \breif 3D view manipulation demo
 *
 * Demonstrates how to use freeglut callbacks to manipulate a 3D view, similarly
 * to how a modelling program or a model viewer would operate.
 *
 * Rotate: drag with the left mouse button.
 * Scale: drag up/down with the right mouse button.
 * Pan: drag with the middle mouse button.
 *
 * Press space to animate the scene and update the display continuously, press
 *   again to return to updating only when the view needs to change.
 * Press escape or q to exit.
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/freeglut.h>

#include <GL/glut.h>
#include <iostream>


#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif

#ifdef _MSC_VER
#pragma warning (disable: 4305 4244)                   // Compiler warning disabled 
#endif


static const char *helpprompt[] = {"Press F1 for help", 0};

static const char *helptext[] = {                     // Array for help prompt outputs
    "Rotate: left mouse drag",
    " Scale: right mouse drag up/down",
    "   Pan: middle mouse drag",
    "",
    "Toggle fullscreen: f",
    "Toggle animation: space",
    "Quit: escape",
    0
};

// defining functions to be used
void idle(void);
void display(void);
void print_help(void);
void reshape(int x, int y);
void keypress(unsigned char key, int x, int y);
void skeypress(int key, int x, int y);
void mouse(int bn, int st, int x, int y);
void motion(int x, int y);

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Cuboid drawing function >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/*! \brief Draws a cuboid using the centre, length, breadth and height.
 *
 * Shape is drawn using a routine on the normal of 6 faces of the Cuboid.
 * @param centrePosX :  The x coordinate of the centroid to be plotted.
 * @param centerPosY :  The y coordinate of the centroid to be plotted.
 * @param centerPosZ :  The z coordinate of the centroid to be plotted.
 * @param Clength :  The length of the cuboid.
 * @param Cbreadth : The breadth of the cuboid.
 * @param Cheight : The height of the cuboid.
 * @param colour[] : colour codes to be used.
 */

void DrawCube(GLfloat centerPosX, GLfloat centerPosY, GLfloat centerPosZ, GLfloat Clength, GLfloat Cbreadth, GLfloat Cheight, GLfloat colour[])
{
    GLfloat halfLength = Clength * 0.5f;
    GLfloat halfbreadth = Cbreadth * 0.5f;
    GLfloat halfheight = Cheight * 0.5f;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_CULL_FACE);
    GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    GLfloat mat_diffuse[] = { 0.1f, 0.5f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, colour);
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    GLfloat mat_shininess[] = { 50.0f };
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    GLfloat lightIntensity[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat light_position[] = { 2.0f, 6.0f, 3.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
    glBegin(GL_QUADS);

    // Top face (y = 1.0f)
    // Define vertices in counter-clockwise (CCW) order with normal pointing out

    glNormal3f(0.0f, 1.0f, 0.0f);     // Normal points up
    glVertex3f(centerPosX - halfLength, centerPosY + halfheight, centerPosZ - halfbreadth);  // v1
    glVertex3f(centerPosX + halfLength, centerPosY + halfheight, centerPosZ - halfbreadth);  // v2
    glVertex3f(centerPosX + halfLength, centerPosY + halfheight, centerPosZ + halfbreadth);  // v3
    glVertex3f(centerPosX - halfLength, centerPosY + halfheight, centerPosZ + halfbreadth);  // v4

    // Bottom face (y = -1.0f)

    glNormal3f(0.0f, -1.0f, 0.0f);    // Normal points down
    glVertex3f(centerPosX - halfLength, centerPosY - halfheight, centerPosZ - halfbreadth);  // v5
    glVertex3f(centerPosX - halfLength, centerPosY - halfheight, centerPosZ + halfbreadth);  // v6
    glVertex3f(centerPosX + halfLength, centerPosY - halfheight, centerPosZ + halfbreadth);  // v7
    glVertex3f(centerPosX + halfLength, centerPosY - halfheight, centerPosZ - halfbreadth);  // v8

    // Front face  (z = 1.0f)

    glNormal3f(0.0f, 0.0f, 1.0f);     // Normal points towards viewer
    glVertex3f(centerPosX - halfLength, centerPosY - halfheight, centerPosZ + halfbreadth);  // v9
    glVertex3f(centerPosX + halfLength, centerPosY - halfheight, centerPosZ + halfbreadth);  // v10
    glVertex3f(centerPosX + halfLength, centerPosY + halfheight, centerPosZ + halfbreadth);  // v11
    glVertex3f(centerPosX - halfLength, centerPosY + halfheight, centerPosZ + halfbreadth);  // v12

    // Back face (z = -1.0f)

    glNormal3f(0.0f, 0.0f, -1.0f);    // Normal points away from viewer
    glVertex3f(centerPosX - halfLength, centerPosY - halfheight, centerPosZ - halfbreadth);  // v13
    glVertex3f(centerPosX - halfLength, centerPosY + halfheight, centerPosZ - halfbreadth);  // v14
    glVertex3f(centerPosX + halfLength, centerPosY + halfheight, centerPosZ - halfbreadth);  // v15
    glVertex3f(centerPosX + halfLength, centerPosY - halfheight, centerPosZ - halfbreadth);  // v16

    // Left face (x = -1.0f)

    glNormal3f(-1.0f, 0.0f, 0.0f);    // Normal points left
    glVertex3f(centerPosX - halfLength, centerPosY - halfheight, centerPosZ + halfbreadth);  // v17
    glVertex3f(centerPosX - halfLength, centerPosY + halfheight, centerPosZ + halfbreadth);  // v18
    glVertex3f(centerPosX - halfLength, centerPosY + halfheight, centerPosZ - halfbreadth);  // v19
    glVertex3f(centerPosX - halfLength, centerPosY - halfheight, centerPosZ - halfbreadth);  // v20

    // Right face (x = 1.0f)

    glNormal3f(1.0f, 0.0f, 0.0f);     // Normal points right
    glVertex3f(centerPosX + halfLength, centerPosY - halfheight, centerPosZ + halfbreadth);  // v21
    glVertex3f(centerPosX + halfLength, centerPosY - halfheight, centerPosZ - halfbreadth);  // v22
    glVertex3f(centerPosX + halfLength, centerPosY + halfheight, centerPosZ - halfbreadth);  // v23
    glVertex3f(centerPosX + halfLength, centerPosY + halfheight, centerPosZ + halfbreadth);  // v24
    glEnd();
}



int win_width, win_height;                                  //width and height of the output window
float cam_theta, cam_phi = 25, cam_dist = 8;
float cam_pan[3];
int mouse_x, mouse_y;
int bnstate[8];
int anim, help;
long anim_start;
long nframes;

#ifndef GL_FRAMEBUFFER_SRGB
#define GL_FRAMEBUFFER_SRGB 0x8db9
#endif

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809d
#endif

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//! \brief The main function from where all the other functions are called.
/*!
 * @param argc The count of arguments passed
 * @param argv A character stream
*
*
*/

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("freeglut 3D view demo");

    glutDisplayFunc(display);   // Tell GLUT to use the method "display" for display.
    glutReshapeFunc(reshape);  // Tell GLUT to use the method "reshape" for reshaping.
    glutKeyboardFunc(keypress); //Tell GLUT to use the method "keyPressed" for key presses.
    glutSpecialFunc(skeypress); // Tell GLUT to use the method "skeypress" for special key presses.
    glutMouseFunc(mouse); // Tell GLUT to use the method "mouse" mouse button pressed.
    glutMotionFunc(motion); // Tell GLUT to use the method "motion" for mouse motion when one button is pressed.

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    //    glEnable(GL_MULTISAMPLE);
    //    glEnable(GL_FRAMEBUFFER_SRGB);
    //    glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;
    //    glEnable(GL_COLOR_MATERIAL);

    glutMainLoop();
    return 0;
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< idle >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

/*! \brief calls the function glutPostRedDisplay.
 *
 *  Marks the current window as needing to be redisplayed
 *
 */

void idle(void)
{
    glutPostRedisplay();
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Display Function >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

/*! \brief The display function calls all the other drawing subroutines.
 *
 * All the things that would be displayed in the OpenGL window, would be initiated from this function.
 */

void display(void)
{
    long tm;
    float lpos[] = { -1, 2, 3, 0};

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -cam_dist);
    glRotatef(cam_phi, 1, 0, 0);
    glRotatef(cam_theta, 0, 1, 0);
    glTranslatef(cam_pan[0], cam_pan[1], cam_pan[2]);

    glLightfv(GL_LIGHT0, GL_POSITION, lpos);

    glPushMatrix();
    if (anim) {
        tm = glutGet(GLUT_ELAPSED_TIME) - anim_start;
        glRotatef(tm / 10.0f, 1, 0, 0);
        glRotatef(tm / 10.0f, 0, 1, 0);
    }

    glPopMatrix();



    //Left rectangle

    glPushMatrix();

//    float temp[4] = {0.0f, 0.0f, 0.0f, 0.0f};
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,temp );

    glColor3f(0.5, 0.5, 0.5);

//    glTranslatef(-8, 3, -5);
//    glScalef(3, 7, 4);
//    glutSolidCube(1);

    GLfloat color[] = {1.0, 0.93, 0.75, 1.0};
    DrawCube(-8, 3, -5, 3, 4, 7, color);
    glPopMatrix();

    glPushMatrix();

//    float col[4] = {1.0f, 0.0f, 0.0f, 0.0f};
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,col );

//    glTranslatef(8, 3, -5);
//    glScalef(3, 7, 4);
//    glutSolidCube(1);

    GLfloat color1[] = {1, 0.9, 0.5, 1.0};
    DrawCube(8, 3, -5, 3, 4, 7, color);
    glPopMatrix();


    glPushMatrix();

//    glTranslatef(6, 2.5, -5);
//    glScalef(3, 5, 4);
//    glutSolidCube(1);

    GLfloat color2[] = {0.5, 0.5, 0.5, 1.0};
    DrawCube(6, 2.5, -5, 3, 4, 5, color);
    glPopMatrix();

    glPushMatrix();

//    glTranslatef(-6, 2.5, -5);
//    glScalef(3, 5, 4);
//    glutSolidCube(1);

    GLfloat color3[] = {0.5, 0.5, 0.5, 1.0};
    DrawCube(-6, 2.5, -5, 3, 4, 5, color);
    glPopMatrix();

    //Center
    glPushMatrix();

//    glTranslatef(0, 3, -5);
//    glScalef(9, 7.5, 4);
//    glutSolidCube(1);

    GLfloat color4[] = {0.5, 0.5, 0.5, 1.0};
    DrawCube(0, 3, -5, 9, 4, 7.5, color);
    glPopMatrix();

    //Dias
    glPushMatrix();


//    glTranslatef(0, 0.5, -2);
//    glScalef(9, 1, 5);
//    glutSolidCube(1);

    GLfloat color5[] = {0.5, 0.5, 0.5, 1.0};
    DrawCube(0, 0.5, -2, 9, 5, 1, color5);

    glPopMatrix();

    //SOLID SPHERE
    glPushMatrix();
    glTranslated(0, 5, -5);
    glScalef(1, 1, 0.5);
    glutSolidSphere(3, 20, 20);
    glPopMatrix();

    //Grass paths
    glPushMatrix();
    glPopMatrix();
    glBegin(GL_QUADS);

    glVertex3f(-10, 0, -10);
    glVertex3f(-10, 0, 10);
    glVertex3f(10, 0, 10);
    glVertex3f(10, 0, -10);
    glEnd();



    print_help();

    //Display model

    glutSwapBuffers();                 // Performs a buffer swap on the layer in use for the current window.
    nframes++;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Print Help Function >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

/*! \brief The print_help function displays the prompts for help
 *
 * The prompt help shows the list of actions user can do in the given 3d model.
 */

void print_help(void)
{
    int i;
    const char *s, **text;

    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, win_width, 0, win_height, -1, 1);

    text = help ? helptext : helpprompt;

    for (i = 0; text[i]; i++) {
        glColor3f(0, 0.1, 0);
        glRasterPos2f(7, win_height - (i + 1) * 20 - 2);
        s = text[i];
        while (*s) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *s++);
        }
        glColor3f(0, 0.9, 0);
        glRasterPos2f(5, win_height - (i + 1) * 20);
        s = text[i];
        while (*s) {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *s++);
        }
    }

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glPopAttrib();
}

#define ZNEAR   0.5f

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Reshape Function >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

/*! \brief Reshapes the models in the current OpenGL window.
 *
 * Reshape function takes 2 arguments,they are width and height of reshaped window.
 * Mainly these parameters are used to set a new viewport.
 *
 * @param x : width of the reshpaed window.
 * @param y : height of the reshaped window.
 */

void reshape(int x, int y)
{
    float vsz, aspect = (float)x / (float)y;
    win_width = x;
    win_height = y;

    glViewport(0, 0, x, y);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    vsz = 0.4663f * ZNEAR;
    glFrustum(-aspect * vsz, aspect * vsz, -vsz, vsz, 0.5, 500.0);
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Reshape Function >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

/*! \brief Reshapes the models in the current OpenGL window.
 *
 * Reshape function takes 2 arguments,they are width and height of reshaped window.
 * Mainly these parameters are used to set a new viewport.
 *
 * @param x : width of the reshpaed window.
 * @param y : height of the reshaped window.
 */

void keypress(unsigned char key, int x, int y)
{
    static int fullscr;
    static int prev_xsz, prev_ysz;

    switch (key) {
    case 27:
    case 'q':
        exit(0);
        break;

    case ' ':
        anim ^= 1;
        glutIdleFunc(anim ? idle : 0);
        glutPostRedisplay();

        if (anim) {
            anim_start = glutGet(GLUT_ELAPSED_TIME);
            nframes = 0;
        } else {
            long tm = glutGet(GLUT_ELAPSED_TIME) - anim_start;
            long fps = (nframes * 100000) / tm;
            printf("framerate: %ld.%ld fps\n", fps / 100, fps % 100);
        }
        break;

    case '\n':
    case '\r':
        if (!(glutGetModifiers() & GLUT_ACTIVE_ALT)) {
            break;
        }
    case 'f':
        fullscr ^= 1;
        if (fullscr) {
            prev_xsz = glutGet(GLUT_WINDOW_WIDTH);
            prev_ysz = glutGet(GLUT_WINDOW_HEIGHT);
            glutFullScreen();
        } else {
            glutReshapeWindow(prev_xsz, prev_ysz);
        }
        break;
    }
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Special Key Press Function >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

/*! \brief Sets the special keyboard callback for the current window.
 *
 * The special keyboard callback is triggered when keyboard function or directional keys are pressed.
 *
 * @param key : key callback parameter is a GLUT_KEY_* constant for the special key pressed.
 * @param x :  Indicate the mouse in window x coordinate when the key was pressed.
 * @param y :  indicate the mouse in window y coordinate when the key was pressed.
 */

void skeypress(int key, int x, int y)
{
    switch (key) {
    case GLUT_KEY_F1:
        help ^= 1;
        glutPostRedisplay();

    default:
        break;
    }
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Mouse Function >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

/*! \brief Sets the mouse callback for the current window.
 *
 * When a user presses and releases mouse buttons in the window, each press and each release generates a mouse callback.
 *
 * @param bn : This parameter is one of GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON.
 * @param st : This parameter is either GLUT_UP or GLUT_DOWN indicating whether the callback was due to a release or press respectively
 * @param x :  indicate the mouse in window x coordinate when the mouse button state changed.
 * @param y :  indicate the mouse in window y coordinate when the mouse button state changed.
 */

void mouse(int bn, int st, int x, int y)
{
    int bidx = bn - GLUT_LEFT_BUTTON;
    bnstate[bidx] = st == GLUT_DOWN;
    mouse_x = x;
    mouse_y = y;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Mouse Motion Function >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

/*! \brief Sets the motion callback respectively for the current window.
 *
 * The motion callback for a window is called when the mouse moves within the window while one or more mouse buttons are pressed.
 *
 * @param x :  indicate the mouse in window x coordinate when the mouse button state changed.
 * @param y :  indicate the mouse in window y coordinate when the mouse button state changed.
 */

void motion(int x, int y)
{
    int dx = x - mouse_x;
    int dy = y - mouse_y;
    mouse_x = x;
    mouse_y = y;

    if (!(dx | dy)) return;

    if (bnstate[0]) {
        cam_theta += dx * 0.5;
        cam_phi += dy * 0.5;
        if (cam_phi < -90) cam_phi = -90;
        if (cam_phi > 90) cam_phi = 90;
        glutPostRedisplay();
    }
    if (bnstate[1]) {
        float up[3], right[3];
        float theta = cam_theta * M_PI / 180.0f;
        float phi = cam_phi * M_PI / 180.0f;

        up[0] = -sin(theta) * sin(phi);
        up[1] = -cos(phi);
        up[2] = cos(theta) * sin(phi);
        right[0] = cos(theta);
        right[1] = 0;
        right[2] = sin(theta);

        cam_pan[0] += (right[0] * dx + up[0] * dy) * 0.01;
        cam_pan[1] += up[1] * dy * 0.01;
        cam_pan[2] += (right[2] * dx + up[2] * dy) * 0.01;
        glutPostRedisplay();
    }
    if (bnstate[2]) {
        cam_dist += dy * 0.1;
        if (cam_dist < 0) cam_dist = 0;
        glutPostRedisplay();
    }
}