#ifndef _SCANSPM_H
#define _SCANSPM_H

#ifdef __cplusplus
extern "C" {
#endif


extern int open_key_button_event();

extern int key_button_state(int key);

extern int read_button_state(int key);

extern void close_key_button_event();



#ifdef __cplusplus
}
#endif

#endif
