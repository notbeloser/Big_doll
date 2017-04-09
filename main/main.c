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
char str[20],temp;
int i=0;
char temp,str[20];
double t;
int servo=0;
void uart_test(){
	scanf("%c",&temp);
			if(temp!=0){
				printf("%c",temp);
				str[i]=temp;
				i++;
				fflush(stdout);
				if(temp=='\n'){
					if(sscanf(str,"%d %lf\n",&servo,&t) == 2){
						ESP_LOGI("Servo_test","Servo%d,t =%lf",servo,t);
						switch(servo){
						case 0:
							d.l_eye.r=t;
							break;
						case 1:
							d.l_eye.angle=t;
							break;
						case 2:
							d.r_eye.r=t;
							break;
						case 3:
							d.r_eye.angle=t;
							break;
						case 4:
							d.l_ear.angle=t;
							break;
						case 5:
							d.r_ear.angle=t;
							break;
						case 6:
							d.l_bow.angle=t;
							break;
						case 7:
							d.l_bow.y=t;
							break;
						case 8:
							d.r_bow.angle=t;
							break;
						case 9:
							d.r_bow.y=t;
							break;
						case 10:
							d.mouth.angle=t;
							break;
						}
						doll_set(d);
					}
					memset(str,0,sizeof(str));
					i=0;
				}
			}
			vTaskDelay(10 / portTICK_PERIOD_MS);
			temp=0;
}
void app_main(){
	nvs_flash_init();
	d=doll_default_setting();
	doll_init(d);
	while(1){
		for(double a=0;a<20;a+=0.4){
			d.l_ear.angle=d.r_ear.angle=a;
			delay(20);
			doll_set(d);
		}
		for(double a=20;a>0;a-=0.4){
			d.l_ear.angle=d.r_ear.angle=a;
			delay(20);
			doll_set(d);
		}
	}
}

