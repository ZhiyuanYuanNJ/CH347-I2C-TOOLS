#include <windows.h>
#include "CH347DLL.H"
#include "utils.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#define BYTE_MODE 0
#define WORD_MODE 1
#define BLOCK_MODE 2
static ULONG g_uIndex = 0;

const char * help = "Usage: i2cset device_index address reg_addr -b/-w/-i -r data0 data1 ... \nExample:  0 0x50 0x10 -b -r 0x77\n";

void i2c_write_reg(int addr, uint8_t reg, uint8_t* data, int count, BOOL bRead)
{
	uint8_t out[128+7] = {0};
	uint8_t in[128+7] = {0};
	out[0] = addr << 1;
	out[1] = reg;
	memcpy(&out[2], data, count);
	BOOL ret = CH347StreamI2C(g_uIndex, count+2, out, 0, NULL);
	if (!ret) {
		printf("CH347StreamI2C write failed\n");
		return;
	}
	if (bRead) {
		Sleep(50);
		ret = CH347StreamI2C(g_uIndex, 2, out, count, in);
		if (!ret) {
			printf("CH347StreamI2C read failed\n");
		}
		if (memcmp(&out[2], &in[0], count) != 0) {
			printf("Verification error.\n");
		} else {
			printf("Verification successfully.\n");
		}
	}
}

// i2cset 0 0x48 0x02 0x5000 w
int main(int argc, char *argv[])
{
  int addr = 0x50;
  int reg = 0;
  int mode = BYTE_MODE;
  BOOL read_back = FALSE;
  uint8_t data[128] = {0};
  uint16_t word_tmp = 0;
  int index = 0;
  if (argc >= 4) {
  	g_uIndex = atoi(argv[1]);
	sscanf(argv[2], "%x", &addr);
	sscanf(argv[3], "%x", &reg);
  } else {
	printf("%s", help);
	return 1;
  }
  for (int i = 4; i < argc; i++)
  {
	if (argv[i][0] == '-') {
		if (argv[i][1] == 'r') {
		  read_back = TRUE;
		} else if (argv[i][1] == 'b') {
		  mode = BYTE_MODE;
		} else if (argv[i][1] == 'w') {
		  mode = WORD_MODE;
		} else if (argv[i][1] == 'i') {
		  mode = BLOCK_MODE;
		} else {
		  printf("%s", help);
		  return 1;
		}
		continue;
	}
	switch (mode)
	{
	case BYTE_MODE:
	case BLOCK_MODE:
	  sscanf(argv[i], "%x", &data[index]);
	  index++;
	break;
	case WORD_MODE:
	  sscanf(argv[i], "%hx", &word_tmp);
	  data[index] = (word_tmp >> 8) & 0xFF; 
      data[index+1] = word_tmp & 0xFF;
	  index+=2;
	break;
	default:
		printf("choose write mode first.\n");
		return 1;
		break;
	}
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
  i2c_write_reg(addr, reg, data, index, read_back);
}