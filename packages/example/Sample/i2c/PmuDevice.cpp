#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "i2c.h"
#include "I2cAdapter.h"
#include "i2c-dev.h"

enum COMMAND{
	READ_BYTE,
	READ_TWO_BYTE,
	WRITE_BYTE,
	WRITE_TWO_BYTE
};

class I2cAdapter;
class PmuDevice{
	public:
		PmuDevice(unsigned char ,char );
		virtual ~PmuDevice();
		int PmuSmbReadByte( char reg_addr);
		int PmuSmbWriteByte(char reg_addr,int value);
		int PmuSmbReadTwoByte(unsigned char reg_addr);
		int PmuSmbWriteTwoByte(unsigned char reg_addr, int value_high, int value_low);
	private:
		I2cAdapter * m_pmudevice;
		char m_slave_addr;
};

PmuDevice::PmuDevice(unsigned char device_node, char set_slave_addr)
{
	m_pmudevice = new  I2cAdapter(device_node);
	m_slave_addr = m_pmudevice->I2cSetSlave(set_slave_addr);
}

PmuDevice::~PmuDevice(){
}

int PmuDevice:: PmuSmbReadByte(char reg_addr)
{
	unsigned char msgbuf0[1];
	unsigned char msgbuf1[1];
	unsigned char cmd=(unsigned char)reg_addr;

	struct i2c_msg msg[2];

	msg[0].addr = m_slave_addr;
	msg[0].flags = 0;
	msg[0].len = 1;
	msg[0].buf = &cmd;

	msg[1].addr = m_slave_addr;
	msg[1].flags = I2C_M_RD;
	msg[1].len = 1;
	msg[1].buf = msgbuf1;

	m_pmudevice->I2cTransfer(msg, 2);
	printf("The value from 0x%02x is:  0x%02x \n", reg_addr,msgbuf1[0]);
}

int PmuDevice:: PmuSmbWriteByte( char reg_addr, int value)
{
	unsigned char msgbuf0[1];
	unsigned char msgbuf1[1];
	unsigned char cmd=(unsigned char)reg_addr;

	struct i2c_msg msg[2];

	msgbuf1[0] = value;
	printf("write to msgbuf1[0] is :  0x%02x \n",msgbuf1[0]);

	msg[0].addr = m_slave_addr;
	msg[0].flags = 0;
	msg[0].len = 1;
	msg[0].buf = &cmd;

	msg[1].addr = m_slave_addr;
	msg[1].flags = !I2C_M_RD;
	msg[1].len = 1;
	msg[1].buf = msgbuf1;

	m_pmudevice->I2cTransfer(msg, 2);
}

int PmuDevice:: PmuSmbReadTwoByte(unsigned char reg_addr)
{
	unsigned char msgbuf0[1];
	unsigned char msgbuf1[2];
	unsigned char cmd=reg_addr;

	struct i2c_msg msg[2];

	msg[0].addr = m_slave_addr;
	msg[0].flags = 0;
	msg[0].len = 1;
	msg[0].buf = &cmd;

	msg[1].addr = m_slave_addr;
	msg[1].flags = I2C_M_RD;
	msg[1].len = 2;
	msg[1].buf = msgbuf1;

	m_pmudevice->I2cTransfer(msg, 2);

	printf("The value from 0x%02x is:  0x %02x %02x \n", reg_addr,msgbuf1[0],msgbuf1[1]);

}

int PmuDevice:: PmuSmbWriteTwoByte(unsigned char reg_addr, int value_high, int value_low)
{

	unsigned char msgbuf0[1];
	unsigned char msgbuf1[1];
	unsigned char cmd=reg_addr;

	struct i2c_msg msg[3];

	msgbuf1[0] = value_high;
	msgbuf1[1] = value_low;

	msg[0].addr = m_slave_addr;
	msg[0].flags = 0;
	msg[0].len = 1;
	msg[0].buf = &cmd;

	msg[1].addr = m_slave_addr;
	msg[1].flags = !I2C_M_RD;
	msg[1].len = 1;
	msg[1].buf = &msgbuf1[0];

	msg[2].addr = m_slave_addr;
	msg[2].flags = !I2C_M_RD;
	msg[2].len = 1;
	msg[2].buf = &msgbuf1[1];

	m_pmudevice->I2cTransfer(msg, 3);
	printf("write in register 0x%02x : high 8bit is : 0x%02x , low 8bit is : 0x%02x \n", reg_addr, msgbuf1[0],msgbuf1[1]);
}


void help_msg( char * name)
{
	printf("*******************************************************\n");
	printf("*usage :\n");
	printf("*    Read 1 Byte data:\n");
	printf("*        %s -n device_node -s slave_addr -r reg_addr\n",name);
	printf("*    Write 1 Byte data:\n");
	printf("*        %s -n device_node -s slave_addr -w reg_addr -v 8bit-value\n",name);
	printf("*    Read 2 Byte data:\n");
	printf("*        %s -n device_node -s slave_addr -R reg_addr\n",name);
	printf("*    Write 2 Byte data:\n");
	printf("*        %s -n device_node -s slave_addr -W reg_addr -h high-8bit -l low-8bit\n",name);
	printf("* eg:\n");
	printf("*        %s -n 1 -s 0x32 -r 0x2c\n",name);
	printf("*        %s -n 0 -s 0x47 -w 0x48 -v 0x01\n",name);
	printf("*        %s -n 0 -s 0x71 -R 0x0d\n",name);
	printf("*        %s -n 0 -s 0x71 -W 0x0d -h 0x3c -l 0x8a\n",name);
	printf("*******************************************************\n");
}

int main(int argc, char* argv[])
{
	unsigned int device_node;
	unsigned int set_slave_addr;
	unsigned int reg_addr;
	unsigned int value, value_high ,value_low;
	char opt ,un;
	enum COMMAND command;

	if(argc == 1 )
	{
		help_msg(argv[0]);
		return 0;
	}

	while ((opt = getopt(argc, argv,"c:n:s:r:w:R:W:v:h:l:")) != -1)
	{
		switch(opt)
		{
			case 'n':
				device_node = strtoul(optarg,NULL,16);
				break;
			case 's':
				set_slave_addr = strtoul(optarg,NULL,16);
				break;
			case 'r':
				reg_addr = strtoul(optarg,NULL,16);
				command = READ_BYTE;
				break;
			case 'R':
				reg_addr = strtoul(optarg,NULL,16);
				command = READ_TWO_BYTE;
				break;
			case 'w':
				reg_addr = strtoul(optarg,NULL,16);
				command = WRITE_BYTE;
				break;
			case 'W':
				reg_addr = strtoul(optarg,NULL,16);
				command = WRITE_TWO_BYTE;
				break;
			case 'v':
				value = strtoul(optarg,NULL,16);
				break;
			case 'h':
				value_high = strtoul(optarg,NULL,16);
				break;
			case 'l':
				value_low = strtoul(optarg,NULL,16);
				break;
			case '?':
				help_msg(argv[0]);
				return -1;
			default:
				printf("%c not support! \n",opt);
		}
	}

	PmuDevice Pmu(device_node,set_slave_addr);
	switch(command)
	{
		case READ_BYTE:
			printf ("-------------read-------------\n");
			Pmu.PmuSmbReadByte(reg_addr);
			break;
		case READ_TWO_BYTE:
			printf ("-------------read two byte-------------\n");
			Pmu.PmuSmbReadTwoByte(reg_addr);
			break;
		case WRITE_BYTE:
			printf ("-------------write-------------\n");
			Pmu.PmuSmbWriteByte(reg_addr,value);
			sleep(3);
			printf("\n");
			printf ("-------------read-------------\n");
			Pmu.PmuSmbReadByte(reg_addr);
			break;
		case WRITE_TWO_BYTE:
			printf ("-------------write two byte-------------\n");
			Pmu.PmuSmbWriteTwoByte(reg_addr,value_high,value_low);
			sleep(3);
			printf("\n");
			printf ("-------------read two byte-------------\n");
			Pmu.PmuSmbReadTwoByte(reg_addr);
			break;
	}
}
