#include <stp/proto_stp_mstp.h>
#include <stp/proto_stp_mstp_digest.h>
#define	ROTATE_LEFT(x, n)		(((x) << (n)) | ((x) >> (32 - (n))))
#define	F(x, y, z)				(((x) & (y)) | ((~(x)) & (z)))
#define	G(x, y, z)				(((x) & (z)) | ((y) & (~(z))))
#define	H(x, y, z)				((x) ^ (y) ^ (z))
#define	I(x, y, z)				((y) ^ ((x) | (~(z))))
#define	R1(a, b, c, d, x, s, t)	{ a = ((b) + ROTATE_LEFT(((a) + F((b), (c), (d)) + (x) + (t)), (s))); }
#define R2(a, b, c, d, x, s, t)	{ a = ((b) + ROTATE_LEFT(((a) + G((b), (c), (d)) + (x) + (t)), (s))); }
#define	R3(a, b, c, d, x, s, t)	{ a = ((b) + ROTATE_LEFT(((a) + H((b), (c), (d)) + (x) + (t)), (s))); }
#define	R4(a, b, c, d, x, s, t)	{ a = ((b) + ROTATE_LEFT(((a) + I((b), (c), (d)) + (x) + (t)), (s))); }

typedef struct mstp_hmac_s
{
    uint8   k_opad[64];
    uint8   i_md5[16];

    uint8   k_ipad[64];
    uint16  vlanMsti[4096];
} mstp_hmac_t;
static mstp_hmac_t hmac = {
    .k_opad =
    {
        0x4F, 0xF0, 0x5A, 0xFA, 0x72, 0x1B, 0xA1, 0x0D,
        0xA5, 0x01, 0x77, 0xFE, 0x1F, 0x91, 0x5F, 0x1A,
        0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C,
        0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C,
        0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C,
        0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C,
        0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C,
        0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C,
    },
    .i_md5 = { 0x0 },

    .k_ipad =
    {
        0x25, 0x9A, 0x30, 0x90, 0x18, 0x71, 0xCB, 0x67,
        0xCF, 0x6B, 0x1D, 0x94, 0x75, 0xFB, 0x35, 0x70,
        0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
        0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
        0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
        0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
        0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
        0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,
    },
    .vlanMsti = { 0x0 },
};
static void md5 (uint8 *data, uint32 len, uint8 *md5)
{
    uint32 i, j;
    uint32 blk;
    static uint32 a, b, c, d;
    uint32 aa, bb, cc, dd;
    uint8  buf[64];
    uint32 x[16];

    blk = (((len + 1 + 8) / 64) + 1);

    {
        a = 0x67452301;
        b = 0xEFCDAB89;
        c = 0x98BADCFE;
        d = 0x10325476;
    }

    for (i=0; i<blk; i++)
    {
        
        for (j=0; j<64; j++)
        {
            if (((i << 6) + j) < len)
            {
                buf[j] = data[((i << 6) + j)];
            }
            else
            {
                buf[j] = (((i << 6) + j) == len)? 0x80 : 0x00;
            }
        }

        if (i < (blk - 1))
        {
            for (j=0; j<16; j++)
            {
                x[j] = ((buf[((4 * j) + 3)] << 24) | \
                        (buf[((4 * j) + 2)] << 16) | \
                        (buf[((4 * j) + 1)] <<  8) | \
                        (buf[((4 * j) + 0)] <<  0));
            }
        }
        else
        {
            for (j=0; j<14; j++)
            {
                x[j] = ((buf[((4 * j) + 3)] << 24) | \
                        (buf[((4 * j) + 2)] << 16) | \
                        (buf[((4 * j) + 1)] <<  8) | \
                        (buf[((4 * j) + 0)] <<  0));
            }
            x[14] = (len << 3);
            x[15] = (len >> 29);
        }

        aa = a;
        bb = b;
        cc = c;
        dd = d;

        R1(a, b, c, d, x[ 0],  7, 0xD76AA478);
        R1(d, a, b, c, x[ 1], 12, 0xE8C7B756);
        R1(c, d, a, b, x[ 2], 17, 0x242070DB);
        R1(b, c, d, a, x[ 3], 22, 0xC1BDCEEE);
        R1(a, b, c, d, x[ 4],  7, 0xF57C0FAF);
        R1(d, a, b, c, x[ 5], 12, 0x4787C62A);
        R1(c, d, a, b, x[ 6], 17, 0xA8304613);
        R1(b, c, d, a, x[ 7], 22, 0xFD469501);
        R1(a, b, c, d, x[ 8],  7, 0x698098D8);
        R1(d, a, b, c, x[ 9], 12, 0x8B44F7AF);
        R1(c, d, a, b, x[10], 17, 0xFFFF5BB1);
        R1(b, c, d, a, x[11], 22, 0x895CD7BE);
        R1(a, b, c, d, x[12],  7, 0x6B901122);
        R1(d, a, b, c, x[13], 12, 0xFD987193);
        R1(c, d, a, b, x[14], 17, 0xA679438E);
        R1(b, c, d, a, x[15], 22, 0x49B40821);

        R2(a, b, c, d, x[ 1],  5, 0xF61E2562);
        R2(d, a, b, c, x[ 6],  9, 0xC040B340);
        R2(c, d, a, b, x[11], 14, 0x265E5A51);
        R2(b, c, d, a, x[ 0], 20, 0xE9B6C7AA);
        R2(a, b, c, d, x[ 5],  5, 0xD62F105D);
        R2(d, a, b, c, x[10],  9, 0x02441453);
        R2(c, d, a, b, x[15], 14, 0xD8A1E681);
        R2(b, c, d, a, x[ 4], 20, 0xE7D3FBC8);
        R2(a, b, c, d, x[ 9],  5, 0x21E1CDE6);
        R2(d, a, b, c, x[14],  9, 0xC33707D6);
        R2(c, d, a, b, x[ 3], 14, 0xF4D50D87);
        R2(b, c, d, a, x[ 8], 20, 0x455A14ED);
        R2(a, b, c, d, x[13],  5, 0xA9E3E905);
        R2(d, a, b, c, x[ 2],  9, 0xFCEFA3F8);
        R2(c, d, a, b, x[ 7], 14, 0x676F02D9);
        R2(b, c, d, a, x[12], 20, 0x8D2A4C8A);

        R3(a, b, c, d, x[ 5],  4, 0xFFFA3942);
        R3(d, a, b, c, x[ 8], 11, 0x8771F681);
        R3(c, d, a, b, x[11], 16, 0x6D9D6122);
        R3(b, c, d, a, x[14], 23, 0xFDE5380C);
        R3(a, b, c, d, x[ 1],  4, 0xA4BEEA44);
        R3(d, a, b, c, x[ 4], 11, 0x4BDECFA9);
        R3(c, d, a, b, x[ 7], 16, 0xF6BB4B60);
        R3(b, c, d, a, x[10], 23, 0xBEBFBC70);
        R3(a, b, c, d, x[13],  4, 0x289B7EC6);
        R3(d, a, b, c, x[ 0], 11, 0xEAA127FA);
        R3(c, d, a, b, x[ 3], 16, 0xD4EF3085);
        R3(b, c, d, a, x[ 6], 23, 0x04881D05);
        R3(a, b, c, d, x[ 9],  4, 0xD9D4D039);
        R3(d, a, b, c, x[12], 11, 0xE6DB99E5);
        R3(c, d, a, b, x[15], 16, 0x1FA27CF8);
        R3(b, c, d, a, x[ 2], 23, 0xC4AC5665);

        R4(a, b, c, d, x[ 0],  6, 0xF4292244);
        R4(d, a, b, c, x[ 7], 10, 0x432AFF97);
        R4(c, d, a, b, x[14], 15, 0xAB9423A7);
        R4(b, c, d, a, x[ 5], 21, 0xFC93A039);
        R4(a, b, c, d, x[12],  6, 0x655B59C3);
        R4(d, a, b, c, x[ 3], 10, 0x8F0CCC92);
        R4(c, d, a, b, x[10], 15, 0xFFEFF47D);
        R4(b, c, d, a, x[ 1], 21, 0x85845DD1);
        R4(a, b, c, d, x[ 8],  6, 0x6FA87E4F);
        R4(d, a, b, c, x[15], 10, 0xFE2CE6E0);
        R4(c, d, a, b, x[ 6], 15, 0xA3014314);
        R4(b, c, d, a, x[13], 21, 0x4E0811A1);
        R4(a, b, c, d, x[ 4],  6, 0xF7537E82);
        R4(d, a, b, c, x[11], 10, 0xBD3AF235);
        R4(c, d, a, b, x[ 2], 15, 0x2AD7D2BB);
        R4(b, c, d, a, x[ 9], 21, 0xEB86D391);

        a += aa;
        b += bb;
        c += cc;
        d += dd;
    } 

    md5[ 0] = (uint8)((a >>  0) & 0xFF);
    md5[ 1] = (uint8)((a >>  8) & 0xFF);
    md5[ 2] = (uint8)((a >> 16) & 0xFF);
    md5[ 3] = (uint8)((a >> 24) & 0xFF);
    md5[ 4] = (uint8)((b >>  0) & 0xFF);
    md5[ 5] = (uint8)((b >>  8) & 0xFF);
    md5[ 6] = (uint8)((b >> 16) & 0xFF);
    md5[ 7] = (uint8)((b >> 24) & 0xFF);
    md5[ 8] = (uint8)((c >>  0) & 0xFF);
    md5[ 9] = (uint8)((c >>  8) & 0xFF);
    md5[10] = (uint8)((c >> 16) & 0xFF);
    md5[11] = (uint8)((c >> 24) & 0xFF);
    md5[12] = (uint8)((d >>  0) & 0xFF);
    md5[13] = (uint8)((d >>  8) & 0xFF);
    md5[14] = (uint8)((d >> 16) & 0xFF);
    md5[15] = (uint8)((d >> 24) & 0xFF);
}
int32 mstp_digest_calc(uint16 *vlanMstiMap, uint8 *digest)
{
    static uint32 size = ((MAX_MSTP_VLAN_NUM * sizeof(*vlanMstiMap)) < sizeof(hmac.vlanMsti)) ?
                    (MAX_MSTP_VLAN_NUM * sizeof(*vlanMstiMap)) : sizeof(hmac.vlanMsti);

    osal_memcpy(hmac.vlanMsti, vlanMstiMap, size);

    md5(hmac.k_ipad, sizeof(hmac.k_ipad) + sizeof(hmac.vlanMsti), hmac.i_md5);
    
    md5(hmac.k_opad, sizeof(hmac.k_opad) + sizeof(hmac.i_md5), digest);

    return SYS_ERR_OK;
}
