#ifndef __BSP_H__
#define __BSP_H__

#include "nrf_gpio.h"
#include "app_uart.h"
#include "app_error.h"

#include "rak_uart_gsm.h"

//#define BC95
//#define M35

/*
		UART PIN Assignment
		P028_UART1_RX
		P029_UART1_TX

*/
#define             LOG_RXD_PIN                        28
#define             LOG_TXD_PIN                        29

/*
		GSM PIN Assignment
		GSM_PWR_ON		--	P0.06
		GSM_TXD			--	P0.12
		GSM_RESET		--	P0.14
		GSM_PWRKEY		--	P0.15
		GSM_RXD			--	P0.20

*/

#define             GSM_PWR_ON_PIN                        6
#define             GSM_RESET_PIN                        14
#define             GSM_PWRKEY_PIN                        15

#define             GSM_TXD_PIN                        9
#define             GSM_RXD_PIN                        7


#define             GSM_PWR_ON                     nrf_gpio_pin_write ( GSM_PWR_ON_PIN, 1 )
#define             GSM_PWR_OFF                      nrf_gpio_pin_write ( GSM_PWR_ON_PIN, 0 )

#define             GSM_PWRKEY_HIGH                           nrf_gpio_pin_write ( GSM_PWRKEY_PIN, 0 )
#define             GSM_PWRKEY_LOW                            nrf_gpio_pin_write ( GSM_PWRKEY_PIN, 1 )

#define             GSM_RESET_HIGH                           nrf_gpio_pin_write ( GSM_RESET_PIN, 0 )
#define             GSM_RESET_LOW                            nrf_gpio_pin_write ( GSM_RESET_PIN, 1 )


/*
		GPS PIN Assignment
		GPS_STANDBY		--	P0.07
		GPS_TXD			--	P0.08
		GPS_RXD		--	P0.09(nfc default)
		GPS_PWR_ON		--	P0.10
		GPS_RESET		--	P0.31

*/
//#define             GPS_STANDBY_PIN                        7
//#define             GPS_TXD_PIN                        8
//#define             GPS_RXD_PIN                        9
//#define 						GPS_PWR_ON_PIN											10
//#define             GPS_RESET_PIN                        31

//#define             GPS_PWR_ON                     nrf_gpio_pin_write ( GPS_PWR_ON_PIN, 1 )
//#define             GPS_PWR_OFF                      nrf_gpio_pin_write ( GPS_PWR_ON_PIN, 0 )

//#define             GPS_RESET_HIGH                           nrf_gpio_pin_write ( GPS_RESET_PIN, 1 )
//#define             GPS_RESET_LOW                            nrf_gpio_pin_write ( GPS_RESET_PIN, 0 )


/*
		lis3dh PIN Assignment
		LIS3DH_SCL		--	P0.18
		LIS3DH_SDA		--	P0.19
		LIS3DH_INT1		--	P0.25
		LIS3DH_RES		--	P0.26
		LIS3DH_INT2		--	P0.27
		
*/
#define             LIS3DH_TWI_SCL_PIN                        18
#define             LIS3DH_TWI_SDA_PIN                        19
#define             LIS3DH_INT1_PIN                        25
#define 						LIS3DH_RES_PIN											26
#define             LIS3DH_INT2_PIN                        27

/*
		lis2mdl PIN Assignment
		LIS2MDL_SCL		--	P0.11
		LIS2MDL_SDA		--	P0.13
		LIS2MDL_INT		--	P0.16
		
*/
#define             LIS2MDL_TWI_SCL_PIN                        11
#define             LIS2MDL_TWI_SDA_PIN                        13
#define             LIS2MDL_INT_PIN                        16


/*
		bme280 PIN Assignment
		BME_CS		--	P0.02
		BME_SDI		--	P0.03
		BME_SCK		--	P0.04
		BME_SDO		--	P0.05
		
*/
#define             BME280_SPI_CS_PIN                        2
#define             BME280_SPI_SDI_PIN                        3
#define             BME280_SPI_SCK_PIN                        4
#define             BME280_SPI_SDO_PIN                        5


/*
		OPT3001 PIN Assignment
		OPT_SDA		--	P0.21
		OPT_INT		--	P0.22
		OPT_SCL		--	P0.23
		
*/
#define             OPT3001_TWI_SDA_PIN                        26//21
#define             OPT3001_INT_PIN                        		 22
#define             OPT3001_TWI_SCL_PIN                        23


typedef struct{
		double temp_value;
		double humidity_value;
		double gas_value;
		int barometer_value;
		float light_value;
		double latitude;
		double longitude;
		int acceleration;
		int x;
		int y;
		int z;
		float magnetic_x;
		float magnetic_y;
		float magnetic_z;
		int compass_value;
		int tilt;
		char ble_uuid[16];
		char sim_imei[16];
		char date[30];
		int status;
		int battery;
}tracker_data_t;

/*
*********************************************************************************************************
*                                             LOG 
*********************************************************************************************************
*/
#define     LOG_NONE     (0x00UL)
#define     LOG_ERROR    (0x01UL)
#define     LOG_WARN     (0x02UL)
#define     LOG_INFO     (0x04UL)
#define     LOG_DEBUG    (0x08UL)
#define     LOG_TRACE    (0x10UL)

#define     G_DEBUG  (LOG_NONE | LOG_ERROR | LOG_WARN | LOG_INFO | LOG_DEBUG )     
//#define     G_DEBUG  (LOG_NONE)     
#define     LOG_LEVEL_CHECK(level)      (G_DEBUG & level)


//extern OS_MUTEX   pfMutex;

//static inline void p_lock_mutex(OS_MUTEX *mutex)
//{
//  OS_ERR oserr;  
//  OSMutexPend(mutex, 0, OS_OPT_PEND_BLOCKING, NULL, &oserr);
//}


//static inline void p_unlock_mutex(OS_MUTEX *mutex)
//{
//  OS_ERR oserr;  
//  OSMutexPost(mutex, OS_OPT_POST_NONE, &oserr);
//}

static inline char* log_level_str(uint8_t level)
{
	  char* string;
    switch(level) 
		{
			case LOG_ERROR:
				string ="ERROR";
			 break;
			case LOG_WARN:
				string ="WARN";
			 break;			
			case LOG_INFO:
				string ="INFO";
			 break;		
			case LOG_DEBUG:
				string ="DEBUG";
			 break;			
			case LOG_TRACE:
				string ="TRACE";
			 break;		
      default:
         break;				
		}
    return string;
}

#define DEBUG

#ifdef DEBUG
static const char* clean_filename(const char* path)
{
  const char* filename = path + strlen(path); 
  while(filename > path)
  {
    if(*filename == '/' || *filename == '\\')
    {
      return filename + 1;
    }
    filename--;
  }
  return path;
}
#endif

#ifdef DEBUG
#define DPRINTF(level, fmt, args...)\
	if(LOG_LEVEL_CHECK(level)) {\
	SEGGER_RTT_printf(0, "[%d] ""[%s]: ", get_stamp(), log_level_str(level));\
  SEGGER_RTT_printf(0, fmt, ##args);}
#else
#define DPRINTF(fmt, args...)
#endif

#endif

