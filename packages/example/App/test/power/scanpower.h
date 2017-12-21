#ifndef _SCANPOWER_H
#define _SCANPOWER_H


#ifdef __cplusplus
extern "C" {
#endif

/***
state : freeze standby mem

***/

extern int set_power_state(char *power_state,int state_len);


#ifdef __cplusplus
}
#endif

#endif
