#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#define SSD1289_GET_KEYS _IOR('keys', 1, unsigned char *)
 
void get_keys(int fd)
{
    unsigned char keys;
 
    if (ioctl(fd, SSD1289_GET_KEYS, &keys) == -1)
    {
        perror("_apps ioctl get");
    }
    else
    {
        printf("Keys : %2x\n", keys);
    }
}
 
int main(int argc, char *argv[])
{
    char *file_name = "/dev/fb1";
    int fd;
    
    fd = open(file_name, O_RDWR);
    if (fd == -1)
    {
        perror("_apps open");
        return 2;
    }
 
    while(1)
    get_keys(fd);

    printf("Ioctl Number: (int)%d  (hex)%x\n", SSD1289_GET_KEYS, SSD1289_GET_KEYS);
    
    close (fd);
 
    return 0;
}
