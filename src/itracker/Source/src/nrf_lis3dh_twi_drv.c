
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "nrf_drv_twi.h"
#include "nrf_gpio.h"
//#include "nrf_drv_config.h"
#include "nrf_delay.h"
#include "app_util_platform.h"
#include "bsp_itracker.h"

#define LIS3DH_ADDR          0x19//(0x90U >> 1)
#define LIS3DH_TWI_BUFFER_SIZE     	  14 
#define LIS3DH_TWI_TIMEOUT			10000

static const nrf_drv_twi_t m_twi_instance = NRF_DRV_TWI_INSTANCE(0);
volatile static bool twi_tx_done = false;
volatile static bool twi_rx_done = false;

uint8_t _twi_tx_buffer[LIS3DH_TWI_BUFFER_SIZE];

static void nrf_drv_twi_event_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    switch(p_event->type)
    {
        case NRF_DRV_TWI_EVT_DONE:
            switch(p_event->xfer_desc.type)
            {
                case NRF_DRV_TWI_XFER_TX:
                    twi_tx_done = true;
                    break;
                case NRF_DRV_TWI_XFER_TXTX:
                    twi_tx_done = true;
                    break;
                case NRF_DRV_TWI_XFER_RX:
                    twi_rx_done = true;
                    break;
                case NRF_DRV_TWI_XFER_TXRX:
                    twi_rx_done = true;
                    break;
                default:
                    break;
            }
            break;
        case NRF_DRV_TWI_EVT_ADDRESS_NACK:
            break;
        case NRF_DRV_TWI_EVT_DATA_NACK:
            break;
        default:
            break;
    }
}

static uint32_t lis3dh_twi_init(void)
{
    uint32_t err_code;
    
    const nrf_drv_twi_config_t twi_lis_config = {
       .scl                = LIS3DH_TWI_SCL_PIN,
       .sda                = LIS3DH_TWI_SDA_PIN,
       .frequency          = NRF_TWI_FREQ_400K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGHEST
    };
    
    err_code = nrf_drv_twi_init(&m_twi_instance, &twi_lis_config, nrf_drv_twi_event_handler, NULL);
    if(err_code != NRF_SUCCESS)
	  {
		    return err_code;
	  }
    
    nrf_drv_twi_enable(&m_twi_instance);
	
	  return NRF_SUCCESS;
}

uint32_t nrf_drv_lis3dh_write_reg(uint8_t reg, uint8_t data)
{
    uint32_t err_code;
    uint32_t timeout = LIS3DH_TWI_TIMEOUT;

    uint8_t packet[2] = {reg, data};

    err_code = nrf_drv_twi_tx(&m_twi_instance, LIS3DH_ADDR, packet, 2, false);
    if(err_code != NRF_SUCCESS) return err_code;

    while((!twi_tx_done) && --timeout);
    if(!timeout) return NRF_ERROR_TIMEOUT;

    twi_tx_done = false;

    return err_code;
}


uint8_t nrf_drv_lis3dh_read_reg(uint8_t reg, uint8_t * p_data)
{
    uint32_t err_code;
    uint32_t timeout = LIS3DH_TWI_TIMEOUT;

    err_code = nrf_drv_twi_tx(&m_twi_instance, LIS3DH_ADDR, &reg, 1, false);
    if(err_code != NRF_SUCCESS) return err_code;

    while((!twi_tx_done) && --timeout);
    if(!timeout) return NRF_ERROR_TIMEOUT;
    twi_tx_done = false;

    err_code = nrf_drv_twi_rx(&m_twi_instance, LIS3DH_ADDR, p_data, 1);
    if(err_code != NRF_SUCCESS) return err_code;

    timeout = LIS3DH_TWI_TIMEOUT;
    while((!twi_rx_done) && --timeout);
    if(!timeout) return NRF_ERROR_TIMEOUT;
    twi_rx_done = false;
		
    return err_code;
}

uint32_t lis3dh_init(void)
{
    uint32_t err_code;
	  uint8_t id =0;
	
	  //设置mpu9250 i2c地址AD0位为1
	  //nrf_gpio_cfg_output(MPU9250_AD0_PIN);
	  //mpu9250_ad0_config(false);
	
	  //初始化TWI
	  err_code = lis3dh_twi_init();
    if(err_code != NRF_SUCCESS) return err_code;
	
    return NRF_SUCCESS;
}

#include "lis3dh_driver.h"
void lis3dh_test()
{
		uint8_t response;
		AxesRaw_t data;
		uint8_t position=0, old_position=0;
		//Inizialize MEMS Sensor
		//set ODR (turn ON device)
		response = LIS3DH_SetODR(LIS3DH_ODR_100Hz);
		if(response==1){
			DPRINTF(LOG_INFO,"%s", "\n\rSET_ODR_OK    \n\r\0");
		}
		//set PowerMode 
		response = LIS3DH_SetMode(LIS3DH_NORMAL);
		if(response==1){
			DPRINTF(LOG_INFO,"%s", "SET_MODE_OK     \n\r\0");
		}
		//set Fullscale
		response = LIS3DH_SetFullScale(LIS3DH_FULLSCALE_2);
		if(response==1){
			DPRINTF(LOG_INFO,"%s", "SET_FULLSCALE_OK\n\r\0");
		}
		//set axis Enable
		response = LIS3DH_SetAxis(LIS3DH_X_ENABLE | LIS3DH_Y_ENABLE | LIS3DH_Z_ENABLE);
		if(response==1){
			DPRINTF(LOG_INFO,"%s", "SET_AXIS_OK     \n\r\0");
		}
	
		/*********************/  
/******Example 1******/ 
#define __EXAMPLE1__H
#ifdef __EXAMPLE1__H 
  uint8_t cnt=30;
  while(cnt--){
  //get Acceleration Raw data  
  response = LIS3DH_GetAccAxesRaw(&data);
  if(response==1){
		DPRINTF(LOG_INFO,"X=%6d Y=%6d Z=%6d \r\n", data.AXIS_X, data.AXIS_Y, data.AXIS_Z);
    old_position = position;
  }
 }
	nrf_drv_twi_uninit(&m_twi_instance);
#endif /* __EXAMPLE1__H  */ 
 
 
//  i8_t tempData;
// LIS3DH_SetADCAux(MEMS_ENABLE);
// LIS3DH_SetBDU(MEMS_ENABLE);
// LIS3DH_SetTemperature(MEMS_ENABLE);
// while(1)
// {
//		  response = LIS3DH_GetTempRaw(&tempData);
//  if(response==1){
//		SEGGER_RTT_printf(0,"%x\n", tempData);
//  }
// }
}
