
#ifndef _QUAGGA_JHASH_H
#define _QUAGGA_JHASH_H

extern u_int32_t jhash(void *key, u_int32_t length, u_int32_t initval);

extern u_int32_t jhash2(u_int32_t *k, u_int32_t length, u_int32_t initval);

extern u_int32_t jhash_3words(u_int32_t a, u_int32_t b, u_int32_t c, u_int32_t initval);
extern u_int32_t jhash_2words(u_int32_t a, u_int32_t b, u_int32_t initval);
extern u_int32_t jhash_1word(u_int32_t a, u_int32_t initval);

#endif 
