#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>



#define dbprintf(...) do{}while(0)
//
//#define dbprintf printf

int create_gpio_node(int node)
{
    int fd;
    char tmp[5 + 1] = {0};
    sprintf(tmp, "%d", node);

    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd < 0)
    {
        dbprintf("open export fail\r\n");
        return -1;
    }

    if (write(fd, tmp, strlen(tmp)) < 0)
    {
        dbprintf("write export fail\r\n");
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

int destroy_gpio_node(int node)
{
    int fd;
    char tmp[5 + 1] = {0};
    sprintf(tmp, "%d", node);

    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (fd < 0)
    {
        dbprintf("open unexport fail\r\n");
        return -1;
    }

    if (write(fd, tmp, strlen(tmp)) < 0)
    {
        dbprintf("write unexport fail\r\n");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}


int set_gpio_output_high_low(int node, const char *high_low)
{
    int gpiofd;
    char tmp [256] = {0};
    sprintf(tmp, "/sys/class/gpio/gpio%d/direction", node);
    gpiofd = open(tmp, O_WRONLY);
    if (gpiofd < 0)
    {
        dbprintf("open direction fail\r\n");
        return -1;
    }

    if (write(gpiofd, high_low, strlen(high_low)) < 0)
    {
        dbprintf("write direction %s fail\r\n", high_low);
        close(gpiofd);
        return -1;
    }

    close(gpiofd);
    return 0;
}



