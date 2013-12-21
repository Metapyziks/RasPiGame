#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

int main()
{
    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char *fbp = 0;
    int x = 0, y = 0;
    long int location = 0;

    // Open the file for reading and writing
    fbfd = open("/dev/fb1", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    // Figure out the size of the screen in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((int)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

    system("setterm -cursor off");

    // Figure out where in memory to put the pixel
    for (y = 0; y < 240; y++) for (x = 0; x < 320; x++) {
        location = (x+vinfo.xoffset) * 2 + (y+vinfo.yoffset) * finfo.line_length;

        int b = 10;
        int g = x/8;     // A little green
        int r = y/16;    // A lot of red
        unsigned short int t = r<<11 | g << 5 | b;
        *((unsigned short int*)(fbp + location)) = t;
    }

    sleep(5);

    for (y = 0; y < 240; y++) for (x = 0; x < 320; x++) {
        location = (x+vinfo.xoffset) * 2 + (y+vinfo.yoffset) * finfo.line_length;
        *((unsigned short int*)(fbp + location)) = 0;
    }

    system("setterm -cursor on");

    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
