#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

static GLenum spinMode = GL_TRUE;
static GLenum singleStep = GL_FALSE;

void OpenGLInit(void);
static void Animate(void );
static void Key_r(void );
static void Key_s(void );
static void Key_up(void );
static void Key_down(void );
static void ResizeWindow(int w, int h);
static void KeyPressFunc( unsigned char Key, int x, int y );
static void SpecialKeyFunc( int Key, int x, int y );

// These three variables control the animation's state and speed.
static float HourOfDay = 0.0;
static float DayOfYear = 0.0;
static float AnimateIncrement = 24.0; // Time step for animation (hours)

// glutKeyboardFunc is called below to set this function to handle
//		all normal key presses.
static void KeyPressFunc(unsigned char Key, int x, int y)
{
    switch (Key)
    {
    case 'R':
    case 'r':
        Key_r();
        break;
    case 's':
    case 'S':
        Key_s();
        break;
    case 27: // Escape key
        exit(1);
    }
}

// glutSpecialFunc is called below to set this function to handle
//		all special key presses.  See glut.h for the names of
//		special keys.
static void SpecialKeyFunc(int Key, int x, int y)
{
    switch (Key)
    {
    case GLUT_KEY_UP:
        Key_up();
        break;
    case GLUT_KEY_DOWN:
        Key_down();
        break;
    }
}

static void Key_r(void)
{
    if (singleStep)
    { // If ending single step mode
        singleStep = GL_FALSE;
        spinMode = GL_TRUE; // Restart animation
    }
    else
    {
        spinMode = !spinMode; // Toggle animation on and off.
    }
}

static void Key_s(void)
{
    singleStep = GL_TRUE;
    spinMode = GL_TRUE;
}

static void Key_up(void)
{
    AnimateIncrement *= 2.0; // Double the animation time step
}

static void Key_down(void)
{
    AnimateIncrement /= 2.0; // Halve the animation time step
}

/*
 * Animate() handles the animation and the redrawing of the
 *		graphics window contents.
 */
static void Animate(void)
{
    // Clear the redering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat mat_color_sun[] = {1.0, 1.0, 0.0, 1.0};
    GLfloat mat_color_earth[] = {0.2, 0.2, 1.0, 1.0};
    GLfloat mat_color_moon[] = {0.3, 0.7, 0.3, 1.0};

    if (spinMode)
    {
        // Update the animation state
        HourOfDay += AnimateIncrement;
        DayOfYear += AnimateIncrement / 24.0;  // 48 gio chia 24 = 2 ngay

        HourOfDay = HourOfDay - ((int)(HourOfDay / 24)) * 24;
        DayOfYear = DayOfYear - ((int)(DayOfYear / 365)) * 365;
    }

    // Clear the current matrix (Modelview)
    glLoadIdentity();

    // Back off eight units to be able to view from the origin.
    glTranslatef(0.0, 0.0, -8.0);

    // Rotate the plane of the elliptic
    // (rotate the model's plane about the x axis by fifteen degrees)
    glRotatef(15.0, 1.0, 0.0, 0.0);

    // Draw the sun	-- as a yellow, wireframe sphere
    glPushMatrix();
    // glColor3f(1.0, 1.0, 0.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color_sun);
    glRotatef(360.0 * DayOfYear / 27.0, 0.0, 1.0, 0.0); // Mat troi quay quanh chinh no mat khoang 27 ngay neu tinh tu vi tri xich dao
    glutSolidSphere(1.0, 30, 30);
    glPopMatrix();

    // Draw the Earth
    // First position it around the sun
    // Use DayOfYear to determine its position
    glRotatef(360.0 * DayOfYear / 365.0, 0.0, 1.0, 0.0);
    glTranslatef(4.0, 0.0, 0.0);
    glPushMatrix(); // Save matrix state
    // Second, rotate the earth on its axsis.
    // Use HourOfDay to determine its rotation.
    glRotatef(360.0 * HourOfDay / 24.0, 0.0, 1.0, 0.0);
    // Third, draw the earth as a wireframe sphere.
    // glColor3f(0.2, 0.2, 1.0);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color_earth);
    glutSolidSphere(0.4, 25, 25);
    glPopMatrix(); // Restore matrix state

    // Draw the moon.
    // Use DayOfYear to control its rotation around the earth
    glRotatef(360.0 * 12.5 * DayOfYear / 365.0, 0.0, 1.0, 0.0);
    //Mat trang quay quanh trai dat mat khoang 27.32 - 29.53 ngay  ==> lay trung binh khoang 12.5 do/ 1 ngay
    glTranslatef(0.7, 0.0, 0.0);
    // glColor3f(0.3, 0.7, 0.3);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_color_moon);
    glutSolidSphere(0.1, 15, 15);

    // Flush the pipeline, and swap the buffers
    glFlush();
    glutSwapBuffers();

    if (singleStep)
    {
        spinMode = GL_FALSE;
    }

    glutPostRedisplay(); // Request a re-draw for animation purposes
}

// Initialize OpenGL's rendering modes
//To bong va do bong cho doi
void OpenGLInit(void)
{
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = {50.0};
    GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
    glClearColor(0.65, 0.65, 0.65, 1.0);
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}

// ResizeWindow is called when the window is resized
static void ResizeWindow(int w, int h)
{
    float aspectRatio;
    h = (h == 0) ? 1 : h;
    w = (w == 0) ? 1 : w;
    glViewport(0, 0, w, h); // View port uses whole window
    aspectRatio = (float)w / (float)h;

    // Set up the projection view matrix (not very well!)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, aspectRatio, 1.0, 30.0);

    // Select the Modelview matrix
    glMatrixMode(GL_MODELVIEW);
}

// Main routine
// Set up OpenGL, hook up callbacks, and start the main loop
int main(int argc, char **argv)
{
    // Need to double buffer for animation
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // Create and position the graphics window
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(600, 360);
    glutCreateWindow("Solar System Demo");

    // Initialize OpenGL.
    OpenGLInit();

    // Set up callback functions for key presses
    glutKeyboardFunc(KeyPressFunc);
    glutSpecialFunc(SpecialKeyFunc);

    // Set up the callback function for resizing windows
    glutReshapeFunc(ResizeWindow);

    // Callback for graphics image redrawing
    glutDisplayFunc(Animate);

    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop();

    return (0); // Compiler requires this to be here. (Never reached)
}