#include <string.h>
#include <stdio.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "freertos/xtensa_api.h"
#include "freertos/queue.h"
#include "freertos/portmacro.h"

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "esp_attr.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include "driver/ledc.h"
#include "driver/pcnt.h"
#include "driver/gpio.h"
#include "driver/periph_ctrl.h"
#include "driver/uart.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "rom/uart.h"
#include "driver/i2c.h"
#include "soc/soc.h"
#include "soc/ledc_reg.h"
#include "soc/ledc_struct.h"
#include "soc/gpio_sig_map.h"
#include "soc/uart_struct.h"

#include "Arduino.h"
#include "esp32-hal.h"
#include "big_doll.h"
#define true 1
#define false 0

#define RXD1  18
#define TXD1  19
#define RXD2  21
#define TXD2  22
#define BUF_SIZE (1024)

static void big_doll(){
	doll d;
	int l_bt,r_bt,l_x,l_y,r_x,r_y;
	d=doll_default_setting();
	doll_init(d);
    uart_config_t uart_config = {
        .baud_rate = 38400,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1,TXD1, RXD1, NULL, NULL);
    uart_driver_install(UART_NUM_1, BUF_SIZE * 2, 0, 0, NULL, 0);

	uart_param_config(UART_NUM_2, &uart_config);
	uart_set_pin(UART_NUM_2,TXD2, RXD2, NULL, NULL);
	uart_driver_install(UART_NUM_2, BUF_SIZE * 2, 0, 0, NULL, 0);

    uint8_t* left_str = (uint8_t*) malloc(BUF_SIZE);
    uint8_t* right_str = (uint8_t*) malloc(BUF_SIZE);

    memset(left_str,0,BUF_SIZE);
    memset(right_str,0,BUF_SIZE);
	int state=0,left_str_len=0,right_str_len=0;
    while(1) {
	switch(state){
		case 0:
			left_str_len = uart_read_bytes(UART_NUM_1, left_str, BUF_SIZE, 5/ portTICK_RATE_MS);
			if(left_str_len>0)
				state=1;
			break;
		case 1:
			right_str_len = uart_read_bytes(UART_NUM_2, right_str, BUF_SIZE, 5/ portTICK_RATE_MS);
			if(right_str_len>0)
				state=2;
			break;
		case 2:
			printf("%s\t%s",left_str,right_str);
			if( sscanf((char *)left_str,"%d,%d,%d\n",&l_bt,&l_x,&l_y) == 3 && sscanf((char *)right_str,"%d,%d,%d\n",&r_bt,&r_x,&r_y) == 3		){
				d.l_bow.y=d.r_bow.y=((double)l_y-512)/512*10+8;
				d.l_bow.angle=d.r_bow.angle=((double)l_x-512)/512*30;
				if(l_bt == 8){
					d.l_ear.angle=40;
				}
				else if(l_bt==4){
					d.l_ear.angle=25;
				}
				else if(l_bt==2){
					d.l_ear.angle=10;
				}
				else if(l_bt==1){
					d.l_ear.angle=-10;
				}
				double eye_x=((double)r_x-511)/2.1;
				double eye_y=((double)r_y-511)/2.1;
				double eye_r = sqrt(pow(eye_x,2) + pow(eye_y,2) );
				double eye_angle=degrees(atan2(eye_y+0.001,eye_x+0.001));
				d.l_eye.r  =d.r_eye.r= eye_r;
				d.l_eye.angle =d.r_eye.angle = eye_angle;

				if(r_bt == 8){
					d.r_ear.angle=40;
				}
				else if(r_bt==4){
					d.r_ear.angle=25;
				}
				else if(r_bt==2){
					d.r_ear.angle=10;
				}
				else if(r_bt==1){
					d.r_ear.angle=-10;
				}
				doll_set(d);
			}
		    memset(left_str,0,BUF_SIZE);
		    memset(right_str,0,BUF_SIZE);
			state=0;
			break;
		}
    }
}

void app_main(){
    xTaskCreate(big_doll, "big_doll",2048, NULL, 10, NULL);
}

