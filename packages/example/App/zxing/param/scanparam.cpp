
#include <stdio.h>    
#include <stdlib.h>     
#include <unistd.h>     
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>      
#include <termios.h>   
#include <errno.h>      
#include <string.h>
#include "scanparam.h"




/** 
 *  @brief exist param file   
 *  @param file_name    
 *  @return  == 0 exist
 *               fail 
 */  
int exist_param_file(char *file_name)
{
    return access(file_name,F_OK);
}





/** 
 *  @brief creat param file   
 *  @param file_name    
 *  @return  > 0 success
 *           < 0 fail 
 */  
int creat_param_file(char *file_name)
{

    int fd = -1;

    fd = open(file_name,O_RDWR|O_CREAT|O_EXCL);

    if(fd > 0)
    {
        return fd;
    }
    else
    {
        return -1;
    }
}

/** 
 *  @brief open param file   
 *  @param file_name    
 *  @return  > 0 success
 *           < 0 fail 
 */  
int open_param_file(char *file_name)
{

    int fd = -1;

    fd = open(file_name,O_RDWR);

    if(fd > 0)
    {
        return fd;
    }
    else
    {
        return -1;
    }
}


/** 
 *  @brief close param file fd    
 *  @param fd    
 *  @return 0 success
 *          -1 fail
 */  
int close_param_file_fd(int fd)
{

    return close(fd);
}


/** 
 *  @brief read param file   
 *  @param fd    
 *  @param buf
 *  @param buf_size
 *  @return  > 0 success
 *           < 0 fail 
 */  
int read_param_file(int fd,void *buf ,int buf_size)
{
    return read(fd,buf,buf_size);
}

/** 
 *  @brief write param file   
 *  @param fd    
 *  @param buf
 *  @param buf_size
 *  @return  > 0 success
 *           < 0 fail 
 */  
int write_param_file(int fd,void *buf ,int buf_size)
{
    return write(fd,buf,buf_size);
}


/** 
 *  @brief destory param file   
 *  @param file_name    
 *  @return  > 0 success
 *           < 0 fail 
 */  
int destory_param_file(char *file_name )
{
    return unlink(file_name);
}




int init_scan_param(void)
{
    int fd;
    if(exist_param_file(PARAM_FILE_NAME) != 0)
    {
        int fd = creat_param_file(PARAM_FILE_NAME);
        if(fd < 0)
        {
            printf("creat_param_file fail\r\n");
            return -1;
        }

        struct scan_param _scan_param;
        memset(&_scan_param,0,sizeof(_scan_param));
        _scan_param._serial_param.speed = DEFAULT_SPEED;
        _scan_param._serial_param.flow_ctrl = DEFAULT_FLOW_CTRL; 
        _scan_param._serial_param.databits = DEFAULT_DATABITS; 
        _scan_param._serial_param.stopbits = DEFAULT_STOPBITS; 
        _scan_param._serial_param.parity = DEFAULT_PARITY;

        _scan_param._comm_param.is_beep = DEFAULT_IS_BEEP;
        _scan_param._comm_param.is_led = DEFAULT_IS_LED;
        _scan_param._comm_param.is_illumination = DEFAULT_IS_ILLUMINATION;
        _scan_param._comm_param.support_code = DEFAULT_SUPPORT_CODE;
        _scan_param._comm_param.time_out = DEFAULT_TIME_OUT;



        if(write_param_file(fd,&_scan_param,sizeof(_scan_param)) != sizeof(_scan_param) )
        {
            printf("write_param_file fail\r\n");            
            close_param_file_fd(fd);
            return -1;
        }
            
        close_param_file_fd(fd);
    }
    return 0;
}



int read_scan_param(struct scan_param *p_scan_param,int buf_size)
{
    int fd;
    fd = open_param_file(PARAM_FILE_NAME);  
    if(fd < 0)
    {
        return -1;
    }

    if(read_param_file(fd,p_scan_param,buf_size) != buf_size)
    {
        close_param_file_fd(fd);
        return -1;
    }
    close_param_file_fd(fd);
    return 0;
}

int write_scan_param(struct scan_param *p_scan_param,int buf_size)
{
    int fd;
    fd = open_param_file(PARAM_FILE_NAME);  
    if(fd < 0)
    {
        return -1;
    }

    if(write_param_file(fd,p_scan_param,buf_size) != buf_size)
    {
        close_param_file_fd(fd);
        return -1;
    }
    close_param_file_fd(fd);
    return 0;
}



int deal_cmd_code(char *cmd_code,char *param)
{
    struct scan_param scan_param_;

    memset(&scan_param_,0,sizeof(scan_param_));

    read_scan_param(&scan_param_,sizeof(scan_param_));
    
    switch (atoi(cmd_code))
    {
    case SPEED_1200:
        scan_param_._serial_param.speed = 1200;
        break;
    case SPEED_2400:
        scan_param_._serial_param.speed = 2400;
        break;
    case SPEED_4800:
        scan_param_._serial_param.speed = 4800;
        break;
    case SPEED_9600:
        scan_param_._serial_param.speed = 9600;
        break;
    case SPEED_14400:
        scan_param_._serial_param.speed = 14400;
        break;
    case SPEED_19200:
        scan_param_._serial_param.speed = 19200;
        break;
    case SPEED_38400:
        scan_param_._serial_param.speed = 38400;
        break;
    case SPEED_56700:
        scan_param_._serial_param.speed = 56700;
        break;
    case SPEED_115200:
        scan_param_._serial_param.speed = 115200;
        break;
    case PARITY_N:
        scan_param_._serial_param.parity = 'N';
        break;
    case PARITY_E:
        scan_param_._serial_param.parity = 'E';
        break;
    case PARITY_O:
        scan_param_._serial_param.parity = 'O';
        break;
    case STOPBITS_1:
        scan_param_._serial_param.stopbits = 1;
        break;
    case STOPBITS_2:
        scan_param_._serial_param.stopbits = 2;
        break;
    case DATABITS_5:
        scan_param_._serial_param.databits = 5;
        break;
    case DATABITS_6:
        scan_param_._serial_param.databits = 6;
        break;
    case DATABITS_7:
        scan_param_._serial_param.databits = 7;
        break;
    case DATABITS_8:
        scan_param_._serial_param.databits = 8;
        break;
    case IS_BEEP_YES:
        scan_param_._comm_param.is_beep = YES;
        break;
    case IS_BEEP_NO:
        scan_param_._comm_param.is_beep = NO;
        break;
    case IS_LED_YES:
        scan_param_._comm_param.is_led = YES;
        break;
    case IS_LED_NO:
        scan_param_._comm_param.is_led = NO;
        break;
    case IS_ILLUMINATION_YES:
        scan_param_._comm_param.is_illumination = YES;
        break;
    case IS_ILLUMINATION_NO:
        scan_param_._comm_param.is_illumination = NO;
        break;
    case TIME_OUT:
        {
            char time_out[7+1] = {0};
            int time_out_i = 0;
            if(*(param+CMD_CODE_LEN) != '=')
            {
                return -1;
            }
            strcpy(time_out,param+CMD_CODE_LEN+1);

            time_out_i = atoi(time_out);

            printf("time_out_i = %d\r\n",time_out_i);

            if(time_out_i == 0)
            {
                return -1;
            }
            
            if (time_out_i > 3600000)
            {
                time_out_i = 3600000;
            }              
            scan_param_._comm_param.time_out = time_out_i;
        }
        break;
    case SUPPORT_UPC_A_YES:        
        scan_param_._comm_param.support_code.addFormat(BarcodeFormat::UPC_A);
        break;
    case SUPPORT_UPC_A_NO:
        scan_param_._comm_param.support_code.delFormat(BarcodeFormat::UPC_A);
        break;

    case SUPPORT_UPC_E_YES:
        scan_param_._comm_param.support_code.addFormat(BarcodeFormat::UPC_E);
        break;
    case SUPPORT_UPC_E_NO:
        scan_param_._comm_param.support_code.delFormat(BarcodeFormat::UPC_E);
        break;

    case SUPPORT_EAN_13_YES:
        scan_param_._comm_param.support_code.addFormat(BarcodeFormat::EAN_13);
        break;
    case SUPPORT_EAN_13_NO:
        scan_param_._comm_param.support_code.delFormat(BarcodeFormat::EAN_13);
        break;

    case SUPPORT_EAN_8_YES:
        scan_param_._comm_param.support_code.addFormat(BarcodeFormat::EAN_8);
        break;
    case SUPPORT_EAN_8_NO:
        scan_param_._comm_param.support_code.delFormat(BarcodeFormat::EAN_8);
        break;

    case SUPPORT_CODE_39_YES:
        scan_param_._comm_param.support_code.addFormat(BarcodeFormat::CODE_39);
        break;
    case SUPPORT_CODE_39_NO:
        scan_param_._comm_param.support_code.delFormat(BarcodeFormat::CODE_39);
        break;

    case SUPPORT_CODE_93_YES:
        scan_param_._comm_param.support_code.addFormat(BarcodeFormat::CODE_93);
        break;
    case SUPPORT_CODE_93_NO:
        scan_param_._comm_param.support_code.delFormat(BarcodeFormat::CODE_93);
        break;

    case SUPPORT_CODE_128_YES:
        scan_param_._comm_param.support_code.addFormat(BarcodeFormat::CODE_128);
        break;
    case SUPPORT_CODE_128_NO:
        scan_param_._comm_param.support_code.delFormat(BarcodeFormat::CODE_128);
        break;

    case SUPPORT_ITF_YES:
        scan_param_._comm_param.support_code.addFormat(BarcodeFormat::ITF);
        break;
    case SUPPORT_ITF_NO:
        scan_param_._comm_param.support_code.delFormat(BarcodeFormat::ITF);
        break;

    case SUPPORT_CODABAR_YES:
        scan_param_._comm_param.support_code.addFormat(BarcodeFormat::CODABAR);
        break;
    case SUPPORT_CODABAR_NO:
        scan_param_._comm_param.support_code.delFormat(BarcodeFormat::CODABAR);
        break;

    case SUPPORT_QR_CODE_YES:
        scan_param_._comm_param.support_code.addFormat(BarcodeFormat::QR_CODE);
        break;
    case SUPPORT_QR_CODE_NO:
        scan_param_._comm_param.support_code.delFormat(BarcodeFormat::QR_CODE);
        break;

    case SUPPORT_DATA_MATRIX_YES:
        scan_param_._comm_param.support_code.addFormat(BarcodeFormat::DATA_MATRIX);
        break;
    case SUPPORT_DATA_MATRIX_NO:
        scan_param_._comm_param.support_code.delFormat(BarcodeFormat::DATA_MATRIX);
        break;

    case SUPPORT_AZTEC_YES:
        scan_param_._comm_param.support_code.addFormat(BarcodeFormat::AZTEC);
        break;
    case SUPPORT_AZTEC_NO:
        scan_param_._comm_param.support_code.delFormat(BarcodeFormat::AZTEC);
        break;

    case SUPPORT_PDF_417_YES:
        scan_param_._comm_param.support_code.addFormat(BarcodeFormat::PDF_417);
        break;
    case SUPPORT_PDF_417_NO:
        scan_param_._comm_param.support_code.delFormat(BarcodeFormat::PDF_417);
        break;
        
    default:
        return -1;
        break;
        
    }

    

    return write_scan_param(&scan_param_,sizeof(scan_param_));
}










