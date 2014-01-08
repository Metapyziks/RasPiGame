#include <string.h>
#include <stdlib.h>
#include <Windows.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>

#include "lcd.h"

#define SET_PIXEL(x, y, clr) (fb[(x) + (DISPLAY_HEIGHT - y - 1) * DISPLAY_WIDTH] = clr)

static color_t fb[DISPLAY_WIDTH * DISPLAY_HEIGHT];
static button_t pressedButtons;

void lcd_idleFunc(void (*idleFunc)(void))
{
    glutIdleFunc(idleFunc);
}

void lcd_displayFunc(void (*displayFunc)(void))
{
    glutDisplayFunc(displayFunc);
}

static void onKeyDownFunc(uint8_t key, int x, int y)
{
    switch (key) {
        case 'w':
            pressedButtons |= BTN_4; break;
        case 'a':
            pressedButtons |= BTN_3; break;
        case 'd':
            pressedButtons |= BTN_2; break;
        case 's':
            pressedButtons |= BTN_1; break;
        case ' ':
            pressedButtons |= BTN_0; break;
    }
}

static void onKeyUpFunc(uint8_t key, int x, int y)
{
    switch (key) {
        case 'w':
            pressedButtons &= ~BTN_4; break;
        case 'a':
            pressedButtons &= ~BTN_3; break;
        case 'd':
            pressedButtons &= ~BTN_2; break;
        case 's':
            pressedButtons &= ~BTN_1; break;
        case ' ':
            pressedButtons &= ~BTN_0; break;
    }
}

static void onSpecialKeyDownFunc(int key, int x, int y)
{
    switch (key) {
        case GLUT_KEY_UP:
            pressedButtons |= BTN_4; break;
        case GLUT_KEY_LEFT:
            pressedButtons |= BTN_3; break;
        case GLUT_KEY_RIGHT:
            pressedButtons |= BTN_2; break;
        case GLUT_KEY_DOWN:
            pressedButtons |= BTN_1; break;
    }
}

static void onSpecialKeyUpFunc(int key, int x, int y)
{
    switch (key) {
        case GLUT_KEY_UP:
            pressedButtons &= ~BTN_4; break;
        case GLUT_KEY_LEFT:
            pressedButtons &= ~BTN_3; break;
        case GLUT_KEY_RIGHT:
            pressedButtons &= ~BTN_2; break;
        case GLUT_KEY_DOWN:
            pressedButtons &= ~BTN_1; break;
    }
}

int lcd_init(void)
{
    int argc = 0;
    char** argv = NULL;

    glutInit(&argc, argv);
    glutInitWindowSize(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("RasPiGame");

    glutKeyboardFunc(onKeyDownFunc);
    glutKeyboardUpFunc(onKeyUpFunc);
    glutSpecialFunc(onSpecialKeyDownFunc);
    glutSpecialUpFunc(onSpecialKeyUpFunc);

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

    glutPostRedisplay();
}

#include "lcd.c"

button_t lcd_getButtons(void)
{
    return pressedButtons;
}

int lcd_buttonDown(button_t button)
{
    return (pressedButtons & button) == button;
}
