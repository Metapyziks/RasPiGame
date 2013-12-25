#include <string.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>

#include "lcd.h"

#define SET_PIXEL(x, y, clr) (fb[(x) + (DISPLAY_HEIGHT - y - 1) * DISPLAY_WIDTH] = clr)

static color_t fb[DISPLAY_WIDTH * DISPLAY_HEIGHT];

void lcd_idleFunc(void (*idleFunc)(void))
{
    glutIdleFunc(idleFunc);
}

void lcd_displayFunc(void (*displayFunc)(void))
{
    glutDisplayFunc(displayFunc);
}

int lcd_init(void)
{
    int argc = 0;
    char** argv = NULL;

    glutInit(&argc, argv);
    glutInitWindowSize(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("RasPiGame");

    return TRUE;
}

void lcd_mainLoop(void)
{
    glutMainLoop();
}

void lcd_stop(void)
{
    return;
}

void lcd_swapBuffers(void)
{
    glRasterPos2i(-1, -1);
    glDrawPixels(DISPLAY_WIDTH, DISPLAY_HEIGHT, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, fb);
    glutSwapBuffers();
}

#include "lcd.c"

button_t lcd_getButtons(void)
{
    return 0;
}

int lcd_buttonDown(button_t button)
{
    return FALSE;
}
