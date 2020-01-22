#include "tca9458a.h"

int tca9458a_init(tca9458a *dev, uint8_t addr)
{
    int status = 1;
    // allocate memory for dev
    dev = (tca9458a *)malloc(sizeof(tca9458a));
    if ( dev == NULL )
    {
        perror("[TCA9458A] Error allocating memory for device");
    }
    // open bus
    dev->fd = open(dev->fname, O_RDWR);
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
    // disable all output
    status &= tca9458a_set(dev, 8);
    return status;
}

void tca9458a_destroy(tca9458a *dev)
{
    if (dev == NULL)
        return;
    // disable mux
    tca9458a_set(dev, 8);
    // close file descriptor
    close(dev->fd);
    // free allocated memory
    free(dev);
    return;
}