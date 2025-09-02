#include "i2c.h"
#include "utils.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "CH347DLL.H"
#define MODE_AUTO	0
#define MODE_QUICK	1
#define MODE_READ	2
#define MODE_FUNC	3
static ULONG g_uIndex = 0;

static int i2c_detect()
{
	int i, j;
	int cmd, res;
  uint8_t in[16];
  uint8_t out[16];
  BOOL ret = FALSE;
  ULONG retAck = 0;
	printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\n");

	for (i = 0; i < 128; i += 16) {
		printf("%02x: ", i);
		for(j = 0; j < 16; j++) {
			fflush(stdout);
			if ((i+j >= 0x30 && i+j <= 0x37)
				|| (i+j >= 0x50 && i+j <= 0x5F))
				cmd = MODE_READ;
			else
				cmd = MODE_QUICK;
      
      /* Skip unwanted addresses */
			if (i+j < i2c_addr_start || i+j > i2c_addr_stop) {
				printf("   ");
				continue;
			}
      res = 0;
      retAck = 0;
      ret = FALSE;
      out[0] = (i+j) << 1;
      out[1] = 0x00;
			/* Probe this address */
			switch (cmd) {
			case MODE_QUICK: /* MODE_QUICK */
        ret = CH347StreamI2C(g_uIndex, 2, &out, 0, NULL); 
        if (ret == TRUE) {
          res = 1; 
        }
				break;
			case MODE_READ:
        ret = CH347StreamI2C(g_uIndex, 2, &out, 1, &in);
        if (ret == TRUE) {
          res = 1;
        }
				break;
			}

			if (res == 0)
				printf("-- ");
			else
				printf("%02x ", i+j);
		}
		printf("\n");
	}

	return 0;
}

const char *help = "Usage: i2cdetect I2CBUS\n i2cdetect -l\n I2CBUS is an "
                   "integer or an I2C bus name\n";

int main(int argc, char *argv[])
{
  if (argc != 1)
  {
    g_uIndex = atoi(argv[1]);
  }
  printf("current using CH347 index : %ld\n", g_uIndex);

  if (!CH347OpenDevice(g_uIndex))
  {
    printf("Can't open device.");
    return 1;
  }

  // 00=低速/20KHz,01=标准/100KHz(默认值),10=快速/400KHz,11=高速/750KHz
  if (!CH347I2C_Set(g_uIndex, 1))
  {
    return 1;
  }
  i2c_detect();
  CH347CloseDevice(g_uIndex);
  return 0;
}
