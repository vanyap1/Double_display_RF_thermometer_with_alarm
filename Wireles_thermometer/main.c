/*
 * Wireles_thermometer.c
 *
 * Created: 25.07.2023 11:20:49
 * Author : princ
 */ 
#include "config.h"
#include <avr/io.h>
#include <u8g2.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "lib/display_fn.h"
#include "lib/uart_hal.h"
#include "lib/gpio_driver.h"
#include "lib/twi_hal.h"
#include "lib/rtc.h"
#include "string.h"
#include "float.h"
#include "stdbool.h"
#include <stdio.h>

gpio health = {(uint8_t *)&PORTB , PORTB5};
gpio lcd_blk = {(uint8_t *)&PORTE , PORTE2};
static FILE mystdout = FDEV_SETUP_STREAM((void *)uart_send_byte, NULL, _FDEV_SETUP_WRITE);
u8g2_t u8g2;

uint8_t serial_buff[128];

uint8_t att = 0;

int main(void)
{
	sei();
	stdout = &mystdout;
	u8g2_Setup_st7565_zolen_128x64_f( &u8g2, U8G2_MIRROR_VERTICAL, u8x8_byte_4wire_sw_spi, u8x8_avr_gpio_and_delay );
	uart_init(38400,0);
	//twi_init(400000);


	
	//rtc_set(&sys_rtc);
	
	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);
	u8g2_SetFlipMode(&u8g2, 1);
	u8g2_SetContrast(&u8g2, 120);
	
	set_pin_dir(&lcd_blk , PORT_DIR_OUT); set_pin_level(&lcd_blk, true);
	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_freedoomr10_mu);
	u8g2_SendBuffer(&u8g2);
    while (1) 
    {
		if (serial_complete()){
			uint8_t const *data_p = (void *)serial_read_data();
			
			uint8_t val = parseString((void *)data_p);
			if (val < 32){
				att = val;
			}else{
				att = 31;
			}
			printf("val %d ", att);
			u8g2_DrawStr(&u8g2, 1, 14, (void*)data_p);
			u8g2_SendBuffer(&u8g2);
			
		}
    }
}

