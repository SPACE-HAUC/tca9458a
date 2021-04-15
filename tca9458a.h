/**
 * @file tca9458a.h
 * @author Sunip K. Mukherjee (sunipkmukherjee@gmail.com)
 * @brief Function prototypes and struct declarations for TCA9458A I2C driver
 * @version 0.1
 * @date 2020-03-19
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef TCA9458A_H
#define TCA9458A_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <i2cbus/i2cbus.h>

/**
 * @brief TCA9458A Device handle.
 * 
 */
typedef struct
{
    i2cbus bus[1];
    uint8_t channel; ///< Current active channel
} tca9458a;
/**
 * @brief Initialize a TCA9458A Mux device
 * 
 * @param dev Pointer to TCA9458a struct
 * @param bus Bus ID
 * @param addr 
 * @param ctx 
 * @return int 
 */
int tca9458a_init(tca9458a *dev, uint8_t bus, uint8_t addr, uint8_t ctx);
/**
 * @brief Update active I2C channel (Inlined global symbol)
 * 
 * @param dev 
 * @param channel_id Channel to enable, 8 to disable MUX
 * @return Returns 1 on success, 0 or -1 on error (see write())
 */
static inline int tca9458a_set(tca9458a *dev, uint8_t channel_id)
{
    dev->channel = channel_id < 8 ? 0x01 << channel_id : 0x00;
#ifdef TCA9458A_DEBUG
    fprintf(stderr, "%s, %d: channel id: %u, command: 0x%01x\n", __func__, __LINE__, channel_id, dev->channel);
    fflush(stderr);
#endif
    return i2cbus_write(dev->bus, &(dev->channel), 1);
}
/**
 * @brief Disable all channels and close a tca9458a device
 * 
 * @param dev Pointer to tca9458a struct
 */
void tca9458a_destroy(tca9458a *dev);
#ifdef __cplusplus
}
#endif
#endif