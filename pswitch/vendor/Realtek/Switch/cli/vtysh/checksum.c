
#include <zebra.h>
#include "checksum.h"

int            
in_cksum(void *parg, int nbytes)
{
    u_short *ptr = parg;
    register long        sum;        
    u_short            oddbyte;
    register u_short    answer;        

    sum = 0;
    while (nbytes > 1)  {
        sum += *ptr++;
        nbytes -= 2;
    }

    if (nbytes == 1) {
        oddbyte = 0;        
        *((u_char *) &oddbyte) = *(u_char *)ptr;   
        sum += oddbyte;
    }

    sum  = (sum >> 16) + (sum & 0xffff);    
    sum += (sum >> 16);            
    answer = ~sum;        
    return(answer);
}

#define MODX                 4102   

u_int16_t
fletcher_checksum(u_char * buffer, const size_t len, const uint16_t offset)
{
  u_int8_t *p;
  int x, y, c0, c1;
  u_int16_t checksum;
  u_int16_t *csum;
  size_t partial_len, i, left = len;

  checksum = 0;

  assert (offset < len);

  csum = (u_int16_t *) (buffer + offset);
  *(csum) = 0;

  p = buffer;
  c0 = 0;
  c1 = 0;

  while (left != 0)
    {
      partial_len = MIN(left, MODX);

      for (i = 0; i < partial_len; i++)
    {
      c0 = c0 + *(p++);
      c1 += c0;
    }

      c0 = c0 % 255;
      c1 = c1 % 255;

      left -= partial_len;
    }

  x = (int)((len - offset - 1) * c0 - c1) % 255;

  if (x <= 0)
    x += 255;
  y = 510 - c0 - x;
  if (y > 255)
    y -= 255;

  buffer[offset] = x;
  buffer[offset + 1] = y;

  checksum = htons((x << 8) | (y & 0xFF));

  return checksum;
}
