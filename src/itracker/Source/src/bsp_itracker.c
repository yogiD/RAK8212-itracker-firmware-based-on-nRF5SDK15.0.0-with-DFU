#include "boards.h"
#include "nrf_delay.h"
#include "nrf_drv_rtc.h"
#include "nrf_rtc.h"

#include "bsp_itracker.h"

#define UART_TX_BUF_SIZE 256                         /**< UART TX buffer size. */
#define UART_RX_BUF_SIZE 1                           /**< UART RX buffer size. */

typedef enum {
		UART_IDLE=0,
		LOG_USE_UART,
		GSM_USE_UART,
		GPS_USE_UART,
}uart_run_t;

uart_run_t uart_use=UART_IDLE;

const nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(2); /**< Declaring an instance of nrf_drv_rtc for RTC0. */

void gsm_off(int emergency) {

}


void Gsm_Gpio_Init(void)
{
		nrf_gpio_cfg_output(GSM_PWR_ON_PIN);
		nrf_gpio_cfg_output(GSM_RESET_PIN);
		nrf_gpio_cfg_output(GSM_PWRKEY_PIN);
		
		//open gsm uart
}

//void Gps_Gpio_Init()
//{
//		nrf_gpio_cfg_output(GPS_PWR_ON_PIN);
//		nrf_gpio_cfg_output(GPS_RESET_PIN);
//}


//void gps_uart_event_handle(app_uart_evt_t * p_event)
//{
//    //static uint8_t data_array[BLE_NUS_MAX_DATA_LEN];
//    static uint8_t index = 0;
//    uint32_t       err_code;

//    switch (p_event->evt_type)
//    {
//        case APP_UART_DATA_READY:
////            UNUSED_VARIABLE(app_uart_get(&data_array[index]));
////            index++;

////            if ((data_array[index - 1] == '\n') || (index >= (m_ble_nus_max_data_len)))
////            {
////                NRF_LOG_DEBUG("Ready to send data over BLE NUS");
////                NRF_LOG_HEXDUMP_DEBUG(data_array, index);

////                do
////                {
////                    uint16_t length = (uint16_t)index;
////                    err_code = ble_nus_string_send(&m_nus, data_array, &length);
////                    if ( (err_code != NRF_ERROR_INVALID_STATE) && (err_code != NRF_ERROR_BUSY) )
////                    {
////                        APP_ERROR_CHECK(err_code);
////                    }
////                } while (err_code == NRF_ERROR_BUSY);

////                index = 0;
////            }
////						uint8_t rx_data;
////						app_uart_get(&rx_data);
////						Gsm_RingBuf(rx_data);
//										GpsMcuIrqNotify();
//            break;

////        case APP_UART_COMMUNICATION_ERROR:
////            APP_ERROR_HANDLER(p_event->data.error_communication);
////            break;

//        case APP_UART_FIFO_ERROR:
//            APP_ERROR_HANDLER(p_event->data.error_code);
//            break;

//        default:
//            break;
//    }
//}

//void Gps_Uart_Init()
//{
//		uint32_t err_code;
//	
//    const app_uart_comm_params_t comm_params =
//    {
//          GPS_RXD_PIN,
//          GPS_TXD_PIN,
//          0,
//          0,
//          APP_UART_FLOW_CONTROL_DISABLED,    //关闭流控
//          false,
//          UART_BAUDRATE_BAUDRATE_Baud9600  //波特率设置为115200bps
//    };

//    APP_UART_FIFO_INIT(&comm_params,
//                         UART_RX_BUF_SIZE,
//                         UART_TX_BUF_SIZE,
//                         gps_uart_event_handle,
//                         APP_IRQ_PRIORITY_LOW,
//                         err_code);

//    APP_ERROR_CHECK(err_code);
//}

void gsm_setup()
{
		
}




void uart_event_handle(app_uart_evt_t * p_event)
{
    //static uint8_t data_array[BLE_NUS_MAX_DATA_LEN];
    static uint8_t index = 0;
    uint32_t       err_code;

    switch (p_event->evt_type)
    {
        case APP_UART_DATA_READY:
//            UNUSED_VARIABLE(app_uart_get(&data_array[index]));
//            index++;

//            if ((data_array[index - 1] == '\n') || (index >= (m_ble_nus_max_data_len)))
//            {
//                NRF_LOG_DEBUG("Ready to send data over BLE NUS");
//                NRF_LOG_HEXDUMP_DEBUG(data_array, index);

//                do
//                {
//                    uint16_t length = (uint16_t)index;
//                    err_code = ble_nus_string_send(&m_nus, data_array, &length);
//                    if ( (err_code != NRF_ERROR_INVALID_STATE) && (err_code != NRF_ERROR_BUSY) )
//                    {
//                        APP_ERROR_CHECK(err_code);
//                    }
//                } while (err_code == NRF_ERROR_BUSY);

//                index = 0;
//            }

						if(uart_use == GSM_USE_UART)
						{
								uint8_t rx_data;
								app_uart_get(&rx_data);
								Gsm_RingBuf(rx_data);		
								//SEGGER_RTT_printf(0, "%c", rx_data);							
						}
//						else if(uart_use == GPS_USE_UART)
//						{
//								GpsMcuIrqNotify();
//						}
				
            break;

//        case APP_UART_COMMUNICATION_ERROR:
//            APP_ERROR_HANDLER(p_event->data.error_communication);
//            break;

        case APP_UART_FIFO_ERROR:
            APP_ERROR_HANDLER(p_event->data.error_code);
            break;

        default:
            break;
    }
}

/*******************************************************************************
 * 描  述 : 串口初始化。波特率115200bps，流控关闭
 * 参  数 : 无
 * 返回值 : 无
 ******************************************************************************/
void Gsm_Uart_Init(void)
{
		if(uart_use != GSM_USE_UART)
		{
				uint32_t err_code;
				app_uart_close();

				const app_uart_comm_params_t comm_params =
				{
							GSM_RXD_PIN,
							GSM_TXD_PIN,
							0,
							0,
							APP_UART_FLOW_CONTROL_DISABLED,
							false,
							UART_BAUDRATE_BAUDRATE_Baud115200
				};
			
				APP_UART_FIFO_INIT(&comm_params,
														 UART_RX_BUF_SIZE,
														 UART_TX_BUF_SIZE,
														 uart_event_handle,
														 APP_IRQ_PRIORITY_LOW,
														 err_code);

				APP_ERROR_CHECK(err_code);
				
				uart_use = GSM_USE_UART;
		}
    
}

//void Gps_Uart_Init(void)
//{
//		if(uart_use != GPS_USE_UART)
//    {
//				uint32_t err_code;
//				app_uart_close();				
//				const app_uart_comm_params_t comm_params =
//				{
//							GPS_RXD_PIN,
//							GPS_TXD_PIN,
//							0,
//							0,
//							APP_UART_FLOW_CONTROL_DISABLED,
//							false,
//							UART_BAUDRATE_BAUDRATE_Baud9600
//				};

//				APP_UART_FIFO_INIT(&comm_params,
//														 UART_RX_BUF_SIZE,
//														 UART_TX_BUF_SIZE,
//														 uart_event_handle,
//														 APP_IRQ_PRIORITY_LOW,
//														 err_code);

//				APP_ERROR_CHECK(err_code);
//				
//				uart_use = GPS_USE_UART;
//		}
//}

/** @brief: Function for handling the RTC0 interrupts.
 * Triggered on TICK and COMPARE0 match.
 */
static void rtc_handler(nrf_drv_rtc_int_type_t int_type)
{
//    if (int_type == NRF_DRV_RTC_INT_COMPARE0)
//    {
//        nrf_gpio_pin_toggle(COMPARE_EVENT_OUTPUT);
//    }
//    else if (int_type == NRF_DRV_RTC_INT_TICK)
//    {
//        nrf_gpio_pin_toggle(TICK_EVENT_OUTPUT);
//    }
}

/** @brief Function starting the internal LFCLK XTAL oscillator.
 */
static void lfclk_config(void)
{
    ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_clock_lfclk_request(NULL);
}

void rtc_config(void)
{
	// Initialize RTC instance
	nrf_drv_rtc_config_t config = NRF_DRV_RTC_DEFAULT_CONFIG;
	config.prescaler = RTC_FREQ_TO_PRESCALER(RTC_DEFAULT_CONFIG_FREQUENCY);
	ret_code_t err_code = nrf_drv_rtc_init(&rtc, &config, rtc_handler);
	APP_ERROR_CHECK(err_code);

	// Enable tick event - no interrupt, we need tick just to drive ADC
	nrf_drv_rtc_tick_enable(&rtc, false);
	nrf_drv_rtc_overflow_enable(&rtc, true);

	// Power on RTC instance
	nrf_drv_rtc_enable(&rtc);
}

uint32_t get_stamp(void)
{
	uint32_t ticks = nrf_drv_rtc_counter_get(&rtc);
	return (ticks / RTC_DEFAULT_CONFIG_FREQUENCY);
}

