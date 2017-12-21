#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include <i2c.h>
#include <i2c-dev.h>
#include <I2cAdapter.h>
#include <iostream>
#include <sstream>

using namespace std;

class I2cAdapter;
class E2promDevice{
	public:
		E2promDevice(unsigned char device_node,char set_slave_addr);
		virtual ~E2promDevice();
		int E2promRead(int buff_size);
	private:
		I2cAdapter *m_e2promdevice;
		char m_slave_addr;
};


E2promDevice::E2promDevice(unsigned char device_node,char set_slave_addr)
{
	m_e2promdevice = new I2cAdapter(device_node);
	m_slave_addr=m_e2promdevice->I2cSetSlave(set_slave_addr);
}

E2promDevice::~E2promDevice()
{}

int E2promDevice::E2promRead(int buff_size)
{
	unsigned char buff[buff_size];
	signed int ret = m_e2promdevice->I2cMasterRecv(buff,buff_size);
}

timespec diff(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0)
	{
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	}
	else
	{
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

void help_msg( char * name)
{
	printf("*****************************************************************\n");
	printf("*usage :\n");
	printf("*        %s -n device_node -a slave_addr -b buff_size\n",name);
	printf("* eg:\n");
	printf("*    Halley2 : Read 16kbit data form eeprom:\n");
	printf("*        %s -n 1 -a 0x50 -b 2048\n",name);
	printf("*****************************************************************\n");
}

main(int argc,char **argv)
{
	unsigned int device_node;
	unsigned int set_slave_addr;
	unsigned int buff_size;
	char opt;

	if(argc == 1)
	{
		help_msg(argv[0]);
		return 0;
	}
	while ((opt =getopt(argc,argv,"n:a:b:")) != -1)
	{
		switch(opt)
		{
			case 'n':
				device_node = strtoul(optarg,NULL,16);
				printf("device_node is %d \n",device_node);
				break;
			case 'a':
				set_slave_addr = strtoul(optarg,NULL,16);
				printf("addr is 0x%02x \n",set_slave_addr);
				break;
			case 'b':
				buff_size = strtoul(optarg,NULL,10);
				printf("buff size is %d \n",buff_size);
				break;
			case '?':
			case 'h':
				help_msg(argv[0]);
				return -1;
			default:
				printf("%c not support ! \n",opt);
		}
	}

	timespec start, finish,alltime;
	E2promDevice e2prom(device_node,set_slave_addr);

	clock_gettime(CLOCK_REALTIME, &start);
	e2prom.E2promRead(buff_size);
	clock_gettime(CLOCK_REALTIME, &finish);
	cout<<"start:"<<start.tv_nsec<<endl;
	cout<<"end :" <<finish.tv_nsec<<endl;
	cout<<"diff(start,finish).tv_sec<<:"<<diff(start,finish).tv_sec<<endl;
	cout<<"diff(start,finish).tv_nsec<<:"<<diff(start,finish).tv_nsec<<endl;


}

