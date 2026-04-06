#ifndef __COMMON_SYS_BITMAP_H__
#define __COMMON_SYS_BITMAP_H__
 
#define SYS_BITMAP_SET_BIT_ltoR(addr, bit) \
	do{ \
		addr[(bit)/SYS_BITS_PER_LONG] |= (1 <<(SYS_BITS_PER_LONG -((bit) % SYS_BITS_PER_LONG)-1)); \
	}while(0)

#define SYS_BITMAP_DECLARE(name, bits) \
    uint32 name[SYS_BITS_TO_LONGS(bits)]

#define SYS_BITMAP_SET_BIT(addr, bit) \
do{ \
    addr[(bit)/SYS_BITS_PER_LONG] |= (1 << ((bit) % SYS_BITS_PER_LONG)); \
}while(0)

#define SYS_BITMAP_CLEAR_BIT(addr, bit) \
do{ \
    addr[(bit)/SYS_BITS_PER_LONG] &= ~(1 << ((bit) % SYS_BITS_PER_LONG)); \
}while(0)

#define SYS_BITMAP_IS_BITSET(addr, bit) \
    (addr[(bit)/SYS_BITS_PER_LONG] & (1 << ((bit) % SYS_BITS_PER_LONG)))

#define SYS_BITMAP_IS_BITCLEAR(addr, bit) \
    !(addr[(bit)/SYS_BITS_PER_LONG] & (1 << ((bit) % SYS_BITS_PER_LONG)))

#define SYS_BITMAP_LASTWORD_MASK(bits) \
    (((bits)%SYS_BITS_PER_LONG) ? (1UL << ((bits)%SYS_BITS_PER_LONG))-1 : ~0UL )

#define SYS_BITMAP_SET_ALL(addr, bits) \
do{ \
    uint32 _n = SYS_BITS_TO_LONGS(bits); \
    if(_n > 1) {\
        uint32 _len = (_n - 1) * sizeof(uint32); \
        osal_memset(addr, 0xFF, _len); \
    } \
    addr[_n - 1] = SYS_BITMAP_LASTWORD_MASK(bits); \
}while(0)

#define SYS_BITMAP_CLEAR_ALL(addr, bits) \
do{ \
    osal_memset(addr, 0, SYS_BITS_TO_LONGS(bits) * sizeof(uint32)); \
}while(0)

#define SYS_BITMAP_IS_CLEAR(addr, bits, result) \
do{ \
    uint32 _i, _n = SYS_BITS_TO_LONGS(bits); \
    result = 1; \
    for(_i = 0; _i < _n; _i ++) \
    { \
        if (0 != addr[_i]) result = 0; \
    } \
}while(0)

#define SYS_BITMAP_IS_EQUAL(addr1, addr2, bits, result) \
do{ \
    uint32 _i, _n = SYS_BITS_TO_LONGS(bits); \
    result = 1; \
    for(_i = 0; _i < _n; _i ++) \
    { \
        if (addr1[_i] != addr2[_i]) result = 0; \
    } \
}while(0)

#define SYS_BITMAP_COPY(dst, src, bits) \
do{ \
    uint32 _len = SYS_BITS_TO_LONGS(bits) * sizeof(uint32); \
    osal_memcpy(dst, src, _len); \
}while(0)

#define SYS_BITMAP_COMPLEMENT(dst, src, bits) \
do{ \
    uint32 _i, _j = (bits) / SYS_BITS_PER_LONG; \
    for(_i = 0; _i < _j; ++_i) \
        dst[_i] = ~src[_i]; \
    if((bits) % SYS_BITS_PER_LONG) \
        dst[_i] = ~src[_i] & SYS_BITMAP_LASTWORD_MASK(bits); \
}while(0)

#define SYS_BITMAP_AND(dst, bitmap1, bitmap2, bits) \
do{ \
    uint32 _i, _n = SYS_BITS_TO_LONGS(bits); \
    for(_i = 0; _i < _n; _i++) \
        dst[_i] = bitmap1[_i] & bitmap2[_i]; \
}while(0)

#define SYS_BITMAP_OR(dst, bitmap1, bitmap2, bits) \
do{ \
    uint32 _i, _n = SYS_BITS_TO_LONGS(bits); \
    for(_i = 0; _i < _n; _i++) \
        dst[_i] = bitmap1[_i] | bitmap2[_i]; \
}while(0)

#define SYS_BITMAP_XOR(dst, bitmap1, bitmap2, bits) \
do{ \
    uint32 _i, _n = SYS_BITS_TO_LONGS(bits); \
    for(_i = 0; _i < _n; _i++) \
        dst[_i] = bitmap1[_i] ^ bitmap2[_i]; \
}while(0)

#define SYS_BITMAP_ANDNOT(dst, bitmap1, bitmap2, bits) \
do{ \
    uint32 _i, _n = SYS_BITS_TO_LONGS(bits); \
    for(_i = 0; _i < _n; _i++) \
        dst[_i] = bitmap1[_i] & ~bitmap2[_i]; \
}while(0)
#endif 

