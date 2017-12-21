#ifndef _SCANPARAM_H
#define _SCANPARAM_H


#include <zxing/common/Counted.h>
#include <zxing/Binarizer.h>
#include <zxing/MultiFormatReader.h>
#include <zxing/ReaderException.h>
#include <zxing/common/GlobalHistogramBinarizer.h>
#include <zxing/common/HybridBinarizer.h>
#include <zxing/common/GreyscaleLuminanceSource.h>

#include <zxing/qrcode/QRCodeReader.h>
#include <zxing/multi/qrcode/QRCodeMultiReader.h>
#include <zxing/multi/ByQuadrantReader.h>
#include <zxing/multi/GenericMultipleBarcodeReader.h>
#include <zxing/common/StringUtils.h>

using namespace zxing;



#ifdef __cplusplus
extern "C" {
#endif


#define YES '1'
#define NO  '0'



#define PARAM_FILE_NAME     "scan_param.ini"




#define DEFAULT_SPEED       9600
#define DEFAULT_FLOW_CTRL   0
#define DEFAULT_DATABITS    8
#define DEFAULT_STOPBITS    1
#define DEFAULT_PARITY      'N'


#define DEFAULT_IS_BEEP     YES
#define DEFAULT_IS_LED      YES
#define DEFAULT_IS_ILLUMINATION     YES
#define DEFAULT_SUPPORT_CODE   (DecodeHints::DEFAULT_HINT) 
#define DEFAULT_TIME_OUT   20000





#define COMPANY_NAME    "NLS"
#define CMD_CODE_LEN    7



/*<uart param */
#define SPEED_1200      100000
#define SPEED_2400      100010
#define SPEED_4800      100020
#define SPEED_9600      100030
#define SPEED_14400     100040
#define SPEED_19200     100050
#define SPEED_38400     100060
#define SPEED_56700     100070
#define SPEED_115200    100080

#define PARITY_N        101000  
#define PARITY_E        101010
#define PARITY_O        101020

#define STOPBITS_1      102000
#define STOPBITS_2      102010

#define DATABITS_5      103000
#define DATABITS_6      103010
#define DATABITS_7      103020
#define DATABITS_8      103030

/*<comm param*/

#define IS_BEEP_YES     203010 
#define IS_BEEP_NO      203000


#define IS_LED_YES      206011
#define IS_LED_NO       206010

#define IS_ILLUMINATION_YES    200010
#define IS_ILLUMINATION_NO     200020 


#define TIME_OUT        313000       


/*< support code */
#define SUPPORT_UPC_A_YES       404020 
#define SUPPORT_UPC_A_NO        404010

#define SUPPORT_UPC_E_YES       403020 
#define SUPPORT_UPC_E_NO        403010

#define SUPPORT_EAN_13_YES      402020  
#define SUPPORT_EAN_13_NO       402010

#define SUPPORT_EAN_8_YES       401020 
#define SUPPORT_EAN_8_NO        401010

#define SUPPORT_CODE_39_YES     408020 
#define SUPPORT_CODE_39_NO      408010

#define SUPPORT_CODE_93_YES     410020 
#define SUPPORT_CODE_93_NO      410010 

#define SUPPORT_CODE_128_YES    400020 
#define SUPPORT_CODE_128_NO     400010

#define SUPPORT_ITF_YES         405020  
#define SUPPORT_ITF_NO          405010 

#define SUPPORT_CODABAR_YES     409020 
#define SUPPORT_CODABAR_NO      409010

#define SUPPORT_QR_CODE_YES     502020
#define SUPPORT_QR_CODE_NO      502010 

#define SUPPORT_DATA_MATRIX_YES     504020
#define SUPPORT_DATA_MATRIX_NO      504010 

#define SUPPORT_AZTEC_YES       503020 
#define SUPPORT_AZTEC_NO        503010

#define SUPPORT_PDF_417_YES     501020 
#define SUPPORT_PDF_417_NO      501010  




#pragma pack (1)
struct  serial_param
{
    int speed;
    int flow_ctrl; 
    int databits; 
    int stopbits; 
    int parity;
};

struct comm_param
{
    int is_beep; //交易成功是否响应蜂鸣
    int is_led;  //交易成功是否响应led
    int is_illumination;     //是否开启照明灯
    DecodeHints support_code;     //支持条码
    int time_out;



};


struct scan_param
{
    struct serial_param _serial_param;
    
    struct comm_param   _comm_param;
    
};












#pragma pack()


extern int exist_param_file(char *file_name);
extern int creat_param_file(char *file_name);
extern int open_param_file(char *file_name);
extern int close_param_file_fd(int fd);
extern int read_param_file(int fd,void *buf ,int buf_size);
extern int write_param_file(int fd,void *buf ,int buf_size);
extern int destory_param_file(char *file_name );

extern int init_scan_param(void);
extern int read_scan_param(struct scan_param *p_scan_param,int buf_size);
extern int write_scan_param(struct scan_param *p_scan_param,int buf_size);
extern int deal_cmd_code(char *cmd_code,char *param);

#ifdef __cplusplus
}
#endif


#endif
