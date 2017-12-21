/*!
  ******************************************************************************
  *
  *   @file     sy_usbhid.c.c
  *   @brief
  *   @author   Jaly
  *   @date     2015/11/26
  *   @version  V1.0
  *   @details
  *   @see
  *   @bug
  *   @attention
  *   @warning
  *
  ******************************************************************************
  */
#ifndef __SY_USBHID_C_
  #define __SY_USBHID_C_

/* Includes ------------------------------------------------------------------*/
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

/* Private typedef -----------------------------------------------------------*/
struct options {
	const char    opt;
	unsigned char val;
};

typedef enum
{
  ECHO_OK = 0,     /*!< work normally */
  ECHO_NOK,    /*!< something wrong with it  */
  ECHO_BUSY,   /*!< need to be called in other time  */
  ECHO_CRASH,
  ECHO_IDLE,
}echo_t;  /* end of enum define */

#define SIZE(X)                   (sizeof(X) / sizeof(*(X)))

/* Private define ------------------------------------------------------------*/
#define BUF_LEN         8
#define OPT_END         0x00
#define JALY_DEBUG      0

#define DEV_HID         "/dev/hidg0"
#define HID_A_BASE      0x04
#define HID_1_BASE      0x1E


#define CODE_SIZE       100
/* Private variables ---------------------------------------------------------*/

static struct options kval[] = {
//	{.opt = "--return",	.val = 0x28},
//	{.opt = "--esc",	.val = 0x29},
//	{.opt = "--bckspc",	.val = 0x2a},
//	{.opt = "--tab",	.val = 0x2b},
//	{.opt = "--spacebar",	.val = 0x2c},
//	{.opt = "--caps-lock",	.val = 0x39},
//	{.opt = "--f1",		.val = 0x3a},
//	{.opt = "--f2",		.val = 0x3b},
//	{.opt = "--f3",		.val = 0x3c},
//	{.opt = "--f4",		.val = 0x3d},
//	{.opt = "--f5",		.val = 0x3e},
//	{.opt = "--f6",		.val = 0x3f},
//	{.opt = "--f7",		.val = 0x40},
//	{.opt = "--f8",		.val = 0x41},
//	{.opt = "--f9",		.val = 0x42},
//	{.opt = "--f10",	.val = 0x43},
//	{.opt = "--f11",	.val = 0x44},
//	{.opt = "--f12",	.val = 0x45},
//	{.opt = "--insert",	.val = 0x49},
//	{.opt = "--home",	.val = 0x4a},
//	{.opt = "--pageup",	.val = 0x4b},
//	{.opt = "--del",	.val = 0x4c},
//	{.opt = "--end",	.val = 0x4d},
//	{.opt = "--pagedown",	.val = 0x4e},
//	{.opt = "--right",	.val = 0x4f},
//	{.opt = "--left",	.val = 0x50},
//	{.opt = "--down",	.val = 0x51},
//	{.opt = "--enter",	.val = 0x58},
//	{.opt = "--up",		.val = 0x52},
//	{.opt = "--num-lock",	.val = 0x53},
  {.opt = '\n',         .val = 0x28},
  {.opt = ' ',          .val = 0x2C},

  {.opt = '-',          .val = 0x2D},
  {.opt = '=',          .val = 0x2E},
  {.opt = '[',          .val = 0x2F},
  {.opt = ']',          .val = 0x30},
  {.opt = '\\',         .val = 0x31},

  {.opt = ';',          .val = 0x33},
  {.opt = '\'',         .val = 0x34},
  {.opt = '\?',         .val = 0x35},
  {.opt = ',',          .val = 0x36},
  {.opt = '.',          .val = 0x37},
  {.opt = '/',          .val = 0x38},

	{.opt = OPT_END,      .val = 0x00},
};


/* add shift author */
static struct options skval[] =
{
  {.opt = '!',          .val = 0x1E},
  {.opt = '@',          .val = 0x1F},
  {.opt = '#',          .val = 0x20},
  {.opt = '$',          .val = 0x21},
  {.opt = '%',          .val = 0x22},
  {.opt = '^',          .val = 0x23},
  {.opt = '&',          .val = 0x24},
  {.opt = '*',          .val = 0x25},
  {.opt = '(',          .val = 0x26},
  {.opt = ')',          .val = 0x27},

  {.opt = '_',          .val = 0x2D},
  {.opt = '+',          .val = 0x2E},
  {.opt = '{',          .val = 0x2F},
  {.opt = '}',          .val = 0x30},
  {.opt = '|',          .val = 0x31},

  {.opt = ':',          .val = 0x33},
  {.opt = '"',          .val = 0x34},

  {.opt = '~',          .val = 0x35},

  {.opt = '<',          .val = 0x36},
  {.opt = '>',          .val = 0x37},
  {.opt = '?',          .val = 0x38},

  {.opt = OPT_END,      .val = 0x00},
};

/* Private functions ---------------------------------------------------------*/
/* 0x00 + 0x00 + 6 key */
#define KEY_NUM_MAX   1
static int keyboard_fill_report(char report[8], char buf[BUF_LEN])
{
	int key = 0;
	int i = 0;
  char dat = buf[0];

  memset(report, 0x0, SIZE(report));

#if JALY_DEBUG
  printf("process  %c \n", dat);
#endif

	while(key < KEY_NUM_MAX)
  {
    /* normal function button */
    if(key < KEY_NUM_MAX)
    {
      if(islower(dat))
      {
#if JALY_DEBUG
        printf("%c is lower\n", dat);
#endif
        report[2 + key] = (dat + HID_A_BASE - 'a');
        key++;
        continue;
      }
      else if(isupper(dat))
      {
#if JALY_DEBUG
        printf("%c is upper\n", dat);
#endif
        report[0] = report[0] | 0x02;                 /* shift */
        report[2 + key] = (dat + HID_A_BASE - 'A');
        key++;
        continue ;
      }
      else
      {
        if(isdigit(dat))
        {
#if JALY_DEBUG
          printf("%c is num\n", dat);
#endif
          dat = (dat - '0' + 9) % 10;
          report[2 + key] = (dat + HID_1_BASE);
          key++;
          continue;
        }
        else    /* special sybol */
        {
          for (i = 0; kval[i].opt != OPT_END; i++)  /* don't need to use shift */
          {
            if(dat == kval[i].opt)
            {
#if JALY_DEBUG
              printf("key is \\n %c\n", kval[i].opt);
#endif
              report[2 + key] = kval[i].val;
              key++;
              break;
            }
          }
          if(kval[i].opt != OPT_END)
            continue;

          for (i = 0; skval[i].opt != OPT_END; i++) /* don't need to use shift */
          {
            if(dat == skval[i].opt)
            {
#if JALY_DEBUG
              printf("key is %c\n", skval[i].opt);
#endif
              report[0] = report[0] | 0x02;   /* shift */
              report[2 + key] = skval[i].val;
              key++;
              break;
            }
          }
          if(skval[i].opt != OPT_END)
            continue;

          /* never comes here cause no such char from keyboard */
#if JALY_DEBUG
          fprintf(stderr, "special symbol (%c) = 0x%x = %d \n", dat, dat, dat);
#endif
          break;
        }
      }
    }
	}

  return 8;
}
/***--------------------------------------------------------------------------*/
/*!
 *  @fn	   	  sy_usbhid_printf
 *  @brief
 *  @param    None
 *  @retval   None
 *  @author   Jaly
 *  @date     2015/11/20
 *  @note     wait for test!
 */
echo_t sy_usbhid_printf(char *dat, int flag)
{
  /*!<      */
  int fd = 0;
	char buf[BUF_LEN];
	char report[8];
	int to_send = 8;
	fd_set rfds;
	int retval, i;
  int size = 0;
  char code[CODE_SIZE];


	if((fd = open(DEV_HID, O_RDWR, 0666)) == -1)
  {
		perror(DEV_HID);
		return 1;
	}

  size = strlen(dat);

  memcpy(code, dat, size);      /* copy for protect */

#if JALY_DEBUG
  printf("usb hid str is %s, size = %d\n", code, size);
#endif

  switch(flag)
  {
    case 1:   /* new line */
    {
      if(code[size - 1] != '\n')
      {
#if JALY_DEBUG
        printf("hid add newline\n");
#endif
        code[size] = '\n';
        size++;
        code[size] = '\0';
      }
      break;
    }
    default:
    {
      break;
    }
  }

#if JALY_DEBUG
  if(code[size - 1] != '\n')
  {
    printf("-----------------final dat is set fail\n");
    printf("usb hid str size = %d  %s", size, code);
  }
#endif

	while(1)
  {
		FD_ZERO(&rfds);
		FD_SET(STDIN_FILENO, &rfds);
		FD_SET(fd, &rfds);

		retval = select(fd + 1, &rfds, NULL, NULL, NULL);
		if(retval == -1 && errno == EINTR)
			continue;

		if(retval < 0)
    {
			perror("select()");
			return 2;
		}

    /* Jaly: print string */
    for(i = 0; i < size; i++)
    {
      buf[0] = code[i];
      memset(report, 0x0, sizeof(report));
#if JALY_DEBUG
      if(size == i + 1)
      {
        if(code[i] != '\n')
        {
          printf("final dat is %d char %c idx = %d-------------\n", code[i], code[i], i);
        }
      }
#endif
      to_send = keyboard_fill_report(report, buf);

      if(write(fd, report, to_send) != to_send)
      {
        perror(DEV_HID);
        return 3;
      }

      /* cancel repeat */
      memset(report, 0x0, sizeof(report));
      if(write(fd, report, to_send) != to_send)
      {
        perror(DEV_HID);
        return 4;
      }
    }

    break;
  }

	close(fd);

  return ECHO_OK;
}/* end of sy_usbhid_printf */

/***--------------------------------------------------------------------------*/
/*!
 *  @fn	   	  sy_usbhid_init
 *  @brief
 *  @param    None
 *  @retval   None
 *  @author   Jaly
 *  @date     2015/11/27
 *  @note     wait for test!
 */
echo_t sy_usbhid_init(int *fd)
{
  /*!<      */
  system("echo connect > /sys/devices/platform/jz-dwc2/dwc2/udc/dwc2/soft_connect");

  return ECHO_OK;
}/* end of sy_usbhid_init */


/***--------------------------------------------------------------------------*/
/*!
 *  @fn	   	  sy_usbhid_deinit
 *  @brief
 *  @param    None
 *  @retval   None
 *  @author   Jaly
 *  @date     2015/11/27
 *  @note     wait for test!
 */
echo_t sy_usbhid_deinit(int *fd)
{
  /*!<      */
  system("echo disconnect > /sys/devices/platform/jz-dwc2/dwc2/udc/dwc2/soft_connect");

  return ECHO_OK;
}/* end of sy_usbhid_deinit */

#undef JALY_DEBUG


/***--------------------------------------------------------------------------*/
/*!
 *  @fn	   	  sy_usbhid_test
 *  @brief
 *  @param    None
 *  @retval   ECHO_OK,  everything works well
 *            ECHO_NOK, some problem accur
 *            ECHO_CRASH, death bug, do something helpful to restore it.
 *            ECHO_BUSY,  please retry, if bus busy or high priority
 *            others, None
 *  @author   Jaly
 *  @date     2016/1/14
 *  @note     wait for test!
 */
echo_t sy_usbhid_test(void)
{
  sy_usbhid_printf("abcdefghijklmnopqrstuvwxyz\n", 0);
  sy_usbhid_printf("ABCDEFGHIJKLMNOPQRSTUVWXYZ\n", 0);
  sy_usbhid_printf("PanZuoBin Love HanXiaoJuan\n", 0);
  sy_usbhid_printf("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz\n", 0);
  sy_usbhid_printf("Jaly love Iris\n", 0);
  sy_usbhid_printf("1234567890-= ~!@#$%^&*()_+,./;'[]<>?:\"{}\n", 0);
  sy_usbhid_printf("ABCabc AaBbCc cAbCBa CaBbAc\n", 0);

  return ECHO_OK;
}/* end of sy_usbhid_test */

/***--------------------------------------------------------------------------*/
/*!
 *  @fn	   	  main
 *  @brief
 *  @param    None
 *  @retval   None
 *  @author   Jaly
 *  @date     2016/4/12
 *  @note     wait for test!
 */
int main(int argc, char *argv[])
{
  /*!< following my step */
  int hid_fd;
  int cnt;

  sy_usbhid_init(&hid_fd);    /* init   */

  printf("hid begin work*******************\n");
  for(cnt = 0; cnt < 10; cnt++)
  {
	sy_usbhid_test();           /* just for test */
	sleep(1);
  }
  printf("hid stop work*******************\n");
  sy_usbhid_deinit(&hid_fd);  /* deinit */

  return ECHO_OK;
}/* end of main */





/* File end ------------------------------------------------------------------*/
#endif /* end undef __SY_USBHID_C_ */



