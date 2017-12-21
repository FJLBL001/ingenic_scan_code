#include <iostream>
#include <gpio_device.h>

/*
 * GPIO_PA(n)  (0*32 + n)
 * GPIO_PB(n)  (1*32 + n)
 * GPIO_PC(n)  (2*32 + n)
 * GPIO_PD(n)  (3*32 + n)
 * GPIO_PE(n)  (4*32 + n)
 * GPIO_PF(n)  (5*32 + n)
*/

int test_gpio_output(int gpio_num)
{
	GPIO_device phoenix_out(gpio_num);
	phoenix_out.export_gpio();
	phoenix_out.write_gpio("direction", "out"); //set gpio output;

	while(1){
		phoenix_out.write_gpio("value", "0");
		printf("-------gpio output 0\n");
		sleep(10);
		phoenix_out.write_gpio("value", "1");
		printf("-------gpio output 1\n");
		sleep(10);
	}
	return 0;
}

int test_gpio_poll(int gpio_num){
	GPIO_device phoenix_in(gpio_num);
	phoenix_in.export_gpio();

	/* gpio input ; or "low" for output 0; or "high" for output 1*/
	phoenix_in.write_gpio("direction", "high");

	/* support "both" or "none", "rising", "falling" edge trigger */
	phoenix_in.write_gpio("edge", "falling");

	while(1){
		if(phoenix_in.poll_gpio()){
			printf("PIN %d pushed!\n",&gpio_num);
		}
	}
	return 0;
}

int main(){
	printf("-----gpio Sample test ----\n");
//	test_gpio_poll(16); //PA16
	test_gpio_output(33); //PB01
}


