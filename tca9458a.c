/**
 * @file tca9458a.c
 * @author Sunip K. Mukherjee (sunipkmukherjee@gmail.com)
 * @brief Function definitions for TCA9458A I2C driver
 * @version 0.1
 * @date 2020-03-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <i2cbus/i2cbus.h>
#include "tca9458a.h"

#define eprintf(...) fprintf(stderr, __VA_ARGS__)

/**
 * @brief Initialize a Mux device, returns 1 on success
 *  TODO: Implement a scan function at init where it checks all 3 CSS are
 * present on 3 buses?
 * 
 * @param dev tca9458a I2C Mux handle
 * @param id X in /dev/i2c-X
 * @param addr TCA9458A device address (default: 0x70)
 * @param ctx Context for I2C bus driver for singular lock operation
 * @return 1 on success, -1 on error
 */
int tca9458a_init(tca9458a *dev, uint8_t id, uint8_t addr, uint8_t ctx)
{
    int status = 1;
    dev->bus = (i2cbus *)malloc(sizeof(i2cbus));
    if (dev->bus == NULL)
    {
        eprintf("%s: Error allocating ", __func__);
        perror("memory");
        return -1;
    }
    if (addr == 0)
    {
        eprintf("%s: Warning: No address specified, assuming default address 0x70\n", __func__);
        addr = 0x70;
    }
    status = i2cbus_open(dev->bus, id, addr);
    if (status < 0)
    {
        eprintf("%s: Error opening I2C device, ", __func__);
        perror("i2c");
        return -1;
    }
    dev->bus->ctx = ctx;
    status &= tca9458a_set(dev, 8);
    return status;
}
/**
 * @brief Disable all outputs, close file descriptor for the I2C Bus.
 * 
 * @param dev 
 */
void tca9458a_destroy(tca9458a *dev)
{
    if (dev == NULL)
        return;
    // disable mux
    tca9458a_set(dev, 8);
    // close file descriptor
    i2cbus_close(dev->bus);
    // free allocated memory
    free(dev->bus);
    return;
}

#ifdef UNIT_TEST
#include <signal.h>
#include <stdio.h>
#include <string.h>
volatile sig_atomic_t done = 0;

void sighandler(int sig)
{
    done = 1;
#ifdef TCA9458A_DEBUG
    eprintf("%s: Received signal %d\n", __func__, sig);
#endif
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Invocation: ./%s <Bus ID> <Address (in hex)>\n\n", argv[0]);
        return 0;
    }
    int id = atoi(argv[0]);
    int addr = (int)strtol(argv[2], NULL, 16); // convert to hex
    if (addr != 0x70)
    {
        printf("0x%02x may not appropriate for TCA9458A...\n", addr);
    }
    tca9458a *dev = (tca9458a *)malloc(sizeof(tca9458a));
    if (tca9458a_init(dev, id, addr, 0x0) < 0)
    {
        printf("Could not initialize device, exiting...\n");
        goto end;
    }
    signal(SIGINT, &sighandler);
    char c = '\0';
    while (!done)
    {
        if (c != '\n')
            printf("[s]et channel, [q]uit: ");
        c = getchar();
        switch (c)
        {
        case 's':
        case 'S':
            printf("Enter channel ID (0 -- 7): ");
            int i = 0;
            scanf(" %d", &i);
            if (tca9458a_set(dev, i) < 0)
            {
                perror("Error setting channel");
            }
            break;
        case 'q':
        case 'Q':
            done = 1;
            break;
        default:
            break;
        }
    }
    printf("\n");
end:
    tca9458a_destroy(dev);
    free(dev);
}
#endif