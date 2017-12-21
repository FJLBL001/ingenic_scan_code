#ifndef _SCANPWM_H
#define _SCANPWM_H

#ifdef __cplusplus
extern "C" {
#endif



int enable_pwm(char mode);


/***
min :200

max :100000000

*****/

int set_pwm_period(char *period,int period_len);


/**

    DEV_OFF = 0,
    DEV_HALF = 127,
    DEV_FULL = 255,

**/
int set_pwm_duty_ratio(char *duty_ratio,int duty_ratio_len);



#ifdef __cplusplus
}
#endif

#endif