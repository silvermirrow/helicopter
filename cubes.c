/*-----------------------------------------------------------
 * An example program to draw a car
 *   Author: S.K. Ueng
 *   Date:  11/4/2001
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#include <GL/glut.h>

#define  PI   3.141592653

#define Step  0.5
typedef    int   menu_t;
menu_t     top_m;
/*-----Define a unit box--------*/
/* Vertices of the box */
float  points[][3] = { {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5},
                      {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},
                      {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5},
                      {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5} };
float p[][3] = { {0,0,0} }, c[][4] = { {0.5,0.5,0.5,0.5} };
/* face of box, each face composing of 4 vertices */
int    face[][4] = { {0, 3, 2, 1}, {0, 1, 5, 4}, {1, 2, 6, 5},
                    {4, 5, 6, 7}, {2, 3, 7, 6}, {0, 4, 7, 3} };
float  colors[6][3] = { {0.5,0.5,0.5}, {0.7,0.7,0.7}, {0.7,0.5,0.5},
                     {0.5,0.5,0.5}, {0.5,0.7,0.5}, {0.5,0.5,0.7} };
/* indices of the box faces */
int    cube[6] = { 0, 1, 2, 3, 4, 5 };
int tree_r[80][3] = { 0 };
/*-Declare GLU quadric objects, sphere, cylinder, and disk --*/
GLUquadricObj* sphere = NULL, * cylind = NULL, * disk;


/*-Declare car position, orientation--*/
float  self_ang = -90.0, glob_ang = 0.0, tilt_ws = 0.0, tilt_ad = 0.0;
float  position[3] = { 8.0, 0.0, 0.0 }, temppo[3] = { 0,0,0 };

//Global variables recording rotation angles of H-arm and blades.
float  arm_ang = 45.0, bld_ang = 5;

/*-----Define window size----*/
int width = 512, height = 512;
int posx = -1, posy = -1, first = 0, dx = 0, dy = 0, dz = 0, vy = 0;
int ranx = 0, rany = 0, ranz = 0;
// Create an array of boolean values of length 256 (0-255)  
bool* keyStates[256] = { 0 };
bool* specialkeyStates[256] = { 0 };
int view_mode = 0, cord = 0, person = 1, pog = 0, xp = 0;
float bd_ang = 5.0, ad_ang = 3.0;
float rot = 0.0, zoom = 1.0;
void timer() {
    xp = (xp) % 4;
    rot += (view_mode == 2) ? 0.5 : 0.0;
    rot -= (rot >= 360) ? 360 : 0;
    bld_ang += bd_ang;
    if (bld_ang > 360.0) {
        bld_ang -= 360.0;
    }
    if (self_ang > 360.0) {
        self_ang -= 360.0;
    }if (self_ang < 0) {
        self_ang += 360.0;
    }
    if (tilt_ws != 0)if (tilt_ws > 0)tilt_ws -= 0.5; else tilt_ws += 0.5;
    if (tilt_ad != 0)if (tilt_ad > 0)tilt_ad -= 0.5; else tilt_ad += 0.5;
    arm_ang += ad_ang;
    if (arm_ang > 360.0) {
        arm_ang -= 360.0;
    }
    if (!keyStates['q']) {
        if (position[1] <= 1 && bd_ang < 0)bd_ang = 0;
        if (bd_ang > 0.0)bd_ang -= 0.2;
    }
    if (position[1] <= 300 && position[1] >= 1)position[1] += ((bd_ang - 90) / 270) - 0.5;
    if (position[1] <= 1)position[1] = 1;
    if (position[1] >= 300)position[1] = 300;
    glutPostRedisplay();
    glutTimerFunc(1000 / 120, timer, 0);
}


/*----------------------------------------------------------
 * Procedure to initialize the working environment.
 */
void  myinit()
{
    ranx = rand() % 4 + 1; rany = rand() % 2; ranz = rand() % 5 + 1;
    glClearColor(0.0, 0.0, 0.0, 1.0);      /*set the background color BLACK */
                     /*Clear the Depth & Color Buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    /*---Create quadratic objects---*/
    if (sphere == NULL) {
        sphere = gluNewQuadric();
        gluQuadricDrawStyle(sphere, GLU_FILL);
        gluQuadricNormals(sphere, GLU_SMOOTH);
    }
    if (cylind == NULL) {
        cylind = gluNewQuadric();
        gluQuadricDrawStyle(cylind, GLU_FILL);
        gluQuadricNormals(cylind, GLU_SMOOTH);
    }
    if (disk == NULL) {
        disk = gluNewQuadric();
        gluQuadricDrawStyle(disk, GLU_FILL);
        gluQuadricNormals(disk, GLU_SMOOTH);
    }
    int m = 79;
    while (m >= 0) {
        int i = 2;
        while (i >= 0) {
            tree_r[m][i] = rand() % (((i + 99) % (m + 1)) + 20);
            i--;
        }
        m--;
    }
    gluLookAt(0.0, 0.0, 15.0, position[0], position[1], position[2], 0.0, 1.0, 0.0);
    glutTimerFunc(1000, timer, 0);
}


/*--------------------------------------------------------
 * Procedure to draw a 1x1x1 cube. The cube is within
 * (-0.5,-0.5,-0.5) ~ (0.5,0.5,0.5)
 */
void draw_cube()
{
    int    i;

    for (i = 0; i < 6; i++) {
        glColor3fv(colors[i]);  /* Set color */
        glBegin(GL_POLYGON);  /* Draw the face */
        glVertex3fv(points[face[i][0]]);
        glVertex3fv(points[face[i][1]]);
        glVertex3fv(points[face[i][2]]);
        glVertex3fv(points[face[i][3]]);
        glEnd();
    }
}

void draw_pog()
{
    int    i, j, k, n;
    glPushMatrix();
    glTranslatef(500, 100, 500);
    glScalef(1000, 500, 1000);//
    glRotatef(-90, 0, 1, 0);
    int d[][3] = { {0,1,2}, { 1,3,5 } };
    if ((view_mode != 2 && (self_ang) >= 90 && self_ang <= 270) || (view_mode == 2 && rot >= 90 && rot <= 270)) { j = 1; }
    else { j = 0; }
    for (i = 0; i < 3; i++) {

        glColor3fv(colors[i]);  /* Set color */
        glBegin(GL_POLYGON);  /* Draw the face */
        glVertex3fv(points[face[d[j][i]][0]]);

        glVertex3fv(points[face[d[j][i]][1]]);

        glVertex3fv(points[face[d[j][i]][2]]);

        glVertex3fv(points[face[d[j][i]][3]]);

        glEnd();
    } glPopMatrix();
}
;
/*---------------------------------------------------------
 * Procedure to draw the floor.
 */
int max = 300;

void draw_floor(float sx, float sz)
{

    //int sx1 = ((int)position[0])%max,sz1=((int)position[2]-15)%max;
    //glPushMatrix();
    //glTranslatef(-sx, 0, -sz);
    //printf("%d %d", sx, sz);
    //glTranslatef(position[0] / 2, 0, position[2] / 2);
    //glTranslatef(-9*sx,0,-9*sz);

    int  i, j, m, n, k = sx / 10, p = sz / 10;
    float a = 0.5, b = 0.8;
    for (i = (int)k * 10 - max, m = 0; m < 60; i += 10, m++)
        for (j = (int)p * 10 - max, n = 0; n < 60; j += 10, n++) {
            int t = (int)abs(i / 10 + j / 10) % 2;

            if (t) glColor3f(a, a + 0.3, b);
            else glColor3f(a, b, a);
            for (float a = 0.0; a < 0.5; a += 0.1) {
                glBegin(GL_POLYGON);
                glVertex3f(i - 1.0 * 10, t-a, j - 1.0 * 10);//
                glVertex3f(i - 1.0 * 10, t+a, j - 0.0 * 10);
                glVertex3f(i - 0.0 * 10, t-a, j - 0.0 * 10);
                glVertex3f(i - 0.0 * 10, t+a, j - 1.0 * 10);
                glEnd();
            }
        }//glTranslatef(-position[0] / 2, 0, -position[2] / 2);
      //glTranslatef(-position[0] / 1, 0, -position[2] / 1);

      //glPopMatrix();
}

/*-------------------------------------------------------
 * Procedure to draw a polygon as a blade of the windmill
 */
void draw_blade()
{
    glBegin(GL_POLYGON);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 4.0, 0.0);
    glVertex3f(1.0, 8.0, 0.0);
    glVertex3f(-1.0, 8.0, 0.0);
    glVertex3f(-1.0, 4.0, 0.0);
    glEnd();
}

/*-------------------------------------------------------
 * Procedure to draw three axes and the orign
 */
void draw_axes()
{

    /*----Draw a white sphere to represent the original-----*/
    glColor3f(0.9, 0.9, 0.9);

    gluSphere(sphere, 2.0,   /* radius=2.0 */
        12,            /* composing of 12 slices*/
        12);           /* composing of 8 stacks */

  /*----Draw three axes in colors, yellow, meginta, and cyan--*/
  /* Draw Z axis  */
    glColor3f(0.0, 0.95, 0.95);
    gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
        10.0,              /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */

/* Draw Y axis */
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);  /*Rotate about x by -90', z becomes y */
    glColor3f(0.95, 0.0, 0.95);
    gluCylinder(cylind, 0.5, 0.5, /* radius of top and bottom circle */
        10.0,             /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();

    /* Draw X axis */
    glColor3f(0.95, 0.95, 0.0);
    glPushMatrix();
    glRotatef(90.0, 0.0, 1.0, 0.0);  /*Rotate about y  by 90', x becomes z */
    gluCylinder(cylind, 0.5, 0.5,   /* radius of top and bottom circle */
        10.0,             /* height of the cylinder */
        12,               /* use 12-side polygon approximating circle*/
        3);               /* Divide it into 3 sections */
    glPopMatrix();
    /*-- Restore the original modelview matrix --*/
    glPopMatrix();
}
// If the 'a' key has been pressed  
// Perform 'a' key operations

int tkey = 0, por = 0;
void udward(int mode) {
    if (mode) {
        if (bd_ang < 270.0)bd_ang += 1.0;
        //if (position[1] < 300)position[1] += 1;
        //keyStates['q'] = false;
    }
    else {
        if (bd_ang > -270.0)bd_ang -= 1.0;
        //if (position[1] > 1)position[1] -= 1;
        //keyStates['e'] = false;
    }
}
void fbward(int mode) {
    float sp = 0.3 + (bd_ang / 135);
    if (mode) {
        position[0] += sp * Step * cos(self_ang * PI / 180.0);
        position[2] -= sp * Step * sin(self_ang * PI / 180.0);
        if (position[1] > 5) if (tilt_ws < 20.0)tilt_ws += 1.0; if (tilt_ws >= 20) tilt_ws = 20.0;
        //keyStates['w'] = false;
    }
    else {
        position[0] -= sp * Step * cos(self_ang * PI / 180.0);
        position[2] += sp * Step * sin(self_ang * PI / 180.0);
        if (position[1] > 5) if (tilt_ws > -20.0)tilt_ws -= 1.0; if (tilt_ws <= -20) tilt_ws = -20.0;
        //keyStates['s'] = false;
    }
}
void rlward(int mode) {
    if (mode) {
        if (ad_ang < 45.0)ad_ang += 2.0;
        self_ang +=  0.5;
        if (position[1] > 5) if (tilt_ad < 20.0)tilt_ad += 1.0; if (tilt_ad >= 20) tilt_ad = 20.0;
        //keyStates['a'] = false;
    }
    else {
        if (ad_ang > -45.0)ad_ang -= 2.0;
        self_ang -=  0.5;
        if (position[1] > 5) if (tilt_ad > -20.0)tilt_ad -= 1.0; if (tilt_ad <= -20) tilt_ad = -20.0;
        //keyStates['d'] = false;
    }
}
void qkey(void) {
    if (keyStates['p'] || keyStates['P']) exit(0);
    else if (keyStates['q']) {
        if (view_mode == 1) {
            dz += 1; dz %= 360;
        }
        else {
            udward(1);
        }
    }

}
void ekey(void) {
    if (keyStates['e']) {
        if (view_mode == 1) {
            dz -= 1; dz %= 360;
        }
        else {
            udward(0);
        }
    }

    //display();
}
void akey(void) {

    if (keyStates['a']) {
        if (view_mode == 1) {
            dx -= 1; dx %= 360;
        }
        else {
            rlward(1);
        }
    }
    else if (ad_ang > 1.0) ad_ang -= 1.0;
}
void dkey(void) {

    if (keyStates['d']) {
        if (view_mode == 1) {
            dx += 1; dx %= 360;
        }
        else {
            rlward(0);
        }
    }
    else if (ad_ang < -1.0) ad_ang += 1.0;
}
void wkey(void) {
    if (keyStates['w']) {
        if (view_mode == 1) {
            dy += 1; dy %= 360;
        }
        else {
            fbward(1);
        }
    }
}
void skey(void) {

    if (keyStates['s']) {
        if (view_mode == 1) {
            dy -= 1; dy %= 360;
        }
        else {
            fbward(0);
        }
    }
}



/*-------------------------------------------------------
 * Display callback func. This func. draws three
 * cubes at proper places to simulate a solar system.
 */
void* font = GLUT_BITMAP_TIMES_ROMAN_10;
int persp = 1,ft=0;
int port[5][4] = { {0,0,2,2},{0,0,1,1},{0,1,1,1},{1,0,1,1},{1,1,1,1} };
void display()
{
    wkey(); skey(); akey(); dkey(); qkey(); ekey();
    int s = 0, e = 1;
    if (por) { s = 1; e = 5; }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (int i = s; i < e; i++) {
        if (i == 1) { xp = 0; persp = 0; }
        if (i == 2) { xp = 1; persp = 1; }
        if (i == 3) { xp = 2; persp = 1; }
        if (i == 4) { xp = 3; persp = 1; }
        glViewport((int)port[i][0] * width / 2, (int)port[i][1] * height / 2, (int)port[i][2] * width / 2, (int)port[i][3] * height / 2);
        //glMatrixMode(GL_PROJECTION);        // Select projection matrix
        //glLoadIdentity();                        // Clear it
        //glPushMatrix();
        //glOrtho(0.0, 1.0, 0.0, 1.0, 1.0, 1.0);
        //glPopMatrix();
        static float  ang_self = 0.0;  /*Define the angle of self-rotate */
        static float  angle = 0.0;

        /*Clear previous frame and the depth buffer */

        /*----Define the current eye position and the eye-coordinate system---*/
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        if (persp)gluPerspective(10, 1, -10.0 * zoom, -30.0 * zoom);
        if (xp == 1 || xp == 3) {

            if (xp == 3) {
                if (view_mode == 1)gluLookAt(10, 10, 0, 20 , tilt_ws+3, 0, 0.0, 1.0, 0.0);
                else gluLookAt(10, 10, 0, position[1] + 40.0, position[1] / 8, 0, 0.0, 1.0, 0.0);
            }
            else gluLookAt(0, 15, 0, -15, 0, 0, 0.0, 1.0, 0.0);
        }


        if (xp == 0) {
            gluLookAt(0, 5, 25, 0, 0, 0, 0.0, 1.0, 0.0);//position[0], position[1], position[2]
            glTranslatef(0, -5, -25);
        }
        if (view_mode == 2) {
            glRotatef(rot, 0.0, 1.0, 0.0);
        }if (xp == 2) {
            gluLookAt(-10, 5, 0, 0, 0, 0, 0.0, 1.0, 0.0);
            glTranslatef(10, -5, 0);
        }
        if (!cord) {
            draw_axes();
        }
        glRotatef(dy, 1.0, 0.0, 0.0);
        glRotatef(dx, 0.0, 1.0, 0.0);
        glRotatef(dz, 0.0, 0.0, 1.0);



        glScalef(zoom, zoom, zoom);

        if (!person) {glRotatef(-90, 0, 1.0, 0);
            //if (ft) {
                
                //ft--;
                gluLookAt(temppo[0], temppo[1], temppo[2], position[0], 0, position[2], 0.0, 1.0, 0.0);//position[0], position[1], position[2]
                //glTranslatef(-temppo[0], -temppo[1], -temppo[2]);
            //}else
                //gluLookAt(temppo[0], temppo[1], temppo[2], 0, 0, 0, 0.0, 1.0, 0.0);
        }

        if (person) {
            glColor4f(0.4, 0.8, 0.8, 0.5);
            /*-------Draw the car body which is a sphere----*/
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glPushMatrix();              /* Save coord. sys */
            glTranslatef(0.0, 4.0, 0.0);
            glScalef(17.0, 6.0, 6.0);    /* Scale up the axes */
            //draw_cube();
            gluSphere(sphere, 0.8,   /* radius=1.5 */
                12,            /* composing of 12 slices*/
                12);           /* composing of 12 stacks */
            glColor4f(0.3, 0.5, 0.8, 0.5);
            glTranslatef(0.55, 0.15, 0.0);
            glRotatef(90, 0.0, 0.0, 1.0);
            glScalef(0.6, 0.4, 0.7);
            glDepthMask(GL_FALSE);
            gluSphere(sphere, 0.8,   /* radius=1.5 */
                12,            /* composing of 12 slices*/
                12);
            glDepthMask(GL_TRUE);
            glPopMatrix();               /* Get M1 back */
            //glDisable(GL_BLEND);
            /*-------Draw the front wheels -----*/
            glColor3f(1.0, 0.0, 0.0);
            glPushMatrix();              /* Save M1 coord. sys */
            glTranslatef(-6.0, 0.0, 4.0); /* Go to left wheel position x:-6*/
            glutSolidTorus(1.0,  /* inner radius */
                2.0,  /* outer radius */
                24,   /* divided into 18 segments */
                12);  /* 12 rings */
            glPopMatrix();

            glPushMatrix();              /* Save M1 coord. sys */
            glTranslatef(-6.0, 0.0, -4.0);/* Go to right wheel position */
            glutSolidTorus(1.0,  /* inner radius */
                2.0,  /* outer radius */
                24,   /* divided into 18 segments */
                12);  /* 12 rings */
            glPopMatrix();

            /*------Draw back wheels ----*/
            glColor3f(1.0, 0.4, 0.0);
            //glPushMatrix();              /* Save M1 coord. sys */
            //glTranslatef(6.0, 0.0, 4.0); /* Go to left wheel position */
            //glutSolidTorus(1.0,  /* inner radius */
            //               2.0,  /* outer radius */
            //               24,   /* divided into 18 segments */
            //              12);  /* 12 rings */
            //glPopMatrix();

            glPushMatrix();              /* Save M1 coord. sys */

            glTranslatef(9.0, 0.0, 0.0);/* Go to right wheel position */
            glRotatef(ad_ang, 0.0, 1.0, 0.0);
            glutSolidTorus(1.0,  /* inner radius */
                2.0,  /* outer radius */
                24,   /* divided into 18 segments */
                12);  /* 12 rings */
            glPopMatrix();
            glPushMatrix();
            glRotatef(-90, 1.0, 0.0, 0.0);
            glColor3f(0.68, 0.68, 0.68);
            gluCylinder(cylind, 1.0, 1.0, 12.0, 12, 3);
            glTranslatef(0.0, 0.0, 12.0);
            glColor3f(0.2, 0.2, 0.95);
            gluSphere(sphere, 0.5,   /* radius=1.5 */
                12,            /* composing of 12 slices*/
                12);           /* composing of 12 stacks */
            glPopMatrix();

            glPushMatrix();

            glColor3f(1.0, 0.5, 0.0);

            glTranslatef(-7.0, 4.9, 0.0);
            glScalef(4.0, 1.25, 0.8);
            glRotatef(-90, 0.0, 1.0, 0.0);
            gluCylinder(cylind, 2.0, 0.7, 4.0, 12, 3);
            //glTranslatef(-16.0, 3.0, 1.5);
            glPopMatrix();
            glPushMatrix();

            /*------Draw three blades ---*/
            glColor3f(0.3, 0.3, 0.3);
            glTranslatef(-21.0, 5.0, 0.0);
            gluCylinder(cylind, 1.0, 1.0, 2.0, 12, 3);
            glColor3f(1.0, 1.0, 1.0);
            glTranslatef(0.0, 0.0, 1.5);
            gluSphere(sphere, 0.8,   /* radius=1.5 */
                12,            /* composing of 12 slices*/
                12);
            glRotatef(-90.0, 0.0, 0.0, 1.0);
            glRotatef(arm_ang, 0.0, 0.0, 1.0);  /* Rotate about x axis, M5 coord. sys. */
            glScalef(0.5, 0.5, 0.5);
            draw_blade();/* draw the first blade */

            glRotatef(120.0, 0.0, 0.0, 1.0);    /* rotate by 120 degree, M6 coord. sys */
            draw_blade();/* draw 2nd blade */

            glRotatef(120.0, 0.0, 0.0, 1.0);    /* rotate by 120 degree,M7 coord. sys */
            draw_blade();/* draw 3rd blade */

            glPopMatrix();
            //glPopMatrix();
            glPushMatrix();
            glTranslatef(0.0, 12.0, 0.0);

            glScalef(2.0, 2.0, 2.0);
            glRotatef(90.0, 1.0, 0.0, 0.0);
            glRotatef(bld_ang, 0.0, 0.0, 1.0);  /* Rotate about x axis, M5 coord. sys. */
            glRotatef(5.0, 1.0, 1.0, 0.0);
            draw_blade();/* draw the first blade */
            glRotatef(-5.0, 1.0, 1.0, 0.0);
            glRotatef(120.0, 0.0, 0.0, 1.0);    /* rotate by 120 degree, M6 coord. sys */
            glRotatef(5.0, 1.0, 1.0, 0.0);
            draw_blade();/* draw 2nd blade */
            glRotatef(-5.0, 1.0, 1.0, 0.0);
            glRotatef(120.0, 0.0, 0.0, 1.0);    /* rotate by 120 degree,M7 coord. sys */
            glRotatef(5.0, 1.0, 1.0, 0.0);
            draw_blade();/* draw 3rd blade */

            glPopMatrix();

            glRotatef(tilt_ad, 1.0, 0.0, 0.0);
            glRotatef(tilt_ws, 0.0, 0.0, 1.0);

            glRotatef(-self_ang, 0.0, 1.0, 0.0);
            glTranslatef(-position[0], -position[1], -position[2]);

        }
        if (person) {

            glPushMatrix();
            //glTranslatef(position[0], 0, position[2]);
            //draw_axes();
            //glTranslatef(-position[0]/2, 0, -position[2]/2);

            draw_floor(position[0], position[2]);
            //glTranslatef(-9 * position[0], 0, -9 * position[2]);
            glPopMatrix();
        }
        else { draw_axes(); draw_floor(temppo[0], temppo[2]); }
        
        if (pog)draw_pog();
        float n = 19.0;
        int j = 79;
        while (j >= 0) {
            glPushMatrix();
            glColor3f((float)j / 40, (float)j / 40, (float)j / 40);
            glTranslatef((tree_r[j][0] * 40.0), 30 * (tree_r[j][1]), (tree_r[j][2] * 40.0));
            glScalef(0.1,0.1,0.1);
            gluSphere(sphere, tree_r[j][1],   /* radius */
                12,            /* composing of 12 slices*/
                12);           /* composing of 12 stacks */
            glPopMatrix();
            glPushMatrix();
            glColor3f(((float)tree_r[j][2]) / 120, ((float)tree_r[j][1] + n) / 80, ((float)tree_r[j][1]) / 120);
            glTranslatef((tree_r[j][0] * 20.0), 1.5 * (tree_r[j][1]), (tree_r[j][2] * 20.0));

            glScalef(4.0, 15.0, 4.0);
            glRotatef(-90, 1, 0, 0);
            gluCylinder(cylind, 0.5 * (tree_r[j][1]), 0.2, 4, 12, 3);
            glRotatef(90, 1, 0, 0);
            glColor3f((float)tree_r[j][0] / 80, (float)tree_r[j][1] / 80, (float)tree_r[j][2] / 60);
            glTranslatef(0, -0.115 * (tree_r[j][1]), 0);
            glScalef(0.3 * tree_r[j][1], 0.25 * tree_r[j][1], 0.3 * tree_r[j][1]);

            draw_cube();
            glPopMatrix();
            n -= 1.0; j--;
        }

        if (!person) {

            glColor3f(0.0, 0.8, 0.8);
            /*-------Draw the car body which is a sphere----*/
            glPushMatrix();
            glTranslatef(position[0], position[1], position[2]);
            glRotatef(self_ang, 0.0, 1.0, 0.0);

            glRotatef(-tilt_ad, 1.0, 0.0, 0.0);
            glRotatef(-tilt_ws, 0.0, 0.0, 1.0);

            glPushMatrix();              /* Save M1 coord. sys */
            glTranslatef(0.0, 4.0, 0.0);
            glScalef(17.0, 6.0, 6.0);    /* Scale up the axes */
            //draw_cube();
            gluSphere(sphere, 0.8,   /* radius=1.5 */
                12,            /* composing of 12 slices*/
                12);           /* composing of 12 stacks */
            glColor3f(0.3, 0.5, 0.8);
            glTranslatef(0.55, 0.15, 0.0);
            glRotatef(90, 0.0, 0.0, 1.0);
            glScalef(0.6, 0.4, 0.7);
            gluSphere(sphere, 0.8,   /* radius=1.5 */
                12,            /* composing of 12 slices*/
                12);
            glPopMatrix();               /* Get M1 back */

            /*-------Draw the front wheels -----*/
            glColor3f(1.0, 0.0, 0.0);
            glPushMatrix();              /* Save M1 coord. sys */
            glTranslatef(-6.0, 0.0, 4.0); /* Go to left wheel position x:-6*/
            glutSolidTorus(1.0,  /* inner radius */
                2.0,  /* outer radius */
                24,   /* divided into 18 segments */
                12);  /* 12 rings */
            glPopMatrix();

            glPushMatrix();              /* Save M1 coord. sys */
            glTranslatef(-6.0, 0.0, -4.0);/* Go to right wheel position */
            glutSolidTorus(1.0,  /* inner radius */
                2.0,  /* outer radius */
                24,   /* divided into 18 segments */
                12);  /* 12 rings */
            glPopMatrix();

            /*------Draw back wheels ----*/
            glColor3f(1.0, 0.4, 0.0);
            //glPushMatrix();              /* Save M1 coord. sys */
            //glTranslatef(6.0, 0.0, 4.0); /* Go to left wheel position */
            //glutSolidTorus(1.0,  /* inner radius */
            //               2.0,  /* outer radius */
            //               24,   /* divided into 18 segments */
            //              12);  /* 12 rings */
            //glPopMatrix();

            glPushMatrix();              /* Save M1 coord. sys */

            glTranslatef(9.0, 0.0, 0.0);/* Go to right wheel position */
            glRotatef(ad_ang, 0.0, 1.0, 0.0);
            glutSolidTorus(1.0,  /* inner radius */
                2.0,  /* outer radius */
                24,   /* divided into 18 segments */
                12);  /* 12 rings */
            glPopMatrix();
            glPushMatrix();
            glRotatef(-90, 1.0, 0.0, 0.0);
            glColor3f(0.68, 0.68, 0.68);
            gluCylinder(cylind, 1.0, 1.0, 12.0, 12, 3);
            glTranslatef(0.0, 0.0, 12.0);
            glColor3f(0.2, 0.2, 0.95);
            gluSphere(sphere, 0.5,   /* radius=1.5 */
                12,            /* composing of 12 slices*/
                12);           /* composing of 12 stacks */
            glPopMatrix();

            glPushMatrix();

            glColor3f(1.0, 0.5, 0.0);

            glTranslatef(-7.0, 4.5, 0.0);
            glScalef(4.0, 1.25, 1.0);
            glRotatef(-90, 0.0, 1.0, 0.0);
            gluCylinder(cylind, 2.0, 1.0, 4.0, 12, 3);
            //glTranslatef(-16.0, 3.0, 1.5);
            glPopMatrix();
            glPushMatrix();

            /*------Draw three blades ---*/
            glColor3f(1.0, 1.0, 1.0);
            glTranslatef(-21.0, 5.0, 1.5);
            glRotatef(-90.0, 0.0, 0.0, 1.0);
            glRotatef(arm_ang, 0.0, 0.0, 1.0);  /* Rotate about x axis, M5 coord. sys. */
            glScalef(0.5, 0.5, 0.5);
            draw_blade();/* draw the first blade */

            glRotatef(120.0, 0.0, 0.0, 1.0);    /* rotate by 120 degree, M6 coord. sys */
            draw_blade();/* draw 2nd blade */

            glRotatef(120.0, 0.0, 0.0, 1.0);    /* rotate by 120 degree,M7 coord. sys */
            draw_blade();/* draw 3rd blade */

            glPopMatrix();
            //glPopMatrix();
            glPushMatrix();
            glTranslatef(0.0, 12.0, 0.0);
            glScalef(2.0, 2.0, 2.0);
            glRotatef(90.0, 1.0, 0.0, 0.0);
            glRotatef(bld_ang, 0.0, 0.0, 1.0);  /* Rotate about x axis, M5 coord. sys. */
            glRotatef(5.0, 1.0, 1.0, 0.0);
            draw_blade();/* draw the first blade */
            glRotatef(-5.0, 1.0, 1.0, 0.0);
            glRotatef(120.0, 0.0, 0.0, 1.0);    /* rotate by 120 degree, M6 coord. sys */
            glRotatef(5.0, 1.0, 1.0, 0.0);
            draw_blade();/* draw 2nd blade */
            glRotatef(-5.0, 1.0, 1.0, 0.0);
            glRotatef(120.0, 0.0, 0.0, 1.0);    /* rotate by 120 degree,M7 coord. sys */
            glRotatef(5.0, 1.0, 1.0, 0.0);
            draw_blade();/* draw 3rd blade */
            glPopMatrix();
            glPopMatrix();
        }
    }
    /*-------Swap the back buffer to the front --------*/
    glutSwapBuffers();

    return;
}


void motion_func(int  x, int y) {
    if (!view_mode) {
        if (!first) { first = 1; posx = x; posy = y; }
        else { dx += (x - posx); dx %= 360; dy += (y - posy); dy %= 360; first = 0; }
    }if (view_mode == 1) {
        if (!first) { first = 1; posx = x; posy = y; }
        udward(0); vy = 512 - y;
    }
    display();
    //posx = x; posy = y;
}
void mouseWheel(int button, int dir, int x, int y)
{
    if (dir > 0)
    {
        if (zoom > 0.3)zoom -= 0.2; // Zoom in
    }
    else
    {
        if (zoom < 10.0)zoom += 0.2; // Zoom out
    }

    return;
}
void mouse_func(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)first = 0;
}
void mouse_move(int x, int y) {
    if (view_mode == 1) {
        if (!first) { first = 1; posx = x; posy = y; }
        udward(1);
        if (x > width - 128)rlward(0);
        if (x < 128)rlward(1);
        if ((y) > (height / 3))fbward(1);
        if (y < (height / 3))fbward(0);

    }glutPostRedisplay();
}
/*--------------------------------------------------
 * Reshape callback function which defines the size
 * of the main window when a reshape event occurrs.
 */
void my_reshape(int w, int h)
{

    width = w;
    height = h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w > h)
        glOrtho(-40.0, 40.0, -40.0 * (float)h / (float)w, 40.0 * (float)h / (float)w,
            -100.0, 100.0);
    else
        glOrtho(-40.0 * (float)w / (float)h, 40.0 * (float)w / (float)h, -40.0, 40.0,
            -100.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void keyUp(unsigned char key, int x, int y) {
    keyStates[key] = false; // Set the state of the current key to not pressed  
    if (tkey > 0)tkey -= 1;
    //display();
}

/*--------------------------------------------------
 * Keyboard callback func. When a key is pressed,
 * the program is aborted.
 */
void my_key(unsigned char key, int x, int y)
{
    if (key == 'v') {
        dx = dy = dz = 0;
        gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    }
    if (key == 'z') {
        por = (por) ? 0 : 1;
    }
    if (key == 'c') {
        cord = (cord) ? 0 : 1;
    }if (key == 'x') {
        dx = dy = dz = 0;
        xp = (xp + 1) % 4;
    }if (key == 'b') {
        pog = (pog) ? 0 : 1;
    }if (key == 'f') {
        ft = 1200;
        //dx = dy = dz = 0;
        person = (person) ? 0 : 1;
        temppo[0] = position[0];
        temppo[1] = position[1];
        temppo[2] = position[2];
    }
    if (key == 'h') { position[0] = 0; position[1] = 0; position[2] = 0; }
    keyStates[key] = true;
    tkey += 1;
    display();
}

/*---------------------------------------------------------------
 * Callback function for top menu. Do nothing.
 */
void top_menu_func(int value)
{
    if (value == 1) {
        view_mode = 0;
    }
    else if (value == 2) {
        view_mode = 1;
    }
    else if (value == 3) {
        view_mode = 2;
    }
    else if (value == 4) {
        zoom = 1.0;
    }
    else if (value == 5) {
        zoom = 0.3;
    }
    else if (value == 6) {
        persp = (persp) ? 0 : 1;
    }
}
//glPushMatrix();
GLfloat sun_light_position[] = { 0.0f, 15.0f, 0.0f, 1.0f }; //光源的位置在世界座標系圓心，齊次座標形式
GLfloat sun_light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //RGBA模式的環境光，為0
GLfloat sun_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //RGBA模式的漫反射光，全白光
GLfloat sun_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };  //RGBA模式下的鏡面光 ，全白光

GLfloat sun_mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };  //定義材質的環境光顏色，為0
GLfloat sun_mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };  //定義材質的漫反射光顏色，為0
GLfloat sun_mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };   //定義材質的鏡面反射光顏色，為0
GLfloat sun_mat_emission[] = { 0.2f, 0.2f, 0.2f, 1.0f };   //定義材質的輻射廣顏色，為偏紅色
GLfloat sun_mat_shininess = 100.0f;

//glPopMatrix();
/*---------------------------------------------------
 * Main procedure which defines the graphics environment,
 * the operation style, and the callback func's.
 */
void main(int argc, char** argv)
{
    /*-----Initialize the GLUT environment-------*/
    glutInit(&argc, argv);

    /*-----Depth buffer is used, be careful !!!----*/
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(width, height);
    glutCreateWindow("FlyingCar");

    myinit();      /*---Initialize other state varibales----*/

    /*----Associate callback func's whith events------*/
    glutDisplayFunc(display);
    /*  glutIdleFunc(display); */
    glutReshapeFunc(my_reshape);
    glutMotionFunc(motion_func);/* Mouse motion event callback func */
    glutMouseFunc(mouse_func);  /* Mouse Button Callback func */
    glutPassiveMotionFunc(mouse_move);
    glutMouseWheelFunc(mouseWheel);
    glutKeyboardFunc(my_key);
    glutKeyboardUpFunc(keyUp);
    top_m = glutCreateMenu(top_menu_func);
    glutAddMenuEntry("view_motion", 1);
    glutAddMenuEntry("view_key", 2);
    glutAddMenuEntry("view_auto", 3);
    glutAddMenuEntry("zoom_normal", 4);
    glutAddMenuEntry("zoom_min", 5);
    glutAddMenuEntry("perspective", 6);

    glutAttachMenu(GLUT_RIGHT_BUTTON);    /* associate top-menu with right but*/
    /*
    glEnable(GL_DEPTH_TEST);                                      //開啟燈光
    glDepthFunc(GL_LESS);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
    glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
    glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);
    draw_cube();*/
    //glutTimerFunc(1000,display,0);
    glutMainLoop();
}

