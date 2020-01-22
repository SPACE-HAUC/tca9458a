#include "tca9458a.h"

int tca9458a_init(tca9458a *dev, uint8_t addr, const char *fname)
{
    int status = 1;
    // allocate memory for dev
    dev = (tca9458a *)malloc(sizeof(tca9458a));
    // open bus
    dev->fd = open(fname, O_RDWR);
    if (dev->fd < 0)
    {
        status &= 0;
        perror("[TCA9458A] Error opening FD");
        return status;
    }
    // execute ioctl to associate FD with address
    if ((status = ioctl(dev->fd, I2C_SLAVE, addr)) < 0)
    {
        perror("[TCA9458A] I2C ioctl failed");
        return -1;
    }
    else
        status = 1;
    dev->channel = 0x00;
    // disable all output
    status &= tca9458a_set(dev);
    return status;
}

inline int tca9458a_set(tca9458a *dev)
{
    return write(dev->fd, &(dev->channel), 1);
}

void tca9458a_destroy(tca9458a *dev)
{
    if (dev == NULL)
        return;
    dev->channel = 0x00;
    // disable mux
    tca9458a_set(dev);
    // close file descriptor
    close(dev->fd);
    // free allocated memory
    free(dev);
    return;
}