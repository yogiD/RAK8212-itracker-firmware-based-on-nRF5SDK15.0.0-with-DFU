
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "nrf_drv_twi.h"
#include "nrf_gpio.h"
//#include "nrf_drv_config.h"
#include "nrf_delay.h"
#include "app_util_platform.h"
#include "lis2mdl_reg.h"
#include "bsp_itracker.h"

#define LIS2MDL_ADDR          0x1E//(0x90U >> 1)
#define LIS2MDL_TWI_BUFFER_SIZE     	  14 
#define LIS2MDL_TWI_TIMEOUT			10000

static const nrf_drv_twi_t m_twi_instance = NRF_DRV_TWI_INSTANCE(0);
volatile static bool twi_tx_done = false;
volatile static bool twi_rx_done = false;

static uint8_t twi_tx_buffer[LIS2MDL_TWI_BUFFER_SIZE];

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

static uint32_t lis2mdl_twi_init(void)
{
    uint32_t err_code;
    
    const nrf_drv_twi_config_t twi_lis_config = {
       .scl                = LIS2MDL_TWI_SCL_PIN,
       .sda                = LIS2MDL_TWI_SDA_PIN,
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

// The TWI driver is not able to do two transmits without repeating the ADDRESS + Write bit byte
// Hence we need to merge the MPU register address with the buffer and then transmit all as one transmission
static void merge_register_and_data(uint8_t * new_buffer, uint8_t reg, uint8_t * p_data, uint32_t length)
{
    new_buffer[0] = reg;
    memcpy((new_buffer + 1), p_data, length);
}
/*
 *   Replace the functions "platform_write" and "platform_read" with your
 *   platform specific read and write function.
 *   This example use an STM32 evaluation board and CubeMX tool.
 *   In this case the "*handle" variable is usefull in order to select the
 *   correct interface but the usage uf "*handle" is not mandatory.
 */

static int32_t platform_write(void *handle, uint8_t Reg, uint8_t *Bufp,
                              uint16_t len)
{
    // This burst write function is not optimal and needs improvement.
    // The new SDK 11 TWI driver is not able to do two transmits without repeating the ADDRESS + Write bit byte
    uint32_t err_code;
    uint32_t timeout = LIS2MDL_TWI_TIMEOUT;

    // Merging MPU register address and p_data into one buffer.
    merge_register_and_data(twi_tx_buffer, Reg, Bufp, len);

    // Setting up transfer
    nrf_drv_twi_xfer_desc_t xfer_desc;
    xfer_desc.address = LIS2MDL_ADDR;
    xfer_desc.type = NRF_DRV_TWI_XFER_TX;
    xfer_desc.primary_length = len + 1;
    xfer_desc.p_primary_buf = twi_tx_buffer;

    // Transferring
    err_code = nrf_drv_twi_xfer(&m_twi_instance, &xfer_desc, 0);

    while((!twi_tx_done) && --timeout);
    if(!timeout) return NRF_ERROR_TIMEOUT;
    twi_tx_done = false;

    return err_code;
}


static int32_t platform_read(void *handle, uint8_t Reg, uint8_t *Bufp,
                             uint16_t len)
{
		uint32_t err_code;
    uint32_t timeout = LIS2MDL_TWI_TIMEOUT;

    err_code = nrf_drv_twi_tx(&m_twi_instance, LIS2MDL_ADDR, &Reg, 1, false);
    if(err_code != NRF_SUCCESS) return err_code;

    while((!twi_tx_done) && --timeout);
    if(!timeout) return NRF_ERROR_TIMEOUT;
    twi_tx_done = false;

    err_code = nrf_drv_twi_rx(&m_twi_instance, LIS2MDL_ADDR, Bufp, len);
    if(err_code != NRF_SUCCESS) return err_code;

    timeout = LIS2MDL_TWI_TIMEOUT;
    while((!twi_rx_done) && --timeout);
    if(!timeout) return NRF_ERROR_TIMEOUT;
    twi_rx_done = false;

    return err_code;
  return 0;
}

lis2mdl_ctx_t dev_ctx;
uint32_t lis2mdl_init(void)
{
    uint32_t err_code;
		
	  //≥ı ºªØTWI
	  err_code = lis2mdl_twi_init();
    if(err_code != NRF_SUCCESS) return err_code;
	
		/*
		 *  Initialize mems driver interface
		 */

		dev_ctx.write_reg = platform_write;
		dev_ctx.read_reg = platform_read;

    return NRF_SUCCESS;
}


void lis2mdl_test()
{
		/* Private variables ---------------------------------------------------------*/
	#define TX_BUF_DIM          1000
static axis3bit16_t data_raw_magnetic;
static axis1bit16_t data_raw_temperature;
static float magnetic_mG[3];
static float temperature_degC;
static uint8_t whoamI, rst;
static uint8_t tx_buffer[TX_BUF_DIM];
	 /*
   *  Check device ID
   */
  whoamI = 0;
  lis2mdl_device_id_get(&dev_ctx, &whoamI);
  if ( whoamI != LIS2MDL_ID )
    while(1); /*manage here device not found */
	/*
   *  Restore default configuration
   */
  lis2mdl_reset_set(&dev_ctx, PROPERTY_ENABLE);
  do {
    lis2mdl_reset_get(&dev_ctx, &rst);
  } while (rst);
  /*
   *  Enable Block Data Update
   */
  lis2mdl_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
  /*
   * Set Output Data Rate
   */
  lis2mdl_data_rate_set(&dev_ctx, LIS2MDL_ODR_10Hz);
  /*
   * Set / Reset sensor mode
   */  
  lis2mdl_set_rst_mode_set(&dev_ctx, LIS2MDL_SENS_OFF_CANC_EVERY_ODR);
  /*
   * Enable temperature compensation
   */  
  lis2mdl_offset_temp_comp_set(&dev_ctx, PROPERTY_ENABLE);
  /*
   * Set device in continuos mode
   */   
  lis2mdl_operating_mode_set(&dev_ctx, LIS2MDL_CONTINUOUS_MODE);
  
  /*
   * Read samples in polling mode (no int)
   */
	uint8_t cnt=30;
	uint8_t cnt2=0;
  while(cnt--)
  {
    /*
     * Read output only if new value is available
     */
    lis2mdl_reg_t reg;
    lis2mdl_status_get(&dev_ctx, &reg.status_reg);

    if (reg.status_reg.zyxda)
    {
      /* Read magnetic field data */
      memset(data_raw_magnetic.u8bit, 0x00, 3*sizeof(int16_t));
      lis2mdl_magnetic_raw_get(&dev_ctx, data_raw_magnetic.u8bit);
      magnetic_mG[0] = LIS2MDL_FROM_LSB_TO_mG( data_raw_magnetic.i16bit[0]);
      magnetic_mG[1] = LIS2MDL_FROM_LSB_TO_mG( data_raw_magnetic.i16bit[1]);
      magnetic_mG[2] = LIS2MDL_FROM_LSB_TO_mG( data_raw_magnetic.i16bit[2]);
      
      sprintf((char*)tx_buffer, "Magnetic field [mG]:%4.2f\t%4.2f\t%4.2f\r\n",
              magnetic_mG[0], magnetic_mG[1], magnetic_mG[2]);
      //tx_com( tx_buffer, strlen( (char const*)tx_buffer ) );
      DPRINTF(LOG_INFO, "%s", tx_buffer);
			
      /* Read temperature data */
      memset(data_raw_temperature.u8bit, 0x00, sizeof(int16_t));
      lis2mdl_temperature_raw_get(&dev_ctx, data_raw_temperature.u8bit);
      temperature_degC = LIS2MDL_FROM_LSB_TO_degC( data_raw_temperature.i16bit );
       
      sprintf((char*)tx_buffer, "Temperature [degC]:%6.2f\r\n", temperature_degC );
      //tx_com( tx_buffer, strlen( (char const*)tx_buffer ) );
			DPRINTF(LOG_INFO, "%s", tx_buffer);
			cnt2++;
    }
		else
		{
				DPRINTF(LOG_INFO, "fail...\n");
		}
  }
	DPRINTF(LOG_INFO, "ok cnt=%d\n", cnt2);
	nrf_drv_twi_uninit(&m_twi_instance);
}
