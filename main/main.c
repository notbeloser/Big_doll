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

#include "Arduino.h"
#include "esp32-hal.h"
#include "big_doll.h"
#define true 1
#define false 0
doll d;
int i=0;
char temp,str[20];
double t;
int servo=0;
uart_t* left_uart;
uart_t* right_uart;
char left_str[50],right_str[50];
int uart_readString(char* s,uart_t* u){
	int index=0;
	while(uartAvailable(u)){
		if(index==0)
			memset(s,0,50);
		s[index]=uartRead(u);
		if(s[index]=='\n')
			break;
		index++;
	}
	return index;
}
int l_bt,r_bt,l_x,l_y,r_x,r_y;
//void uart_test(){
//	scanf("%c",&temp);
//	if(temp!=0){
//		printf("%c",temp);
//		str[i]=temp;
//		i++;
//		fflush(stdout);
//		if(temp=='\n'){
//			if(sscanf(str,"%d %lf\n",&servo,&t) == 2){
//				ESP_LOGI("Servo_test","Servo%d,t =%lf",servo,t);
//				switch(servo){
//				case 0:
//					d.l_eye.r=t;
//					break;
//				case 1:
//					d.l_eye.angle=t;
//					break;
//				case 2:
//					d.r_eye.r=t;
//					break;
//				case 3:
//					d.r_eye.angle=t;
//					break;
//				case 4:
//					d.l_ear.angle=t;
//					break;
//				case 5:
//					d.r_ear.angle=t;
//					break;
//				case 6:
//					d.l_bow.angle=t;
//					break;
//				case 7:
//					d.l_bow.y=t;
//					break;
//				case 8:
//					d.r_bow.angle=t;
//					break;
//				case 9:
//					d.r_bow.y=t;
//					break;
//				case 10:
//					d.mouth.angle=t;
//					break;
//				}
//				doll_set(d);
//			}
//			memset(str,0,sizeof(str));
//			i=0;
//		}
//	}
//	vTaskDelay(10 / portTICK_PERIOD_MS);
//	temp=0;
//}
void app_main(){
	nvs_flash_init();
	d=doll_default_setting();
	doll_init(d);
	left_uart=uartBegin(1, 115200, SERIAL_8N1, 18,19, 2048, 0);
	right_uart=uartBegin(2, 115200, SERIAL_8N1, 21,22, 2048, 0);
	while(1){
			if( uart_readString(left_str,left_uart)>0 ){
				if(sscanf(left_str,"%d,%d,%d\n",&l_bt,&l_x,&l_y) == 3){
				printf("%d,%d,%d\n",l_bt,l_x,l_y);
				d.l_bow.y=d.r_bow.y=((double)l_y-512)/512*10+8;
				d.l_bow.angle=d.r_bow.angle=((double)l_x-512)/512*30;
				printf("%f,%f\n\n",d.r_bow.y, d.l_bow.angle );
				doll_set(d);
				}
		}
		if(uart_readString(right_str,right_uart)>0){
			if(sscanf(right_str,"%d,%d,%d\n",&r_bt,&r_x,&r_y) == 3){
				double eye_x=((double)r_x-511)/2.1;
				double eye_y=((double)r_y-511)/2.1;
				double eye_r = sqrt(pow(eye_x,2) + pow(eye_y,2) );
				double eye_angle=degrees(atan2(eye_y+0.001,eye_x+0.001));
				d.l_eye.r  =d.r_eye.r= eye_r;
				d.l_eye.angle =d.r_eye.angle = eye_angle;
			}
		}
	}
}

