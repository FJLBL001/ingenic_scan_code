#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>



#define dbprintf(...) do{}while(0)

//#define dbprintf printf


/***
state : freeze standby mem

***/

int set_power_state(char *power_state,int state_len)
{
    int power_fd = 0;

    power_fd = open("/sys/power/state",O_WRONLY);
    if (power_fd <0)
    {
        dbprintf("open power state fail\r\n");
        return -1;
    }

    if (write(power_fd, power_state, state_len) < 0)
    {
        dbprintf("write power state fail\r\n");
        close(power_fd);
        return -1;
    }

    close(power_fd);
    return 0;
}

