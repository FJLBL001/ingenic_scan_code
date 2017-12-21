#ifndef _SCANGPIO_H
#define _SCANGPIO_H

#ifdef __cplusplus
extern "C" {
#endif



extern int create_gpio_node(int node);


extern int destroy_gpio_node(int node);


extern int set_gpio_output_high_low(int node,const char *high_low);


#ifdef __cplusplus
}
#endif

#endif
