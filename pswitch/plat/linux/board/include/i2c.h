
#ifndef __INTF_I2C_H__
#define __INTF_I2C_H__

#include <drv/i2c/i2c.h>

#define I2C_DFL_CHIP_ID   0x50
#define I2C_DFL_DELAY     50
#define I2C_DFL_DELAY_20  10
#define I2C_DFL_DELAY_4000 4000

#define I2C_DEVICE_CHK(dev)     ((dev >= 0) && (dev < I2C_DEV_ID_END))

extern int32 i2c_dev_init(i2c_devConf_t *pI2cConf);

extern int32 i2c_byte_get(i2c_devConf_t *pI2cConf, uint32 addr, uint32 *pData);

extern int32 i2c_byte_set(i2c_devConf_t *pI2cConf, uint32 addr, uint32 data);

extern int32 i2c_halfWord_get(i2c_devConf_t *pI2cConf, uint32 addr, uint32 *pData);

extern int32 i2c_word_get(i2c_devConf_t *pI2cConf, uint32 addr, uint32 *pData);
extern int32 i2c_bigbuff_byte_set(i2c_devConf_t *pI2cConf, uint32 addr, uint8* pData);
extern int32 i2c_two_byte_set(i2c_devConf_t *pI2cConf, uint32 addr, uint8* pData);
extern int32 i2c_two_byte_get(i2c_devConf_t *pI2cConf, uint32 addr, uint32 *pData);
#endif 

