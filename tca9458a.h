#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <errno.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#define MUX_I2C_FIle "/dev/i2c-1"

typedef struct
{
    int fd;
    uint8_t channel;
} tca9458a;

// Initialize a Mux device, returns 1 on success
// TODO: Implement a scan function at init where it checks all 3 CSS are present on 3 buses?
int tca9458a_init(tca9458a *, uint8_t addr, char *);

// Update active I2C channel using the 'channel' variable in the struct,
// returns 1 on success.
inline int tca9458a_set(tca9458a * dev, uint8_t channel_id)
{
    dev->channel = channel_id < 8 ? 0x01 << channel_id : 0x00;
    return write(dev->fd, &(dev->channel), 1);
}

// Disable all outputs, close fd
void tca9458a_destroy(tca9458a *);